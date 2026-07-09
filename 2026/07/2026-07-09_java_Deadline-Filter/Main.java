import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;
public class Main {
	public static void main(String[] args) {
		LocalDate today = LocalDate.now();

		List<Task> tasks = new ArrayList<>();
		tasks.add(new Task("ES提出", today.plusDays(1)));
		tasks.add(new Task("面接準備", today.plusDays(3)));
		tasks.add(new Task("ポートフォリオ更新", today.plusDays(5)));
		tasks.add(new Task("企業研究", today));
		tasks.add(new Task("履歴書見直し", today.minusDays(1)));

		System.out.println("Upcoming tasks:");
		for (Task task : tasks) {
			if (!task.deadline.isBefore(today) && !task.deadline.isAfter(today.plusDays(3))) {
				System.out.println(task.name + ": " + task.deadline);
			}
		}
	}
}

class Task {
	String name;
	LocalDate deadline;

	Task(String name, LocalDate deadline) {
		this.name = name;
		this.deadline = deadline;
	}
}
