package eventbus;

import java.util.function.Consumer;

/**
 * シンプルな型安全イベントバス。
 * 返り値の AutoCloseable は購読解除用ハンドル（View破棄時に close 推奨）。
 */
public interface EventBus {
    <T extends AppEvent> AutoCloseable subscribe(Class<T> type, Consumer<T> handler);
    void publish(AppEvent event);
}
