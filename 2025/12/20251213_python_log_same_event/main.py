def detect_consecutive_events(filename):
    try:
        with open(filename, 'r') as f:
            lines = [line.strip() for line in f if line.strip()]
    except FileNotFoundError:
        print(f"No File !!!")
        return
    
    if not lines:
        return
    
    current_event = lines[0]
    count = 1
    seen = set()
    
    for i in range(1, len(lines)):
        if lines[i] == current_event:
            count += 1
        else:
            if count >= 3 and current_event not in seen:
                print(f"{current_event} {count}")
                seen.add(current_event)
            current_event = lines[i]
            count = 1
    
    if count >= 3 and current_event not in seen:
        print(f"{current_event} {count}")


if __name__ == "__main__":
    detect_consecutive_events("events.log")