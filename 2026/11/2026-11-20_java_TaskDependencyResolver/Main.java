import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Set;

class TaskDependencyResolver {
	private final Set<String> tasks = new HashSet<>();
	private final Map<String, Set<String>> dependents = new HashMap<>();
	private final Map<String, Integer> dependencyCounts = new HashMap<>();

	public void addTask(String task) {
		if (tasks.add(task)) {
			dependents.put(task, new HashSet<>());
			dependencyCounts.put(task, 0);
		}
	}

	public void addDependency(String task, String dependency) {
		addTask(task);
		addTask(dependency);

		if (dependents.get(dependency).add(task)) {
			dependencyCounts.put(task, dependencyCounts.get(task) + 1);
		}
	}

	public boolean removeDependency(String task, String dependency) {
		Set<String> dependencyDependents = dependents.get(dependency);
		if (dependencyDependents == null || !dependencyDependents.remove(task)) {
			return false;
		}

		dependencyCounts.put(task, dependencyCounts.get(task) - 1);
		return true;
	}

	public List<String> resolve() {
		Map<String, Integer> remainingDependencies = new HashMap<>(dependencyCounts);
		PriorityQueue<String> ready = new PriorityQueue<>();

		for (String task : tasks) {
			if (remainingDependencies.get(task) == 0) {
				ready.add(task);
			}
		}

		List<String> order = new ArrayList<>(tasks.size());
		while (!ready.isEmpty()) {
			String task = ready.remove();
			order.add(task);

			for (String dependent : dependents.get(task)) {
				int remaining = remainingDependencies.get(dependent) - 1;
				remainingDependencies.put(dependent, remaining);
				if (remaining == 0) {
					ready.add(dependent);
				}
			}
		}

		if (order.size() != tasks.size()) {
			throw new IllegalStateException("Circular dependency detected");
		}
		return order;
	}
}

public class Main {
	public static void main(String[] args) {
		TaskDependencyResolver resolver = new TaskDependencyResolver();
		resolver.addDependency("Network", "Driver");
		resolver.addDependency("Application", "Network");
		resolver.addDependency("Application", "Logger");
		resolver.addTask("Monitor");

		System.out.println(resolver.resolve());
	}
}
