#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <atomic>
#include <thread>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <cmath>

struct RingBuffer {
    std::vector<float> data;
    size_t capacity;
    size_t writeIndex = 0;
    bool full = false;
    std::mutex mtx;

    RingBuffer(size_t cap) : data(cap, 0.f), capacity(cap) {}

    void push(const float* samples, size_t count) {
        std::lock_guard<std::mutex> lock(mtx);
        for (size_t i = 0; i < count; ++i) {
            data[writeIndex] = samples[i];
            writeIndex = (writeIndex + 1) % capacity;
            if (writeIndex == 0) full = true;
        }
    }

    // Get newest count samples (if available, else as many as exist)
    void getRecent(size_t count, std::vector<float>& out) {
        std::lock_guard<std::mutex> lock(mtx);
        size_t available = full ? capacity : writeIndex;
        if (count > available) count = available;
        out.resize(count);
        size_t start = (writeIndex + capacity - count) % capacity;
        for (size_t i = 0; i < count; ++i) {
            out[i] = data[(start + i) % capacity];
        }
    }

    size_t available() {
        std::lock_guard<std::mutex> lock(mtx);
        return full ? capacity : writeIndex;
    }
};

class MicRecorder : public sf::SoundRecorder {
public:
    RingBuffer& rb;
    std::atomic<size_t> lastChunkSize{0};
    MicRecorder(RingBuffer& buf) : rb(buf) {
        setChannelCount(1); // We'll convert to mono if device gives more
    }
    bool onStart() override {
        std::cout << "[INFO] Microphone recording started. SampleRate=" << getSampleRate() << " Channels=" << getChannelCount() << "\n";
        return true;
    }
    bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount) override {
        lastChunkSize = sampleCount;
        // If multi-channel, take first channel only
        unsigned ch = getChannelCount();
        if (ch < 1) return true;
        std::vector<float> tmp;
        tmp.reserve(sampleCount / ch);
        for (size_t i = 0; i < sampleCount; i += ch) {
            int16_t s = samples[i];
            float v = (s <= -32768) ? -1.f : (static_cast<float>(s) / 32768.f);
            tmp.push_back(v);
        }
        rb.push(tmp.data(), tmp.size());
        return true;
    }
    void onStop() override {
        std::cout << "[INFO] Microphone recording stopped.\n";
    }
};

struct WavData {
    unsigned sampleRate = 0;
    unsigned channels = 0;
    std::vector<float> monoSamples; // normalized mono
};

bool readWav16PCM(const std::string& path, WavData& out, std::string& err) {
    std::ifstream f(path, std::ios::binary);
    if (!f) { err = "Cannot open WAV file."; return false; }
    auto readU32 = [&](uint32_t& v){ f.read(reinterpret_cast<char*>(&v),4); };
    auto readU16 = [&](uint16_t& v){ f.read(reinterpret_cast<char*>(&v),2); };
    char riff[4]; f.read(riff,4);
    if (f.gcount()!=4 || std::string(riff,4)!="RIFF") { err="Not RIFF."; return false; }
    uint32_t fileSize; readU32(fileSize);
    char wave[4]; f.read(wave,4);
    if (std::string(wave,4)!="WAVE") { err="Not WAVE."; return false; }
    uint16_t audioFormat=0, channels=0, bitsPerSample=0;
    uint32_t sampleRate=0;
    std::vector<char> dataChunk;
    while (f && f.peek()!=EOF) {
        char chunkId[4]; f.read(chunkId,4); if (!f) break;
        uint32_t chunkSize; readU32(chunkSize);
        std::string id(chunkId,4);
        if (id=="fmt ") {
            readU16(audioFormat);
            readU16(channels);
            readU32(sampleRate);
            uint32_t byteRate; readU32(byteRate);
            uint16_t blockAlign; readU16(blockAlign);
            readU16(bitsPerSample);
            if (chunkSize > 16) f.ignore(chunkSize - 16);
        } else if (id=="data") {
            dataChunk.resize(chunkSize);
            f.read(dataChunk.data(), chunkSize);
        } else {
            f.ignore(chunkSize);
        }
    }
    if (audioFormat!=1) { err="Unsupported WAV (not PCM)."; return false; }
    if (bitsPerSample!=16) { err="Unsupported bitsPerSample (need 16)."; return false; }
    if (channels!=1 && channels!=2) { err="Unsupported channel count."; return false; }
    if (dataChunk.empty()) { err="No data chunk."; return false; }
    size_t sampleCount = dataChunk.size() / (channels * 2);
    out.sampleRate = sampleRate;
    out.channels = channels;
    out.monoSamples.reserve(sampleCount);
    const int16_t* pcm = reinterpret_cast<const int16_t*>(dataChunk.data());
    for (size_t i=0;i<sampleCount;i++) {
        int16_t s = pcm[i*channels]; // left or mono
        float v = (s <= -32768) ? -1.f : (static_cast<float>(s) / 32768.f);
        out.monoSamples.push_back(v);
    }
    return true;
}

