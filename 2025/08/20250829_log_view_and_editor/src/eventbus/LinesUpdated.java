package eventbus;

import java.util.List;

/** 行更新イベント：表示用の行と件数情報を通知 */
public record LinesUpdated(List<String> lines, int total, int matched) implements AppEvent {}
