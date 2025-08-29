package view;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.nio.file.Paths;
import java.util.List;
import javax.swing.*;

import controller.Controller;
import eventbus.*;

/**
 * JFrame ルート。内部で LogPanel / ToolBar / StatusBar を構成し、
 * EventBus を購読して描画を更新する。
 */
public class LogFrame extends JFrame {
    private final Controller controller;
    private final EventBus bus;

    private final LogPanel logPanel;
    private final ToolBar toolBar;
    private final StatusBar statusBar;

    private AutoCloseable subLines, subError;

    public LogFrame(Controller controller) {
        super("ログビューア");
        this.controller = controller;
        this.bus = controller.getEventBus();

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(1000, 700);
        setLocationByPlatform(true);
        setLayout(new BorderLayout());

        // 中央パネル
        logPanel = new LogPanel();
        add(logPanel, BorderLayout.CENTER);

        // ツールバー（Controller を渡す）
        toolBar = new ToolBar(controller);
        add(toolBar, BorderLayout.NORTH);

        // ステータスバー
        statusBar = new StatusBar();
        add(statusBar, BorderLayout.SOUTH);

        // メニューバー
        setJMenuBar(new AppMenuBar(controller, this));

        // EventBus 購読（LinesUpdated / ErrorRaised）
        subLines = bus.subscribe(LinesUpdated.class, evt ->
            SwingUtilities.invokeLater(() -> {
                logPanel.setLines(evt.lines());
                setTitle(String.format("ログビューア  (total=%d, matched=%d)", evt.total(), evt.matched()));
                statusBar.setStatus(true, new int[]{0, 0}, currentMemoryBytes());
            })
        );
        subError = bus.subscribe(ErrorRaised.class, evt ->
            SwingUtilities.invokeLater(() -> {
                statusBar.setMessage("エラー: " + evt.message());
                JOptionPane.showMessageDialog(this, evt.message(), "エラー", JOptionPane.ERROR_MESSAGE);
            })
        );
    }

    private int currentMemoryBytes() {
        long used = Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory();
        return (int)Math.min(Integer.MAX_VALUE, used);
    }

    @Override
    public void dispose() {
        try { if (subLines != null) subLines.close(); } catch (Exception ignore) {}
        try { if (subError != null) subError.close(); } catch (Exception ignore) {}
        super.dispose();
    }

    // ====== 中央テキストエリア ======
    static class LogPanel extends JPanel {
        private final JTextArea textArea = new JTextArea();

        LogPanel() {
            setLayout(new BorderLayout());
            textArea.setEditable(false);
            textArea.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 13));
            add(new JScrollPane(textArea), BorderLayout.CENTER);
        }

        void setLines(List<String> lines) {
            var sb = new StringBuilder(Math.max(1024, lines.size() * 32));
            for (String line : lines) {
                sb.append(line).append('\n');
            }
            textArea.setText(sb.toString());
            textArea.setCaretPosition(0);
        }

        JTextArea getTextArea() { return textArea; }
    }

    // ====== ツールバー ======
    static class ToolBar extends JToolBar {
        private final JTextField searchField = new JTextField(24);
        private final JCheckBox regexCheck = new JCheckBox("正規表現", true);

        ToolBar(Controller controller) {
            JButton openBtn = new JButton("開く");
            JButton saveBtn = new JButton("保存(表示中→同名)");
            JButton exportBtn = new JButton("エクスポート...");
            JButton clearBtn = new JButton("解除");
            JButton searchBtn = new JButton("検索");

            add(openBtn);
            add(saveBtn);
            add(exportBtn);
            addSeparator();
            add(new JLabel("検索: "));
            add(searchField);
            add(searchBtn);
            add(regexCheck);
            add(clearBtn);

            openBtn.addActionListener(e -> {
                JFileChooser fc = new JFileChooser();
                if (fc.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
                    controller.openFile(fc.getSelectedFile().getAbsolutePath());
                }
            });

            saveBtn.addActionListener(e -> {
                controller.saveViewToSameFile();
            });


            exportBtn.addActionListener(e -> {
                JFileChooser fc = new JFileChooser();
                if (fc.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
                    controller.exportToFile(fc.getSelectedFile().getAbsolutePath());
                }
            });

            searchBtn.addActionListener(e -> {
                controller.applyFilter(searchField.getText(), regexCheck.isSelected());
            });

            clearBtn.addActionListener(e -> {
                searchField.setText("");
                controller.applyFilter("", regexCheck.isSelected());
            });
        }
    }

    // ====== メニューバー ======
    static class AppMenuBar extends JMenuBar {
        AppMenuBar(Controller controller, Component parent) {
            JMenu file = new JMenu("ファイル");
            JMenuItem open = new JMenuItem("開く...");
            JMenuItem export = new JMenuItem("エクスポート...");
            JMenuItem exit = new JMenuItem("終了");

            file.add(open);
            file.add(export);
            file.addSeparator();
            file.add(exit);

            JMenu view = new JMenu("表示");
            JCheckBoxMenuItem wrap = new JCheckBoxMenuItem("折り返し", false);
            view.add(wrap);

            JMenu help = new JMenu("ヘルプ");
            JMenuItem about = new JMenuItem("バージョン情報");
            help.add(about);

            add(file); add(view); add(help);

            open.addActionListener(e -> {
                JFileChooser fc = new JFileChooser();
                if (fc.showOpenDialog(parent) == JFileChooser.APPROVE_OPTION) {
                    controller.openFile(fc.getSelectedFile().getAbsolutePath());
                }
            });

            export.addActionListener(e -> {
                JFileChooser fc = new JFileChooser();
                if (fc.showSaveDialog(parent) == JFileChooser.APPROVE_OPTION) {
                    controller.exportToFile(fc.getSelectedFile().getAbsolutePath());
                }
            });

            exit.addActionListener((ActionEvent e) -> {
                Window w = SwingUtilities.getWindowAncestor(parent);
                if (w != null) w.dispose();
                System.exit(0);
            });

            about.addActionListener(e ->
                JOptionPane.showMessageDialog(parent,
                    "ログビューア v0.1\nOneDayOneCode / JFrame 練習",
                    "バージョン情報",
                    JOptionPane.INFORMATION_MESSAGE)
            );

            wrap.addActionListener(e -> {
                // 折り返しは LogPanel の JTextArea に適用したい場合、
                // 親階層を辿って JTextArea を取得して設定するなど（今回は簡易に通知だけ）
                JOptionPane.showMessageDialog(parent, "折り返しの実装は任意課題です。");
            });
        }
    }

    // ====== ステータスバー ======
    static class StatusBar extends JPanel {
        private final JLabel statusLabel;

        StatusBar() {
            setLayout(new BorderLayout());
            statusLabel = new JLabel("準備完了 | 行: 0 | 列: 0 | 使用メモリ: 0B");
            add(statusLabel, BorderLayout.WEST);
        }

        void setStatus(boolean isLoadComplete, int[] cursorPosition, int usingMemoryBytes) {
            String message = isLoadComplete ? "読み込み完了" : "読み込み中...";
            statusLabel.setText(message + " | 行: " + cursorPosition[0] + " | 列: " + cursorPosition[1]
                    + " | 使用メモリ: " + usingMemoryBytes + "B");
        }

        void setMessage(String msg) {
            statusLabel.setText(msg);
        }
    }
}
