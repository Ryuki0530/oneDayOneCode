import javax.swing.*;
import java.awt.*;
import java.awt.event.*;


public class Main {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new Main().createAndShowGUI());
    }

    private boolean started = false;
    private long startTime = 0L;
    private long lastTime = 0L;
    private int lapCount = 0;

    private JLabel label;
    private JButton startLapButton;
    private JButton resetButton;

    private void createAndShowGUI() {
        JFrame frame = new JFrame("Click lap timer");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new FlowLayout());

        label = new JLabel("Ready");
        label.setFont(label.getFont().deriveFont(Font.BOLD, 24f));

        startLapButton = new JButton("START");
        resetButton = new JButton("RESET");
        resetButton.setEnabled(false);

        startLapButton.addActionListener(e -> onStartLap());
        resetButton.addActionListener(e -> onReset());

        frame.add(label);
        frame.add(startLapButton);
        frame.add(resetButton);

        frame.setSize(320, 120);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }

    private void onStartLap() {
        long now = System.currentTimeMillis();
        if (!started) {
            started = true;
            startTime = now;
            lastTime = now;
            lapCount = 0;
            label.setText("Started!");
            startLapButton.setText("LAP");
            resetButton.setEnabled(true);
        } else {
            lapCount++;
            long lap = now - lastTime;
            label.setText(String.format("LAP #%d: %d ms", lapCount, lap));
            lastTime = now;
        }
    }

    private void onReset() {
        started = false;
        startTime = 0L;
        lastTime = 0L;
        lapCount = 0;
        label.setText("Ready");
        startLapButton.setText("START");
        resetButton.setEnabled(false);
    }
}