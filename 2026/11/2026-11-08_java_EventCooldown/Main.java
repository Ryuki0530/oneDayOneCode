import java.util.HashMap;
import java.util.Map;

public class Main {
	public static void main(String[] args) {
		int cooldown = 5;
		int[][] times = {
				{0, 0},
				{1, 1},
				{3, 0},
				{5, 0},
				{5, 1},
				{6, 1},
				{8, 0},
				{10, 0}
		};
		String[] eventNames = {"LOGIN", "SAVE"};

		Map<String, Integer> lastExecutedAt = new HashMap<>();

		for (int[] event : times) {
			int time = event[0];
			String name = eventNames[event[1]];
			Integer previousTime = lastExecutedAt.get(name);

			if (previousTime == null || time - previousTime >= cooldown) {
				System.out.println(time + " " + name);
				lastExecutedAt.put(name, time);
			}
		}
	}
}
