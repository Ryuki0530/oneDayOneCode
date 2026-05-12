#include <array>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <sys/wait.h>

#include "main.h"

static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \n\r\t");
    if (start == std::string::npos) return "";

    size_t end = s.find_last_not_of(" \n\r\t");
    return s.substr(start, end - start + 1);
}

static bool startsWith(const std::string& s, const std::string& prefix) {
    return s.rfind(prefix, 0) == 0;
}

static bool endsWith(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size()
        && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

static int parseInterval(const std::string& s) {
    try {
        size_t pos = 0;
        int value = std::stoi(s, &pos);

        if (pos != s.size()) {
            throw std::runtime_error("invalid interval-sec");
        }

        return value;
    } catch (...) {
        throw std::runtime_error("invalid interval-sec");
    }
}
    
class Logger {
public:
    explicit Logger(const std::string& logPath = "/tmp/oodc-watchdog.log")
        : logPath_(logPath) {}

    void info(const std::string& msg) { log("[INFO] " + msg); }
    void warn(const std::string& msg) { log("[WARN] " + msg); }
    void check(const std::string& msg) { log("[CHECK] " + msg); }
    void action(const std::string& msg) { log("[ACTION] " + msg); }
    void result(const std::string& msg) { log("[RESULT] " + msg); }
    void error(const std::string& msg) { log("[ERROR] " + msg); }

private:
    std::string logPath_;

    void log(const std::string& msg) {
        std::cout << msg << std::endl;

        std::ofstream ofs(logPath_, std::ios::app);
        if (ofs.is_open()) {
            ofs << msg << std::endl;
        }
    }
};
 
class CommandRunner {
public:
    struct CommandResult {
        int exitCode{-1};
        std::string output;
    };

    static CommandResult run(const std::string& command) {
        std::string fullCommand = command + " 2>&1";

        std::array<char, 256> buffer{};
        std::string output;

        FILE* pipe = popen(fullCommand.c_str(), "r");
        if (!pipe) {
            throw std::runtime_error("failed to execute command: " + command);
        }

        while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
            output += buffer.data();
        }

        int status = pclose(pipe);

        int exitCode = -1;
        if (status != -1) {
            if (WIFEXITED(status)) {
                exitCode = WEXITSTATUS(status);
            } else {
                exitCode = status;
            }
        }

        return {exitCode, trim(output)};
    }
};

class SystemdService {
public:
    enum class State {
        Active,
        Inactive,
        Failed,
        Unknown
    };

    explicit SystemdService(std::string name)
        : name_(std::move(name)) {}

    const std::string& name() const {
        return name_;
    }

    State queryState() const {
        auto result = CommandRunner::run("systemctl is-active " + name_);

        if (result.output == "active") {
            return State::Active;
        }
        if (result.output == "inactive") {
            return State::Inactive;
        }
        if (result.output == "failed") {
            return State::Failed;
        }

        return State::Unknown;
    }

    bool restart() const {
        auto result = CommandRunner::run("systemctl restart " + name_);
        return result.exitCode == 0;
    }

private:
    std::string name_;
};


class WatchdogApplication {
public:
    int run(int argc, char* argv[]) {
        try {
            parseArgs(argc, argv);
            printHeader();
            printDefaultTarget();
            execute();

            logger_.info("watchdog finished");
            return 0;
        } catch (const std::exception& e) {
            logger_.error(e.what());
            return 1;
        }
    }

private:
    static constexpr int kCheckCount = 10;

    Logger logger_;
    std::string serviceName_;
    int intervalSec_ = 0;
    bool dryRun_ = false;

    void parseArgs(int argc, char* argv[]) {
        if (argc < 3) {
            throw std::runtime_error("usage: ./watchdog [--dry-run] <service-name> <interval-sec>");
        }

        int index = 1;

        // 修正: --dry-run を先頭に指定できるようにする
        if (std::string(argv[index]) == "--dry-run") {
            dryRun_ = true;
            ++index;
        }

        if (argc - index < 2) {
            throw std::runtime_error("usage: ./watchdog [--dry-run] <service-name> <interval-sec>");
        }

        serviceName_ = argv[index++];
        intervalSec_ = parseInterval(argv[index]);

        if (intervalSec_ < 1) {
            throw std::runtime_error("interval-sec must be at least 1");
        }

        if (!endsWith(serviceName_, ".service")) {
            throw std::runtime_error("service-name must end with .service");
        }
    }

    void printHeader() {
        logger_.info("target service: " + serviceName_);
        logger_.info("interval: " + std::to_string(intervalSec_) + " sec");

        if (dryRun_) {
            logger_.info("dry-run: enabled");
        }
    }

    void printDefaultTarget() {
        auto result = CommandRunner::run("systemctl get-default");

        if (result.exitCode == 0) {
            logger_.info("default target: " + result.output);
        } else {
            logger_.warn("failed to get default target: " + result.output);
        }
    }

    void execute() {
        SystemdService service(serviceName_);

        for (int i = 1; i <= kCheckCount; ++i) {
            logger_.info("check count: " + std::to_string(i) + "/" + std::to_string(kCheckCount));

            auto state = service.queryState();
            handleState(service, state);

            if (i < kCheckCount) {
                std::this_thread::sleep_for(std::chrono::seconds(intervalSec_));
            }
        }
    }

    void handleState(const SystemdService& service, SystemdService::State state) {
        switch (state) {
            case SystemdService::State::Active:
                logger_.check(service.name() + " is active");
                break;

            case SystemdService::State::Inactive:
                logger_.check(service.name() + " is inactive");
                restartIfAllowed(service);
                break;

            case SystemdService::State::Failed:
                logger_.check(service.name() + " is failed");
                restartIfAllowed(service);
                break;

            case SystemdService::State::Unknown:
                logger_.check(service.name() + " is unknown");
                break;
        }
    }

    void restartIfAllowed(const SystemdService& service) {
        if (!startsWith(service.name(), "oodc-")) {
            logger_.warn("restart skipped: service name must start with oodc-");
            return;
        }

        logger_.action("restart " + service.name());

        if (dryRun_) {
            logger_.result("restart skipped (dry-run)");
            return;
        }

        bool ok = service.restart();

        // 修正: restart の成否を確認する
        if (ok) {
            logger_.result("restart success");
        } else {
            logger_.result("restart failed");
        }
    }
};


int main(int argc, char* argv[]) {
    WatchdogApplication app;
    return app.run(argc, argv);
}