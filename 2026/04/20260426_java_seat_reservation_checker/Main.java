import java.io.BufferedInputStream;
import java.io.IOException;

public class Main {
	private static class FastScanner {
		private final BufferedInputStream in = new BufferedInputStream(System.in);
		private final byte[] buffer = new byte[1 << 16];
		private int ptr = 0;
		private int len = 0;

		private int read() throws IOException {
			if (ptr >= len) {
				len = in.read(buffer);
				ptr = 0;
				if (len <= 0) return -1;
			}
			return buffer[ptr++];
		}

		String next() throws IOException {
			StringBuilder sb = new StringBuilder();
			int c;
			do {
				c = read();
			} while (c != -1 && c <= ' ');

			while (c != -1 && c > ' ') {
				sb.append((char) c);
				c = read();
			}
			return sb.toString();
		}

		int nextInt() throws IOException {
			return Integer.parseInt(next());
		}
	}

	public static void main(String[] args) throws Exception {
		FastScanner fs = new FastScanner();
		int n = fs.nextInt();
		int q = fs.nextInt();

		boolean[] reserved = new boolean[n + 1];
		StringBuilder out = new StringBuilder();

		for (int i = 0; i < q; i++) {
			String command = fs.next();
			int x = fs.nextInt();

			if ("reserve".equals(command)) {
				if (!reserved[x]) {
					reserved[x] = true;
					out.append("reserved\n");
				} else {
					out.append("already reserved\n");
				}
			} else if ("cancel".equals(command)) {
				if (reserved[x]) {
					reserved[x] = false;
					out.append("canceled\n");
				} else {
					out.append("not reserved\n");
				}
			} else if ("check".equals(command)) {
				out.append(reserved[x] ? "reserved\n" : "available\n");
			}
		}

		System.out.print(out);
	}
}
