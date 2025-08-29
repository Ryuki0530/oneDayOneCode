package eventbus;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.function.Consumer;

/** スレッドセーフな最小実装。型ごとに購読者を保持。 */
public class SimpleEventBus implements EventBus {
    private final ConcurrentHashMap<Class<?>, CopyOnWriteArrayList<Consumer<?>>> handlers = new ConcurrentHashMap<>();

    @Override
    public <T extends AppEvent> AutoCloseable subscribe(Class<T> type, Consumer<T> handler) {
        var list = handlers.computeIfAbsent(type, k -> new CopyOnWriteArrayList<>());
        list.add(handler);
        return () -> list.remove(handler); // 購読解除
    }

    @SuppressWarnings("unchecked")
    @Override
    public void publish(AppEvent event) {
        var list = handlers.get(event.getClass());
        if (list == null) return;
        for (var h : list) {
            ((Consumer<AppEvent>) h).accept(event);
        }
    }
}