// Simple CLI parsing
struct Options {
    bool showHelp=false;
    bool listDevices=false;
    std::string inputWav;
    std::string deviceName;
    int deviceIndex=-1;
    unsigned sampleRate=44100;
};

void printHelp() {
    std::cout <<
"Mini Oscilloscope (SFML)\n"
"Usage: mini_oscilloscope [options]\n"
"  --help                 Show help\n"
"  --list-devices         List recording devices\n"
"  --input <file.wav>     Use WAV file (16bit PCM mono/stereo) as looping input\n"
"  --sr <rate>            Sampling rate (mic mode) default 44100\n"
"  --device <name>        Specify recording device name\n"
"  --device-index <i>     Specify recording device by index from list\n";
}

bool parseArgs(int argc, char** argv, Options& opt) {
    for (int i=1;i<argc;i++) {
        std::string a=argv[i];
        if (a=="--help") opt.showHelp=true;
        else if (a=="--list-devices") opt.listDevices=true;
        else if (a=="--input" && i+1<argc) opt.inputWav=argv[++i];
        else if (a=="--sr" && i+1<argc) opt.sampleRate=static_cast<unsigned>(std::stoul(argv[++i]));
        else if (a=="--device" && i+1<argc) opt.deviceName=argv[++i];
        else if (a=="--device-index" && i+1<argc) opt.deviceIndex=std::stoi(argv[++i]);
        else { std::cout << "[WARN] Unknown or incomplete arg: " << a << "\n"; }
    }
    return true;
}

std::string timestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm,&t);
#else
    localtime_r(&t,&tm);
#endif
    std::ostringstream oss;
    oss<< std::put_time(&tm,"%Y%m%d_%H%M%S");
    return oss.str();
}

