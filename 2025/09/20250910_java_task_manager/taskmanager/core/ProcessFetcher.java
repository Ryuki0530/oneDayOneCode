
package core;

import java.util.*;

public class ProcessFetcher {
	public static class ProcessInfo {
		public final String imageName;
		public final int pid;
		public final long memUsage;
		public ProcessInfo(String imageName, int pid, long memUsage) {
			this.imageName = imageName;
			this.pid = pid;
			this.memUsage = memUsage;
		}
	}

	public static List<ProcessInfo> fetchProcesses() throws Exception {
		// Windows: tasklist /fo csv /nh
		List<String> lines = CmdRunner.run("cmd", "/c", "tasklist /fo csv /nh");
		List<String[]> parsed = CsvUtil.parseLines(lines);
		List<ProcessInfo> result = new ArrayList<>();
		for (String[] row : parsed) {
			if (row.length < 5) continue;
			String imageName = row[0];
			int pid;
			long memUsage;
			try {
				pid = Integer.parseInt(row[1]);
				// "12,345 K" → 12345
				String mem = row[4].replace(",", "").replace(" K", "").replace("k", "");
				memUsage = Long.parseLong(mem.trim());
			} catch (Exception e) {
				continue;
			}
			result.add(new ProcessInfo(imageName, pid, memUsage));
		}
		return result;
	}
}

