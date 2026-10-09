import java.util.ArrayDeque;

class Editor {
	private String text = "";
	private final ArrayDeque<String> undoHistory = new ArrayDeque<>();
	private final ArrayDeque<String> redoHistory = new ArrayDeque<>();

	public void append(String value) {
		if (value.isEmpty()) {
			return;
		}
		undoHistory.push(text);
		text += value;
		redoHistory.clear();
	}

	public void undo() {
		if (undoHistory.isEmpty()) {
			return;
		}
		redoHistory.push(text);
		text = undoHistory.pop();
	}

	public void redo() {
		if (redoHistory.isEmpty()) {
			return;
		}
		undoHistory.push(text);
		text = redoHistory.pop();
	}

	public String getText() {
		return text;
	}
}

// for test
public class Main {
	public static void main(String[] args) {
		Editor editor = new Editor();
		editor.append("Java");
		print(editor);
		editor.append(" is");
		print(editor);
		editor.append(" fun");
		print(editor);
		editor.undo();
		print(editor);
		editor.undo();
		print(editor);
		editor.redo();
		print(editor);
		editor.append(" great");
		print(editor);
		editor.redo();
		print(editor);
	}

	private static void print(Editor editor) {
		System.out.println("[" + editor.getText() + "]");
	}
}
