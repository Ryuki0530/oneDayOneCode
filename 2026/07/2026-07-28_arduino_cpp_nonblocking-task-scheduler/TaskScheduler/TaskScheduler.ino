// Task class for non-blocking periodic execution
class Task {
public:
    Task(unsigned long interval, void (*callback)())
        : interval(interval), callback(callback), lastExecutionTime(0), isRunning(true), executionCount(0), maxExecutionCount(0) {}

    void update() {
        if (!isRunning) return;
        
        unsigned long currentTime = millis();
        
        if (currentTime - lastExecutionTime >= interval) {
            lastExecutionTime = currentTime;
            
            if (callback) {
                callback();
            }
            
            if (maxExecutionCount > 0) {
                executionCount++;
                if (executionCount >= maxExecutionCount) {
                    isRunning = false;
                }
            }
        }
    }

    void start() {
        isRunning = true;
        executionCount = 0;
    }

    void stop() {
        isRunning = false;
    }

    void setInterval(unsigned long newInterval) {
        interval = newInterval;
    }

    void setMaxExecutionCount(unsigned long count) {
        maxExecutionCount = count;
        executionCount = 0;
    }

    bool isActive() const {
        return isRunning;
    }

private:
    unsigned long interval;
    void (*callback)();
    unsigned long lastExecutionTime;
    bool isRunning;
    unsigned long executionCount;
    unsigned long maxExecutionCount;
};

// Scheduler class for managing multiple tasks
class Scheduler {
public:
    Scheduler() : taskCount(0) {}

    void addTask(Task* task) {
        if (taskCount < MAX_TASKS) {
            tasks[taskCount++] = task;
        }
    }

    void update() {
        for (int i = 0; i < taskCount; i++) {
            if (tasks[i]) {
                tasks[i]->update();
            }
        }
    }

private:
    static const int MAX_TASKS = 10;
    Task* tasks[MAX_TASKS];
    int taskCount;
};

// Global objects
Scheduler scheduler;
const int LED_PIN = 13;

// Task callbacks
void ledToggle() {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void sensorUpdate() {
    Serial.println("Sensor Update");
}

void heartbeat() {
    Serial.println("Heartbeat");
}

// Task objects
Task task1(100, ledToggle);
Task task2(500, sensorUpdate);
Task task3(1000, heartbeat);

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
    // Register tasks with scheduler
    scheduler.addTask(&task1);
    scheduler.addTask(&task2);
    scheduler.addTask(&task3);
    
    Serial.println("Task Scheduler Started");
}

void loop() {
    // Non-blocking update of all tasks
    scheduler.update();
}
