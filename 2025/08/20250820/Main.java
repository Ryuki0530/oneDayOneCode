import javax.sound.sampled.*;
import java.io.File;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class Main {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            JFrame frame = new JFrame("Sound Player");
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setSize(400, 200);
            frame.setLayout(new FlowLayout());

            JLabel fileLabel = new JLabel("ファイル未選択");
            JButton selectFileButton = new JButton("ファイル選択");
            JButton playButton = new JButton("再生");
            JButton pauseButton = new JButton("一時停止");
            JButton resumeButton = new JButton("再開");
            JButton stopButton = new JButton("停止");

            SoundPlayer[] player = new SoundPlayer[1];
            player[0] = new SoundPlayer(null, true);

            selectFileButton.addActionListener(e -> {
                JFileChooser fileChooser = new JFileChooser();
                int result = fileChooser.showOpenDialog(frame);
                if (result == JFileChooser.APPROVE_OPTION) {
                    File selectedFile = fileChooser.getSelectedFile();
                    fileLabel.setText(selectedFile.getName());
                    player[0].stop();
                    player[0] = new SoundPlayer(selectedFile.getAbsolutePath(), true);
                }
            });

            playButton.addActionListener(e -> player[0].SoundStart());
            pauseButton.addActionListener(e -> player[0].pause());
            resumeButton.addActionListener(e -> player[0].resume());
            stopButton.addActionListener(e -> player[0].stop());

            frame.add(selectFileButton);
            frame.add(fileLabel);
            frame.add(playButton);
            frame.add(pauseButton);
            frame.add(resumeButton);
            frame.add(stopButton);

            frame.setVisible(true);
        });
    }
}

class SoundPlayer {

    private String filePath;
    private volatile boolean paused = false;
    private volatile boolean stopped = false;
    private Thread playThread;
    private Clip clip;
    private boolean debug;

    SoundPlayer(String filePath, boolean debug) {
        this.filePath = filePath;
        this.debug = debug;
        if (debug) {
            System.out.println("SoundPlayer initialized. Debug mode is ON.");
        }
    }

    public boolean SoundStart() {
        if (filePath == null) {
            if (debug) System.out.println("No file selected.");
            return false;
        }
        if (playThread != null && playThread.isAlive()) {
            if (debug) System.out.println("Already playing.");
            return false;
        }
        stopped = false;
        paused = false;
        playThread = new Thread(() -> {
            try {
                AudioInputStream audioInputStream = AudioSystem.getAudioInputStream(new File(filePath));
                clip = AudioSystem.getClip();
                clip.open(audioInputStream);
                clip.start();
                if (debug) System.out.println("Playback started.");
                while (!stopped && clip.isActive()) {
                    if (paused) {
                        clip.stop();
                        if (debug) System.out.println("Playback paused.");
                        while (paused && !stopped) {
                            Thread.sleep(100);
                        }
                        if (!stopped) {
                            clip.start();
                            if (debug) System.out.println("Playback resumed.");
                        }
                    }
                    Thread.sleep(100);
                }
                clip.stop();
                clip.close();
                audioInputStream.close();
                if (debug) System.out.println("Playback stopped.");
            } catch (Exception e) {
                e.printStackTrace();
            }
        });
        playThread.start();
        return true;
    }

    public void pause() {
        if (clip != null && clip.isRunning()) {
            paused = true;
        }
    }

    public void resume() {
        if (clip != null && paused) {
            paused = false;
        }
    }

    public void stop() {
        stopped = true;
        paused = false;
        if (clip != null) {
            clip.stop();
            clip.close();
        }
        if (playThread != null && playThread.isAlive()) {
            try {
                playThread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        if (debug) System.out.println("Playback thread stopped.");
    }
}