int main(int argc, char** argv) {
    Options opt;
    parseArgs(argc, argv, opt);
    if (opt.showHelp) { printHelp(); return 0; }

    if (opt.listDevices) {
        auto devices = sf::SoundRecorder::getAvailableDevices();
        std::cout << "Recording Devices:\n";
        for (size_t i=0;i<devices.size();++i) {
            std::cout << " ["<<i<<"] " << devices[i] << "\n";
        }
        return 0;
    }

    // Setup ring buffer (3+ seconds)
    const unsigned targetSampleRate = opt.sampleRate;
    size_t ringCapacity = targetSampleRate * 3;
    RingBuffer ring(ringCapacity);

    bool useWav = !opt.inputWav.empty();
    WavData wav;
    std::atomic<bool> wavThreadRunning{false};
    std::thread wavThread;

    if (useWav) {
        std::string err;
        if (!readWav16PCM(opt.inputWav, wav, err)) {
            std::cout << "[ERROR] WAV load failed: " << err << "\n";
            return 1;
        }
        std::cout << "[INFO] WAV loaded. File=" << opt.inputWav
                  << " SampleRate=" << wav.sampleRate
                  << " Channels=" << wav.channels
                  << " Samples=" << wav.monoSamples.size() << "\n";
        // Override ring capacity if file SR differs
        ringCapacity = wav.sampleRate * 3;
        ring = RingBuffer(ringCapacity);
        wavThreadRunning = true;
        wavThread = std::thread([&](){
            size_t pos=0;
            const unsigned sr = wav.sampleRate;
            const size_t chunk = 512;
            auto chunkDur = std::chrono::duration<double>(double(chunk)/double(sr));
            while (wavThreadRunning) {
                std::vector<float> buf;
                buf.reserve(chunk);
                for (size_t i=0;i<chunk;i++) {
                    float v = wav.monoSamples[pos];
                    buf.push_back(v);
                    pos = (pos + 1) % wav.monoSamples.size();
                }
                ring.push(buf.data(), buf.size());
                std::this_thread::sleep_for(chunkDur);
            }
        });
    } else {
        if (!sf::SoundRecorder::isAvailable()) {
            std::cout << "[ERROR] No recording capability.\n";
            return 1;
        }
        auto devices = sf::SoundRecorder::getAvailableDevices();
        std::string chosen = sf::SoundRecorder::getDefaultDevice();
        if (opt.deviceIndex >=0 && opt.deviceIndex < (int)devices.size()) chosen = devices[opt.deviceIndex];
        if (!opt.deviceName.empty()) chosen = opt.deviceName;
        bool found=false;
        for (auto& d: devices) if (d==chosen) { found=true; break; }
        if (!found) {
            std::cout << "[ERROR] Device not found: " << chosen << "\n";
            return 1;
        }
        MicRecorder recorder(ring);
        recorder.setDevice(chosen);
        if (!recorder.start(opt.sampleRate)) {
            std::cout << "[ERROR] Failed to start recorder.\n";
            return 1;
        }
        std::cout << "[INFO] Using input device: " << chosen << "\n";
        // We'll stop recorder on exit (RAII like)
        // Move recorder into lambda scope via pointer (simple)
        struct RecorderWrapper {
            MicRecorder* rec;
            ~RecorderWrapper(){ if(rec) rec->stop(); }
        } rw{&recorder};

        // Graphics
        sf::RenderWindow window(sf::VideoMode(1280,720), "Mini Oscilloscope (SFML)");
        window.setVerticalSyncEnabled(true);

        sf::Font font;
#ifdef _WIN32
        font.loadFromFile("C:/Windows/Fonts/consola.ttf");
#else
        font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
#endif

        bool paused=false;
        double rmsValue=0.0;
        auto lastRmsUpdate = std::chrono::steady_clock::now();
        size_t rmsWindowSamples = static_cast<size_t>(recorder.getSampleRate() * 0.05); // 50ms
        const unsigned waveformSamples = window.getSize().x; // show last width samples
        sf::RectangleShape rmsBar;
        rmsBar.setPosition(20.f, window.getSize().y - 200.f);
        rmsBar.setSize({40.f, 0.f});
        rmsBar.setFillColor(sf::Color(100,200,100));

        while (window.isOpen()) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) window.close();
                if (ev.type == sf::Event::KeyPressed) {
                    if (ev.key.code == sf::Keyboard::Escape) window.close();
                    else if (ev.key.code == sf::Keyboard::Space) paused = !paused;
                    else if (ev.key.code == sf::Keyboard::P) {
                        sf::Image img = window.capture();
                        std::string name = "capture_" + timestamp() + ".png";
                        if (img.saveToFile(name))
                            std::cout << "[INFO] Saved screenshot: " << name << "\n";
                        else
                            std::cout << "[ERROR] Screenshot save failed.\n";
                    }
                }
            }

            // RMS update every ~50ms
            auto now = std::chrono::steady_clock::now();
            if (now - lastRmsUpdate > std::chrono::milliseconds(50)) {
                std::vector<float> recent;
                ring.getRecent(rmsWindowSamples, recent);
                double sumSq=0.0;
                for (float v: recent) sumSq += v*v;
                if (!recent.empty()) rmsValue = std::sqrt(sumSq / recent.size());
                lastRmsUpdate = now;
            }

            window.clear(sf::Color(20,20,30));

            // Waveform
            if (!paused) {
                std::vector<float> wf;
                ring.getRecent(waveformSamples, wf);
                sf::VertexArray va(sf::LineStrip, wf.size());
                float midY = window.getSize().y * 0.45f;
                float scaleY = window.getSize().y * 0.4f;
                for (size_t i=0;i<wf.size();++i) {
                    float x = static_cast<float>(i);
                    float y = midY - wf[i]*scaleY;
                    va[i].position = {x,y};
                    va[i].color = sf::Color::Cyan;
                }
                window.draw(va);
            } else {
                // Still draw last waveform but maybe dim
                std::vector<float> wf;
                ring.getRecent(waveformSamples, wf);
                sf::VertexArray va(sf::LineStrip, wf.size());
                float midY = window.getSize().y * 0.45f;
                float scaleY = window.getSize().y * 0.4f;
                for (size_t i=0;i<wf.size();++i) {
                    float x = static_cast<float>(i);
                    float y = midY - wf[i]*scaleY;
                    va[i].position = {x,y};
                    va[i].color = sf::Color(80,80,80);
                }
                window.draw(va);
            }

            // RMS bar
            float rmsHeight = static_cast<float>(rmsValue) * 180.f;
            rmsBar.setSize({40.f, rmsHeight});
            rmsBar.setPosition(20.f, window.getSize().y - 20.f - rmsHeight);
            window.draw(rmsBar);

            // Text overlay
            if (font.getInfo().family.size()) {
                sf::Text text;
                text.setFont(font);
                text.setCharacterSize(16);
                std::ostringstream oss;
                double latencyMs = recorder.lastChunkSize.load() * 1000.0 / recorder.getSampleRate();
                oss << "Source: Microphone\n"
                    << "Device: " << recorder.getDevice() << "\n"
                    << "SampleRate: " << recorder.getSampleRate() << "\n"
                    << "RMS(50ms): " << std::fixed << std::setprecision(3) << rmsValue << "\n"
                    << "Estimated latency: " << std::fixed << std::setprecision(2) << latencyMs << " ms\n"
                    << "Samples buffered: " << ring.available() << "\n"
                    << (paused ? "[SPACE] Resume\n" : "[SPACE] Pause\n")
                    << "[P] Capture PNG   [ESC] Quit\n";
                text.setString(oss.str());
                text.setFillColor(sf::Color::White);
                text.setPosition(10.f,10.f);
                window.draw(text);
            }

            window.display();
        }
        return 0;
    }

    // WAV mode graphics
    sf::RenderWindow window(sf::VideoMode(1280,720), "Mini Oscilloscope (SFML)");
    window.setVerticalSyncEnabled(true);
    sf::Font font;
