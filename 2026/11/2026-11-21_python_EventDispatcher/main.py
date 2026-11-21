class EventDispatcher:
    def __init__(self):
        self._listeners = {}

    def subscribe(self, event_name: str, callback) -> None:
        listeners = self._listeners.setdefault(event_name, [])
        if not any(existing_callback == callback for existing_callback, _ in listeners):
            listeners.append((callback, False))

    def once(self, event_name: str, callback) -> None:
        listeners = self._listeners.setdefault(event_name, [])
        if not any(existing_callback == callback for existing_callback, _ in listeners):
            listeners.append((callback, True))

    def unsubscribe(self, event_name: str, callback) -> bool:
        listeners = self._listeners.get(event_name)
        if not listeners:
            return False

        for index, (existing_callback, _) in enumerate(listeners):
            if existing_callback == callback:
                listeners.pop(index)
                if not listeners:
                    del self._listeners[event_name]
                return True
        return False

    def emit(self, event_name: str, data=None) -> int:
        listeners = self._listeners.get(event_name)
        if not listeners:
            return 0

        # Emit the snapshot at the start so changes made by callbacks are
        # reflected only by a subsequent emit.
        snapshot = list(listeners)
        executed = 0

        for callback, run_once in snapshot:
            callback(data)
            executed += 1

            if run_once:
                current = self._listeners.get(event_name)
                if current:
                    for index, (existing_callback, _) in enumerate(current):
                        if existing_callback == callback:
                            current.pop(index)
                            if not current:
                                del self._listeners[event_name]
                            break

        return executed

    def listener_count(self, event_name: str) -> int:
        listeners = self._listeners.get(event_name)
        return len(listeners) if listeners else 0

def on_temperature(data):
    print(f"Temperature: {data}")

def on_warning(data):
    if data > 80:
        print("WARNING!")

dispatcher = EventDispatcher()

dispatcher.subscribe("sensor", on_temperature)
dispatcher.subscribe("sensor", on_warning)

print(dispatcher.listener_count("sensor"))
# 2

print(dispatcher.emit("sensor", 85))
# Temperature: 85
# WARNING!
# 2

dispatcher.unsubscribe("sensor", on_warning)

print(dispatcher.emit("sensor", 60))
# Temperature: 60
# 1