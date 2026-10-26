import java.util.ArrayList;
import java.util.List;

public class Main {
	static List<String> wrapText(String text, int width) {
		List<String> result = new ArrayList<>();
		String trimmedText = text.trim();

		if (trimmedText.isEmpty()) {
			return result;
		}

		StringBuilder currentLine = new StringBuilder();
		for (String word : trimmedText.split(" +")) {
			if (currentLine.length() == 0) {
				currentLine.append(word);
			} else if (currentLine.length() + 1 + word.length() <= width) {
				currentLine.append(' ').append(word);
			} else {
				result.add(currentLine.toString());
				currentLine.setLength(0);
				currentLine.append(word);
			}
		}

		result.add(currentLine.toString());
		return result;
	}

    //test
	public static void main(String[] args) {
		String text = "Java makes text processing fun and useful";
		int width = 16;

		for (String line : wrapText(text, width)) {
			System.out.println(line);
		}


        text = "  one   two three  ";
        width = 7;

        for (String line : wrapText(text, width)) {
            System.out.println(line);
        }
	}
}
