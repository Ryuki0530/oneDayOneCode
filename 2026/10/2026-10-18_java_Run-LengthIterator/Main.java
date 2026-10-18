import java.util.ArrayList;
import java.util.List;

public class Main {
	static List<List<Integer>> splitBatches(int[] values, int size) {
		if (size <= 0) {
			throw new IllegalArgumentException("size must be positive");
		}

		List<List<Integer>> batches = new ArrayList<>();
		for (int start = 0; start < values.length; start += size) {
			int end = Math.min(start + size, values.length);
			List<Integer> batch = new ArrayList<>();
			for (int i = start; i < end; i++) {
				batch.add(values[i]);
			}
			batches.add(batch);
		}
		return batches;
	}

    //test
	public static void main(String[] args) {
		System.out.println(splitBatches(new int[]{10, 20, 30, 40, 50, 60, 70}, 3));
		System.out.println(splitBatches(new int[]{1, 2, 3, 4}, 2));
		System.out.println(splitBatches(new int[]{1, 2}, 5));
		System.out.println(splitBatches(new int[]{}, 3));

		try {
			splitBatches(new int[]{1, 2}, 0);
		} catch (IllegalArgumentException e) {
			System.out.println("IllegalArgumentException");
		}
	}
}
