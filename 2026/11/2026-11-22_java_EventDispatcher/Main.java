import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@FunctionalInterface
interface EventListener {
	void onEvent(String eventType, String message);
}

class EventDispatcher {
	private final Map<String, List<EventListener>> listeners = new HashMap<>();

	public void subscribe(String eventType, EventListener listener) {
		List<EventListener> eventListeners = listeners.computeIfAbsent(
				eventType, key -> new ArrayList<>());

		for (EventListener registered : eventListeners) {
			if (registered == listener) {
				return;
			}
		}
		eventListeners.add(listener);
	}

	public void unsubscribe(String eventType, EventListener listener) {
		List<EventListener> eventListeners = listeners.get(eventType);
		if (eventListeners == null) {
			return;
		}

		for (int i = 0; i < eventListeners.size(); i++) {
			if (eventListeners.get(i) == listener) {
				eventListeners.remove(i);
				return;
			}
		}
	}

	public void publish(String eventType, String message) {
		List<EventListener> eventListeners = listeners.get(eventType);
		if (eventListeners == null) {
			return;
		}

		for (EventListener listener : new ArrayList<>(eventListeners)) {
			listener.onEvent(eventType, message);
		}
	}

	public int listenerCount(String eventType) {
		List<EventListener> eventListeners = listeners.get(eventType);
		return eventListeners == null ? 0 : eventListeners.size();
	}
}

public class Main {
	public static void main(String[] args) {
        EventDispatcher dispatcher = new EventDispatcher();

        EventListener listenerA = (type, message) ->
            System.out.println("A: " + message);

        EventListener listenerB = (type, message) ->
            System.out.println("B: " + message);

        dispatcher.subscribe("ALERT", listenerA);
        dispatcher.subscribe("ALERT", listenerB);
        dispatcher.subscribe("ALERT", listenerA);

        System.out.println(dispatcher.listenerCount("ALERT"));

        dispatcher.publish("ALERT", "Temperature high");

        dispatcher.unsubscribe("ALERT", listenerA);

        dispatcher.publish("ALERT", "Temperature normal");
	}
}