#ifdef _WIN32
    font.loadFromFile("C:/Windows/Fonts/consola.ttf");
#else
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
#endif
    bool paused=false;
    double rmsValue=0.0;
    auto lastRmsUpdate = std::chrono::steady_clock::now();
    size_t rmsWindowSamples = static_cast<size_t>(wav.sampleRate * 0.05);
    const unsigned waveformSamples = window.getSize().x;
    sf::RectangleShape rmsBar;
    rmsBar.setPosition(20.f, window.getSize().y - 200.f);
    rmsBar.setSize({40.f,0.f});
    rmsBar.setFillColor(sf::Color(200,180,80));

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) window.close();
            if (ev.type == sf::Event::KeyPressed) {
                if (ev.key.code == sf::Keyboard::Escape) window.close();
                else if (ev.key.code == sf::Keyboard::Space) paused = !paused;
                else if (ev.key.code == sf::Keyboard::P) {
                    sf::Image img = window.capture();
                    std::string name = "capture_" + timestamp() + ".png";
                    if (img.saveToFile(name))
                        std::cout << "[INFO] Saved screenshot: " << name << "\n";
                    else
                        std::cout << "[ERROR] Screenshot save failed.\n";
                }
            }
        }

        auto now = std::chrono::steady_clock::now();
        if (now - lastRmsUpdate > std::chrono::milliseconds(50)) {
            std::vector<float> recent;
            ring.getRecent(rmsWindowSamples, recent);
            double sumSq=0.0;
            for (float v: recent) sumSq += v*v;
            if (!recent.empty()) rmsValue = std::sqrt(sumSq / recent.size());
            lastRmsUpdate = now;
        }

        window.clear(sf::Color(25,20,35));

        // Waveform
        std::vector<float> wf;
        ring.getRecent(waveformSamples, wf);
        sf::VertexArray va(sf::LineStrip, wf.size());
        float midY = window.getSize().y * 0.45f;
        float scaleY = window.getSize().y * 0.4f;
        for (size_t i=0;i<wf.size();++i) {
            float x = static_cast<float>(i);
            float y = midY - wf[i]*scaleY;
            va[i].position = {x,y};
            va[i].color = paused ? sf::Color(90,90,90) : sf::Color::Green;
        }
        window.draw(va);

        // RMS bar
        float rmsHeight = static_cast<float>(rmsValue) * 180.f;
        rmsBar.setSize({40.f, rmsHeight});
        rmsBar.setPosition(20.f, window.getSize().y - 20.f - rmsHeight);
        window.draw(rmsBar);

        if (font.getInfo().family.size()) {
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(16);
            std::ostringstream oss;
            double latencyMs = 512 * 1000.0 / wav.sampleRate; // approximate chunk used
            oss << "Source: WAV File\n"
                << "File: " << opt.inputWav << "\n"
                << "SampleRate: " << wav.sampleRate << "\n"
                << "Channels(raw): " << wav.channels << " (display: mono)\n"
                << "RMS(50ms): " << std::fixed << std::setprecision(3) << rmsValue << "\n"
                << "Estimated latency: " << std::fixed << std::setprecision(2) << latencyMs << " ms\n"
                << "Samples buffered: " << ring.available() << "\n"
                << (paused ? "[SPACE] Resume\n" : "[SPACE] Pause\n")
                << "[P] Capture PNG   [ESC] Quit\n";
            text.setString(oss.str());
            text.setFillColor(sf::Color::White);
            text.setPosition(10.f,10.f);
            window.draw(text);
        }

        window.display();
    }

    wavThreadRunning = false;
    if (wavThread.joinable()) wavThread.join();
    return 0;
}