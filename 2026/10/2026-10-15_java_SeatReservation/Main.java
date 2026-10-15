import java.util.BitSet;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Main {
	public static void main(String[] args) throws IOException {
		BitSet reserved = new BitSet(10);
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		String command;

		while ((command = reader.readLine()) != null) {
			command = command.trim();
			if (command.isEmpty()) {
				continue;
			}
			String[] parts = command.split("\\s+");

			if (parts[0].equals("NEXT")) {
				int seat = reserved.nextClearBit(0);
				System.out.println(seat < 10 ? seat + 1 : "FULL");
				continue;
			}

			int seat = Integer.parseInt(parts[1]);
			if (seat < 1 || seat > 10) {
				System.out.println("INVALID");
				continue;
			}

			int index = seat - 1;
			if (parts[0].equals("BOOK")) {
				if (reserved.get(index)) {
					System.out.println("ALREADY BOOKED");
				} else {
					reserved.set(index);
					System.out.println("BOOKED");
				}
			} else {
				if (reserved.get(index)) {
					reserved.clear(index);
					System.out.println("CANCELED");
				} else {
					System.out.println("NOT BOOKED");
				}
			}
		}
	}
}
