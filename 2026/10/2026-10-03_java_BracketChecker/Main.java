import java.util.ArrayDeque;

public class Main {
	static boolean isBalanced(String text) {
		ArrayDeque<Character> stack = new ArrayDeque<>();

		for (char c : text.toCharArray()) {
			if (c == '(' || c == '[' || c == '{') {
				stack.push(c);
			} else if (c == ')' || c == ']' || c == '}') {
				if (stack.isEmpty() || !matches(stack.pop(), c)) {
					return false;
				}
			}
		}

		return stack.isEmpty();
	}

	private static boolean matches(char opening, char closing) {
		return (opening == '(' && closing == ')')
				|| (opening == '[' && closing == ']')
				|| (opening == '{' && closing == '}');
	}

	public static void main(String[] args) {
		String[] inputs = {
				"(a + b) * [c - {d / e}]",
				"([)]",
				"(() ",
				"abc",
				""
		};

		for (String input : inputs) {
			System.out.println(isBalanced(input));
		}
	}
}
