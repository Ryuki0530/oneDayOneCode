import java.util.Comparator;
import java.util.PriorityQueue;

class Task {
	private final String name;
	private final int priority;
	private final long sequence;

	Task(String name, int priority, long sequence) {
		this.name = name;
		this.priority = priority;
		this.sequence = sequence;
	}

	String getName() {
		return name;
	}

	int getPriority() {
		return priority;
	}

	long getSequence() {
		return sequence;
	}
}

class TaskScheduler {
	private final PriorityQueue<Task> queue = new PriorityQueue<>(
			Comparator.comparingInt(Task::getPriority)
					.reversed()
					.thenComparingLong(Task::getSequence));
	private long nextSequence;

	public void addTask(String name, int priority) {
		queue.offer(new Task(name, priority, nextSequence++));
	}

	public String executeNext() {
		Task task = queue.poll();
		return task == null ? null : task.getName();
	}

	public String peekNext() {
		Task task = queue.peek();
		return task == null ? null : task.getName();
	}

	public int size() {
		return queue.size();
	}
}

public class Main {
	public static void main(String[] args) {
		TaskScheduler scheduler = new TaskScheduler();

		scheduler.addTask("Logging", 1);
		scheduler.addTask("MotorControl", 5);
		scheduler.addTask("SensorRead", 3);
		scheduler.addTask("EmergencyStop", 10);
		scheduler.addTask("MotorUpdate", 5);

		System.out.println(scheduler.peekNext());
		System.out.println(scheduler.executeNext());
		System.out.println(scheduler.executeNext());
		System.out.println(scheduler.executeNext());
		System.out.println(scheduler.executeNext());
		System.out.println(scheduler.executeNext());
		System.out.println(scheduler.executeNext());
	}
}
