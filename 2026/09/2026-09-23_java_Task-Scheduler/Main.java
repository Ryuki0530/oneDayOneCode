import java.util.Comparator;
import java.util.PriorityQueue;

class Task {
	final String name;
	final int priority;
	final int order;

	Task(String name, int priority, int order) {
		this.name = name;
		this.priority = priority;
		this.order = order;
	}
}

class TaskScheduler {
	private final PriorityQueue<Task> tasks = new PriorityQueue<>(
			Comparator.comparingInt((Task task) -> task.priority)
					.reversed()
					.thenComparingInt(task -> task.order));
	private int nextOrder;

	public void addTask(String name, int priority) {
		tasks.offer(new Task(name, priority, nextOrder++));
	}

	public Task pollTask() {
		return tasks.poll();
	}

	public Task peekTask() {
		return tasks.peek();
	}
}

public class Main {
	public static void main(String[] args) {
		TaskScheduler scheduler = new TaskScheduler();
		scheduler.addTask("Compile", 2);
		scheduler.addTask("Test", 3);
		scheduler.addTask("Deploy", 1);
		scheduler.addTask("Lint", 3);
		scheduler.addTask("Package", 2);

		Task task;
		while ((task = scheduler.pollTask()) != null) {
			System.out.println(task.name);
		}
	}
}
