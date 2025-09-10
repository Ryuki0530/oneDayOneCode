

package ui;

import core.CmdRunner;
import core.ProcessFetcher;

import javax.swing.*;
import javax.swing.table.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.datatransfer.StringSelection;
import java.util.Comparator;
import java.util.List;
import java.util.concurrent.*;

public class MainFrame extends JFrame {
    private final JTextField searchField = new JTextField();
    private final JButton refreshButton = new JButton("更新");
    private final JButton killButton = new JButton("選択を終了");
    private final ProcessTableModel tableModel = new ProcessTableModel();
    private final JTable table = new JTable(tableModel);
    private final TableRowSorter<ProcessTableModel> sorter = new TableRowSorter<>(tableModel);
    private final ExecutorService executor = Executors.newSingleThreadExecutor();

    public MainFrame() {
        setTitle("WinTaskLite - 極小タスクマネージャ");
        setSize(700, 500);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        // 検索ボックス
        searchField.setToolTipText("名前 or PID を入力");
        searchField.setPreferredSize(new Dimension(200, 28));

        // JTable
        table.setRowSorter(sorter);
        table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        table.setAutoCreateRowSorter(false);
        sorter.setComparator(2, Comparator.comparingLong(o -> (Long)o)); // Mem Usage数値ソート

        JScrollPane scrollPane = new JScrollPane(table);

        // ボタンパネル
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new FlowLayout(FlowLayout.RIGHT));
        buttonPanel.add(refreshButton);
        buttonPanel.add(killButton);

        // レイアウト
        JPanel topPanel = new JPanel(new BorderLayout());
        topPanel.add(searchField, BorderLayout.CENTER);

        setLayout(new BorderLayout(8, 8));
        add(topPanel, BorderLayout.NORTH);
        add(scrollPane, BorderLayout.CENTER);
        add(buttonPanel, BorderLayout.SOUTH);

        // イベント
        refreshButton.addActionListener(e -> reloadProcessList());
        killButton.addActionListener(e -> killSelectedProcess());
        searchField.getDocument().addDocumentListener(new DocumentListener() {
            public void insertUpdate(DocumentEvent e) { filter(); }
            public void removeUpdate(DocumentEvent e) { filter(); }
            public void changedUpdate(DocumentEvent e) { filter(); }
        });
        // Ctrl+Fで検索ボックスにフォーカス
        getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
            .put(KeyStroke.getKeyStroke(KeyEvent.VK_F, InputEvent.CTRL_DOWN_MASK), "focusSearch");
        getRootPane().getActionMap().put("focusSearch", new AbstractAction() {
            public void actionPerformed(ActionEvent e) { searchField.requestFocusInWindow(); }
        });
        // Enterで即フィルタ、Escでクリア
        searchField.addKeyListener(new KeyAdapter() {
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
                    searchField.setText("");
                }
            }
        });
        // PIDダブルクリックでクリップボードコピー
        table.addMouseListener(new MouseAdapter() {
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 2) {
                    int row = table.getSelectedRow();
                    if (row >= 0) {
                        int modelRow = table.convertRowIndexToModel(row);
                        ProcessFetcher.ProcessInfo p = tableModel.getProcessAt(modelRow);
                        StringSelection sel = new StringSelection(String.valueOf(p.pid));
                        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(sel, sel);
                        JOptionPane.showMessageDialog(MainFrame.this, "PID " + p.pid + " をコピーしました。", "コピー", JOptionPane.INFORMATION_MESSAGE);
                    }
                }
            }
        });

        // 初期ロード
        reloadProcessList();
    }

    private void filter() {
        tableModel.setFilter(searchField.getText());
    }

    private void reloadProcessList() {
        refreshButton.setEnabled(false);
        executor.submit(() -> {
            try {
                List<ProcessFetcher.ProcessInfo> list = ProcessFetcher.fetchProcesses();
                SwingUtilities.invokeLater(() -> {
                    tableModel.setProcesses(list);
                    refreshButton.setEnabled(true);
                });
            } catch (Exception ex) {
                SwingUtilities.invokeLater(() -> {
                    refreshButton.setEnabled(true);
                    JOptionPane.showMessageDialog(this, "プロセス一覧の取得に失敗しました:\n" + ex.getMessage(), "エラー", JOptionPane.ERROR_MESSAGE);
                });
            }
        });
    }

    private void killSelectedProcess() {
        int viewRow = table.getSelectedRow();
        if (viewRow < 0) {
            JOptionPane.showMessageDialog(this, "プロセスを選択してください。", "警告", JOptionPane.WARNING_MESSAGE);
            return;
        }
        int modelRow = table.convertRowIndexToModel(viewRow);
        ProcessFetcher.ProcessInfo p = tableModel.getProcessAt(modelRow);
        int res = JOptionPane.showConfirmDialog(this, "PID " + p.pid + " (" + p.imageName + ") を終了しますか？", "確認", JOptionPane.YES_NO_OPTION);
        if (res != JOptionPane.YES_OPTION) return;

        killButton.setEnabled(false);
        executor.submit(() -> {
            try {
                int exit = CmdRunner.runAndGetExitCode("cmd", "/c", "taskkill /PID " + p.pid + " /F");
                SwingUtilities.invokeLater(() -> {
                    killButton.setEnabled(true);
                    if (exit == 0) {
                        JOptionPane.showMessageDialog(this, "プロセス(PID " + p.pid + ")を終了しました。", "成功", JOptionPane.INFORMATION_MESSAGE);
                        reloadProcessList();
                    } else {
                        JOptionPane.showMessageDialog(this, "終了に失敗しました (アクセス拒否/既に終了等)", "エラー", JOptionPane.ERROR_MESSAGE);
                    }
                });
            } catch (Exception ex) {
                SwingUtilities.invokeLater(() -> {
                    killButton.setEnabled(true);
                    JOptionPane.showMessageDialog(this, "プロセス終了に失敗:\n" + ex.getMessage(), "エラー", JOptionPane.ERROR_MESSAGE);
                });
            }
        });
    }
}
