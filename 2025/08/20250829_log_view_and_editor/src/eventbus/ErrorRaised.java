package eventbus;

/** エラー通知イベント：ユーザーに見せる短いメッセージ想定 */
public record ErrorRaised(String message) implements AppEvent {}
