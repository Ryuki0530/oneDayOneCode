import java.util.ArrayDeque;
import java.util.Queue;

public class Main {
	static int shortestPath(String[] maze) {
		int rows = maze.length;
		int cols = maze[0].length();
		int startRow = 0;
		int startCol = 0;
		int goalRow = 0;
		int goalCol = 0;

		for (int row = 0; row < rows; row++) {
			for (int col = 0; col < cols; col++) {
				char cell = maze[row].charAt(col);
				if (cell == 'S') {
					startRow = row;
					startCol = col;
				} else if (cell == 'G') {
					goalRow = row;
					goalCol = col;
				}
			}
		}

		int[][] distance = new int[rows][cols];
		for (int row = 0; row < rows; row++) {
			for (int col = 0; col < cols; col++) {
				distance[row][col] = -1;
			}
		}

		Queue<int[]> queue = new ArrayDeque<>();
		queue.offer(new int[] {startRow, startCol});
		distance[startRow][startCol] = 0;

		int[] rowDirection = {-1, 1, 0, 0};
		int[] colDirection = {0, 0, -1, 1};

		while (!queue.isEmpty()) {
			int[] current = queue.poll();
			int row = current[0];
			int col = current[1];

			if (row == goalRow && col == goalCol) {
				return distance[row][col];
			}

			for (int direction = 0; direction < 4; direction++) {
				int nextRow = row + rowDirection[direction];
				int nextCol = col + colDirection[direction];

				if (nextRow >= 0 && nextRow < rows
						&& nextCol >= 0 && nextCol < cols
						&& maze[nextRow].charAt(nextCol) != '#'
						&& distance[nextRow][nextCol] == -1) {
					distance[nextRow][nextCol] = distance[row][col] + 1;
					queue.offer(new int[] {nextRow, nextCol});
				}
			}
		}

		return -1;
	}

	public static void main(String[] args) {
		String[] maze1 = {
				"S..#",
				".#..",
				"...G"
		};
		String[] maze2 = {
				"S#G",
				".#.",
				".#."
		};

		System.out.println(shortestPath(maze1));
		System.out.println(shortestPath(maze2));
	}
}
