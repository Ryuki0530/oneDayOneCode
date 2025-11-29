import javax.sound.sampled.*;
import java.io.*;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Scanner;
import java.util.concurrent.atomic.AtomicBoolean;

public class Recorder {

    static final AudioFormat FORMAT = new AudioFormat(
            44100, 16, 1, true, false
    );

    public static void main(String[] args) {
        System.out.println("=== Java CLI Recorder ===");
        try (Scanner sc = new Scanner(System.in)) {
            Mixer.Info[] devicesList = listRecordingDevices();
            Mixer.Info selectedDevice = deviceSelector(devicesList, sc);
            Mixer mixer = AudioSystem.getMixer(selectedDevice);
            TargetDataLine line = openTargetDataLine(mixer);
            System.out.println("\n===STANDBY=== press R to start recording");
            AtomicBoolean recording = new AtomicBoolean(false);
            ByteArrayOutputStream buffer = new ByteArrayOutputStream();

            LocalDateTime startTime = null;

            while (true) {
                String key = sc.nextLine().trim().toUpperCase();
                if ("R".equals(key) && !recording.get()) {
                    buffer.reset();
                    startTime = startRecording(line, buffer, recording);
                    System.out.print("\u001B[A");    // カーソルを1行上へ
                    System.out.print("\u001B[2K");   // その行をクリア
                    System.out.println("===RECORDING=== press S to stop");
                    System.out.flush();
                } else if ("S".equals(key) && recording.get()) {
                    stopAndSave(line, buffer, recording, startTime);
                    break;
                } else {
                    System.out.println("Unknown command or invalid state. Use R to start, S to stop.");
                }
            }

            line.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static Mixer.Info[] listRecordingDevices() {
        Mixer.Info[] mixers = AudioSystem.getMixerInfo();
        for (int i = 0; i < mixers.length; i++) {
            Mixer m = AudioSystem.getMixer(mixers[i]);
            if (m.getTargetLineInfo().length > 0) {
                System.out.println("[" + i + "] " + mixers[i].getName());
            }
        }
        return mixers;
    }

    private static Mixer.Info deviceSelector(Mixer.Info[] mixers, Scanner sc) {
        while (true) {
            System.out.print("Use device number: ");
            String line = sc.nextLine().trim();
            try {
                int idx = Integer.parseInt(line);
                if (0 <= idx && idx < mixers.length) {
                    return mixers[idx];
                }
            } catch (NumberFormatException ignored) {
            }
            System.out.println("Invalid index. Try again.");
        }
    }

    private static TargetDataLine openTargetDataLine(Mixer mixer) throws LineUnavailableException {
        Line.Info[] infos = mixer.getTargetLineInfo();
        if (infos.length == 0) {
            throw new IllegalStateException("No target line available for selected mixer.");
        }
        TargetDataLine line = (TargetDataLine) mixer.getLine(infos[0]);
        line.open(FORMAT);
        return line;
    }

    private static LocalDateTime startRecording(TargetDataLine line, ByteArrayOutputStream buffer, AtomicBoolean recording) {
        recording.set(true);
        line.start();
        Thread t = new Thread(() -> {
            byte[] temp = new byte[4096];
            try {
                while (recording.get()) {
                    int read = line.read(temp, 0, temp.length);
                    if (read > 0) {
                        buffer.write(temp, 0, read);
                    }
                }
            } catch (Exception e) { // 修正: IOException -> Exception
                e.printStackTrace();
            }
        });
        t.setDaemon(true);
        t.start();
        return LocalDateTime.now();
    }

    private static void stopAndSave(TargetDataLine line, ByteArrayOutputStream buffer, AtomicBoolean recording, LocalDateTime startTime) {
        try {
            recording.set(false);
            line.stop();
            LocalDateTime endTime = LocalDateTime.now();
            String fileName = makeFileName(startTime, endTime);
            saveWav(buffer.toByteArray(), fileName);
            System.out.println("Saved: " + fileName);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static String makeFileName(LocalDateTime s, LocalDateTime e) {
        DateTimeFormatter fmt = DateTimeFormatter.ofPattern("yyyyMMdd_HHmmss");
        return "record_" + s.format(fmt) + "_" + e.format(fmt) + ".wav";
    }

    private static void saveWav(byte[] pcm, String fileName) throws IOException {
        try (ByteArrayInputStream bais = new ByteArrayInputStream(pcm);
             AudioInputStream ais = new AudioInputStream(bais, FORMAT, pcm.length / FORMAT.getFrameSize())) {
            AudioSystem.write(ais, AudioFileFormat.Type.WAVE, new File(fileName));
        }
    }
}
