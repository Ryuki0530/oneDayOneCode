public class Main {
	static int compareVersions(String a, String b) {
		String[] partsA = a.split("\\.");
		String[] partsB = b.split("\\.");
		int length = Math.max(partsA.length, partsB.length);

		for (int i = 0; i < length; i++) {
			int versionA = i < partsA.length ? Integer.parseInt(partsA[i]) : 0;
			int versionB = i < partsB.length ? Integer.parseInt(partsB[i]) : 0;

			if (versionA < versionB) {
				return -1;
			}
			if (versionA > versionB) {
				return 1;
			}
		}

		return 0;
	}

	public static void main(String[] args) {
		String[][] examples = {
			{"1.9", "1.10"},
			{"2.0", "2"},
			{"1.2.1", "1.2"},
			{"01.002", "1.2"},
			{"3.0.0", "3.0.1"}
		};

		for (String[] example : examples) {
			System.out.println(example[0] + " vs " + example[1] + ": "
					+ compareVersions(example[0], example[1]));
		}
	}
}
