
package core;

import java.util.*;

public class CsvUtil {
	// 1行のCSVをパース（ダブルクォート・カンマ対応）
	public static String[] parseLine(String line) {
		List<String> result = new ArrayList<>();
		StringBuilder sb = new StringBuilder();
		boolean inQuotes = false;
		for (int i = 0; i < line.length(); i++) {
			char c = line.charAt(i);
			if (c == '"') {
				if (inQuotes && i + 1 < line.length() && line.charAt(i + 1) == '"') {
					sb.append('"');
					i++;
				} else {
					inQuotes = !inQuotes;
				}
			} else if (c == ',' && !inQuotes) {
				result.add(sb.toString());
				sb.setLength(0);
			} else {
				sb.append(c);
			}
		}
		result.add(sb.toString());
		return result.toArray(new String[0]);
	}

	// 複数行のCSVをパース
	public static List<String[]> parseLines(List<String> lines) {
		List<String[]> parsed = new ArrayList<>();
		for (String line : lines) {
			parsed.add(parseLine(line));
		}
		return parsed;
	}
}