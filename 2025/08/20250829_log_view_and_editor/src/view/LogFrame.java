package view;

import java.awt.*;
import javax.swing.*;
import controller.Controller;
import eventbus.*;
public class LogFrame extends JFrame {

    private Controller controller;

    public LogFrame(Controller controller) {

        super("ログビューア");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(800, 600);
        this.controller = controller;
        setLayout(new BorderLayout());

        add(new LogPanel(controller.getEventBus()), BorderLayout.CENTER);
        add(new ToolBar(), BorderLayout.NORTH);
        add(new StatusBar(), BorderLayout.SOUTH);

        setJMenuBar(new MenuBar());


        
    }
}

class LogPanel extends JPanel {
    private final JTextArea textArea = new JTextArea();

    public LogPanel(EventBus bus) {
        setLayout(new BorderLayout());
        textArea.setEditable(false);
        add(new JScrollPane(textArea), BorderLayout.CENTER);

        // EventBus購読
        bus.subscribe(LinesUpdated.class, evt -> 
            SwingUtilities.invokeLater(() -> {
                var sb = new StringBuilder();
                for (String line : evt.lines()) {
                    sb.append(line).append("\n");
                }
                textArea.setText(sb.toString());
            })
        );
    }
}

class ToolBar extends JToolBar {
    public ToolBar() {
        JButton openButton = new JButton("開く");
        JButton saveButton = new JButton("保存");
        JButton editButton = new JButton("編集");
        JTextField searchField = new JTextField(20);
        JButton searchButton = new JButton("検索");
        JCheckBox regexCheckBox = new JCheckBox("正規表現");

        add(openButton);
        add(saveButton);
        add(editButton);
        add(searchField);
        add(searchButton);
        add(regexCheckBox);
    }
}

class MenuBar extends JMenuBar {
    public MenuBar() {
        JMenu fileMenu = new JMenu("ファイル");
        JMenuItem openItem = new JMenuItem("開く");
        JMenuItem overwriteSaveItem = new JMenuItem("上書き保存");
        JMenuItem saveItem = new JMenuItem("名前を付けて保存");
        JMenuItem exitItem = new JMenuItem("終了");
        fileMenu.add(openItem);
        fileMenu.add(saveItem);
        fileMenu.add(overwriteSaveItem);
        fileMenu.add(exitItem);

        JMenu viewMenu = new JMenu("表示");
        JMenuItem logViewItem = new JMenuItem("行番号の表示切替");
        JMenuItem editorViewItem = new JMenuItem("折り返し切替");
        // viewMenu.add(logViewItem);
        // viewMenu.add(editorViewItem);

        JMenu helpMenu = new JMenu("ヘルプ");
        JMenuItem aboutItem = new JMenuItem("バージョン情報");
        helpMenu.add(aboutItem);

        add(fileMenu);
        add(viewMenu);
        add(helpMenu);
   }
}

class StatusBar extends JPanel {
    private final JLabel statusLabel;

    public StatusBar() {
        setLayout(new BorderLayout());
        statusLabel = new JLabel("message | 行: 0 | 列: 0 | 使用メモリ: 0バイト");
        add(statusLabel, BorderLayout.WEST);
    }

    public void setStatus(boolean isLoadComplete, int[] cursorPosition, int usingMemoryBytes) {
        String message = isLoadComplete ? "読み込み完了" : "読み込み中...";
        statusLabel.setText(message + " | 行: " + cursorPosition[0] + " | 列: " + cursorPosition[1] + " | 使用メモリ: " + usingMemoryBytes + "バイト");
    }
}