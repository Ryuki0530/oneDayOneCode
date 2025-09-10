package ui;
import javax.swing.JFrame;
import javax.swing.JPanel;
import core.CmdRunner;
import core.ProcessFetcher;

public class MainFrame extends JFrame {
    private TaskManager taskManager;

    public MainFrame() {
        // Initialize the main frame
        setTitle("Task Manager");
        setSize(800, 600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        JPanel panel = new JPanel();
        add(panel);
    }
}
