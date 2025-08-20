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
    private Clip clip;
    private long pausePosition = 0;
    private boolean paused = false;
    private boolean stopped = false;
    private boolean debug;
    private String filePath;

    SoundPlayer(String filePath, boolean debug) {
        this.filePath = filePath;
        this.debug = debug;
        if (debug) {
            System.out.println("SoundPlayer initialized. Debug mode is ON.");
        }
    }

    // 再生開始
    public boolean SoundStart() {
        if (filePath == null) {
            if (debug) System.out.println("No file selected.");
            return false;
        }
        try {
            if (clip != null && clip.isOpen()) {
                clip.close();
            }
            AudioInputStream ais = AudioSystem.getAudioInputStream(new File(filePath));
            clip = AudioSystem.getClip();
            clip.open(ais);

            clip.addLineListener(event -> {
                if (event.getType() == LineEvent.Type.STOP && !paused && !stopped) {
                    if (debug) System.out.println("Playback finished.");
                }
            });

            pausePosition = 0;
            stopped = false;
            paused = false;
            clip.setMicrosecondPosition(0);
            clip.start();
            if (debug) System.out.println("Playback started.");
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    // 一時停止
    public void pause() {
        if (clip != null && clip.isRunning()) {
            pausePosition = clip.getMicrosecondPosition();
            clip.stop();
            paused = true;
            if (debug) System.out.println("Paused at " + pausePosition + " µs");
        }
    }

    // 再開
    public void resume() {
        if (clip != null && paused) {
            clip.setMicrosecondPosition(pausePosition);
            clip.start();
            paused = false;
            if (debug) System.out.println("Resumed from " + pausePosition + " µs");
        }
    }

    // 停止
    public void stop() {
        if (clip != null) {
            stopped = true;
            clip.stop();
            clip.setMicrosecondPosition(0);
            pausePosition = 0;
            paused = false;
            if (debug) System.out.println("Playback stopped.");
        }
    }
}