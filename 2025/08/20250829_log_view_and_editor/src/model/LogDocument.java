// model/LogDocument.java
package model;

import io.FileLoader;
import eventbus.*;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.List;
import java.util.ArrayList;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;

public class LogDocument {
    private final EventBus bus;

    /** 読み込み済みの全行 */
    private List<String> all = List.of();
    /** フィルタ後の表示用行（all と同一参照のこともある） */
    private List<String> view = List.of();

    private Path currentPath; // 現在のファイルパス

    public LogDocument(EventBus bus) {
        this.bus = bus;
    }

    /**
     * ファイルを読み込む（同期）。読み込み後に LinesUpdated を publish。
     * 失敗時は ErrorRaised を publish。
     * @return 0 = OK, 非0 = エラー
     */
    public int loadFile(String filePath) {
        try {
            this.currentPath = Path.of(filePath);
            FileLoader fileLoader = new FileLoader(filePath);
            // FileLoader#loadLines() が String[] を返す前提
            this.all = Arrays.asList(fileLoader.loadLines());
            this.view = this.all;

            // ログ（任意）
            System.out.println("Model loaded " + all.size() + " lines from " + filePath);

            // 成功イベント
            publishLinesUpdated();
            return 0;
        } catch (Exception e) {
            bus.publish(new ErrorRaised("読み込み失敗: " + e.getMessage()));
            return 1;
        }
    }

    /**
     * フィルタ適用。pattern が null/空ならフィルタ解除と同等。
     * 正規表現が不正な場合は ErrorRaised を publish（view は変更しない）。
     */
    public void applyFilter(String pattern, boolean regex) {
        if (pattern == null || pattern.isEmpty()) {
            clearFilter();
            return;
        }
        try {
            List<String> filtered = new ArrayList<>(Math.max(16, all.size()));
            if (regex) {
                Pattern p = Pattern.compile(pattern);
                for (String s : all) {
                    if (p.matcher(s).find()) {
                        filtered.add(s);
                    }
                }
            } else {
                for (String s : all) {
                    if (s.contains(pattern)) {
                        filtered.add(s);
                    }
                }
            }
            this.view = filtered;
            publishLinesUpdated();
        } catch (PatternSyntaxException ex) {
            bus.publish(new ErrorRaised("正規表現が不正です: " + ex.getDescription()));
        } catch (Exception ex) {
            bus.publish(new ErrorRaised("フィルタ処理でエラー: " + ex.getMessage()));
        }
    }

    /** フィルタ解除（全行表示） */
    public void clearFilter() {
        this.view = this.all;
        publishLinesUpdated();
    }

    /** 現在の表示内容を標準出力へ出力（--export stdout 用） */
    public void exportToStdout() {
        try {
            for (String line : view) {
                System.out.println(line);
            }
        } catch (Exception e) {
            bus.publish(new ErrorRaised("標準出力への書き出しに失敗: " + e.getMessage()));
        }
    }

    /** 現在の表示内容を指定パスに UTF-8 で保存 */
    public void exportToFile(Path outPath) {
        try (var w = Files.newBufferedWriter(outPath, StandardCharsets.UTF_8)) {
            for (String line : view) {
                w.write(line);
                w.newLine();
            }
        } catch (Exception e) {
            bus.publish(new ErrorRaised("エクスポート失敗: " + e.getMessage()));
        }
    }

    /** 現在のファイルに「全行」を上書き保存（注意：元データを壊し得ます） */
    public void saveAllToSameFile() {
        if (currentPath == null) {
            bus.publish(new ErrorRaised("保存できません（ファイル未読です）"));
            return;
        }
        try (var w = Files.newBufferedWriter(currentPath, StandardCharsets.UTF_8)) {
            for (String line : all) {
                w.write(line);
                w.newLine();
            }
        } catch (Exception e) {
            bus.publish(new ErrorRaised("上書き保存（全行）に失敗: " + e.getMessage()));
        }
    }

    /** 現在のファイルに「表示中の行（フィルタ後）」を上書き保存（注意） */
    public void saveViewToSameFile() {
        if (currentPath == null) {
            bus.publish(new ErrorRaised("保存できません（ファイル未読です）"));
            return;
        }
        try (var w = Files.newBufferedWriter(currentPath, StandardCharsets.UTF_8)) {
            for (String line : view) {
                w.write(line);
                w.newLine();
            }
        } catch (Exception e) {
            bus.publish(new ErrorRaised("上書き保存（表示中）に失敗: " + e.getMessage()));
        }
    }

    /** 現在表示中の行を取得（View があと追いで描画したい時に使用） */
    public List<String> getCurrentLines() { return view; }

    /** 全行を取得 */
    public List<String> getAllLines() { return all; }

    /** 現在のファイルパスを取得 */
    public Path getCurrentPath() { return currentPath; }

    // --- 内部ユーティリティ ---

    private void publishLinesUpdated() {
        bus.publish(new LinesUpdated(view, all.size(), view.size()));
    }
}
