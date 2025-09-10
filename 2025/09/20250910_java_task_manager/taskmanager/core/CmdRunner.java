
package core;

import java.io.*;
import java.nio.charset.Charset;
import java.util.*;

public class CmdRunner {
	public static List<String> run(String... command) throws IOException {
		ProcessBuilder pb = new ProcessBuilder(command);
		pb.redirectErrorStream(true);
		Process process = pb.start();
		List<String> lines = new ArrayList<>();
		try (BufferedReader reader = new BufferedReader(
				new InputStreamReader(process.getInputStream(), Charset.defaultCharset()))) {
			String line;
			while ((line = reader.readLine()) != null) {
				lines.add(line);
			}
		}
		try {
			process.waitFor();
		} catch (InterruptedException e) {
			Thread.currentThread().interrupt();
		}
		return lines;
	}

	public static int runAndGetExitCode(String... command) throws IOException {
		ProcessBuilder pb = new ProcessBuilder(command);
		pb.redirectErrorStream(true);
		Process process = pb.start();
		try (BufferedReader reader = new BufferedReader(
				new InputStreamReader(process.getInputStream(), Charset.defaultCharset()))) {
			while (reader.readLine() != null) {
				// consume output
			}
		}
		try {
			return process.waitFor();
		} catch (InterruptedException e) {
			Thread.currentThread().interrupt();
			return -1;
		}
	}
}
