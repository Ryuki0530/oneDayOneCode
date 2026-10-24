public class Main {
	static int compareVersions(String a, String b) {
		String[] partsA = a.split("\\.");
		String[] partsB = b.split("\\.");

		for (int i = 0; i < Math.max(partsA.length, partsB.length); i++) {
			int valueA = i < partsA.length ? Integer.parseInt(partsA[i]) : 0;
			int valueB = i < partsB.length ? Integer.parseInt(partsB[i]) : 0;

			if (valueA < valueB) {
				return -1;
			}
			if (valueA > valueB) {
				return 1;
			}
		}
		return 0;
	}

    //test 
	public static void main(String[] args) {
		String[][] pairs = {
			{"1.9", "1.10"},
			{"2.0", "2"},
			{"1.02.0", "1.2"},
			{"3.0.1", "3"},
			{"0.9", "1.0"}
		};

		for (String[] pair : pairs) {
			System.out.println(pair[0] + " vs " + pair[1] + ": "
					+ compareVersions(pair[0], pair[1]));
		}
	}
}
