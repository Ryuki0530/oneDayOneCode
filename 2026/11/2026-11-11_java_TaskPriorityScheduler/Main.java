import java.util.HashMap;
import java.util.Map;

import java.util.List;
import java.util.ArrayList;

//タスクスキーマ
class Task{
    String name;
    int priority;
    public Task(String name, int priority) {
        this.name = name;
        this.priority = priority;
    }
}

class TaskScheduler{

    private ArrayList<Task> taskQueue = new ArrayList<>();

    // タスクを登録する
    public void addTask(Task t) {
        taskQueue.add(t);
    }

    // 最優先のタスク名を返し、キューから削除する
    public String executeNext() {
        // 最も優先度が高いタスクを検索
        Task highestPriorityTask = null;
        int highestPriority = Integer.MAX_VALUE;
        for (Task t : taskQueue) {
            if (t.priority < highestPriority) {
                highestPriority = t.priority;
                highestPriorityTask = t;
            }
        }

        // 最も優先度が高いタスクが存在しない場合はnullを返す
        if (highestPriorityTask == null) {
            return null;
        }

        // 最も優先度が高いタスクをキューから削除
        taskQueue.remove(highestPriorityTask);
        return highestPriorityTask.name;
    }

    // 未実行タスク数を返す
    public int pendingCount() {
        return taskQueue.size();
    }

  
}

//test
class Main {
  public static void main(String[] args) {
    TaskScheduler scheduler = new TaskScheduler();
    scheduler.addTask(new Task("Logging", 3));
    scheduler.addTask(new Task("SensorRead", 1));
    scheduler.addTask(new Task("MotorControl", 1));
    scheduler.addTask(new Task("Backup", 5));

    System.out.println(scheduler.executeNext()); 
    System.out.println(scheduler.pendingCount()); 
    System.out.println(scheduler.executeNext()); 
    System.out.println(scheduler.pendingCount());
    System.out.println(scheduler.executeNext()); 
    System.out.println(scheduler.pendingCount());
    System.out.println(scheduler.executeNext()); 
    System.out.println(scheduler.pendingCount());
  }
}
