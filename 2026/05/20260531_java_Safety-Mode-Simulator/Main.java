import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;

public class Main {
	public static void main(String[] args) throws Exception {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		int n = Integer.parseInt(br.readLine().trim());

		Map<String, Map<String, String>> transitions = new HashMap<>();
		addTransition(transitions, "OFF", "START", "IDLE");
		addTransition(transitions, "IDLE", "START", "RUNNING");
		addTransition(transitions, "RUNNING", "STOP", "IDLE");
		addTransition(transitions, "IDLE", "STOP", "OFF");
		addTransition(transitions, "IDLE", "FAIL", "ERROR");
		addTransition(transitions, "RUNNING", "FAIL", "ERROR");
		addTransition(transitions, "ERROR", "RESET", "IDLE");
		addTransition(transitions, "ERROR", "CLEAR", "OFF");

		String state = "OFF";
		int invalidCount = 0;

		for (int i = 0; i < n; i++) {
			String command = br.readLine().trim();
			Map<String, String> stateTransitions = transitions.get(state);
			if (stateTransitions != null && stateTransitions.containsKey(command)) {
				state = stateTransitions.get(command);
			} else {
				invalidCount++;
			}
		}

		System.out.println(state + " " + invalidCount);
	}

	private static void addTransition(Map<String, Map<String, String>> transitions,
									  String currentState,
									  String command,
									  String nextState) {
		transitions.computeIfAbsent(currentState, k -> new HashMap<>()).put(command, nextState);
	}
}
