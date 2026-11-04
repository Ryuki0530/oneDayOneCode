import java.util.ArrayDeque;
import java.util.Deque;

public class Main {
	public static void main(String[] args) {
		String[] commands = {
			"APPEND Hello",
			"APPEND World",
			"UNDO",
			"REDO",
			"UNDO",
			"APPEND Java",
			"REDO"
		};

		Deque<String> undo = new ArrayDeque<>();
		Deque<String> redo = new ArrayDeque<>();
		String text = "";

		for (String command : commands) {
			if (command.startsWith("APPEND ")) {
				undo.push(text);
				text += command.substring("APPEND ".length());
				redo.clear();
			} else if (command.equals("UNDO") && !undo.isEmpty()) {
				redo.push(text);
				text = undo.pop();
			} else if (command.equals("REDO") && !redo.isEmpty()) {
				undo.push(text);
				text = redo.pop();
			}

			System.out.println("[" + text + "]");
		}
	}
}
