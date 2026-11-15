import heapq


class PriorityTaskScheduler:
	def __init__(self):
		self._queue = []
		self._sequence = 0
		self._count = 0
		self._cancelled = set()

	def add_task(self, name: str, priority: int) -> None:
		heapq.heappush(self._queue, (priority, self._sequence, name))
		self._sequence += 1
		self._count += 1

	def _remove_cancelled(self) -> None:
		while self._queue and self._queue[0][1] in self._cancelled:
			_, sequence, _ = heapq.heappop(self._queue)
			self._cancelled.remove(sequence)

	def execute_next(self):
		self._remove_cancelled()
		if not self._queue:
			return None

		_, _, name = heapq.heappop(self._queue)
		self._count -= 1
		return name

	def pending_count(self) -> int:
		return self._count

	def peek_next(self):
		self._remove_cancelled()
		if not self._queue:
			return None
		return self._queue[0][2]

	def cancel_task(self, name) -> bool:
		found = False
		for _, sequence, task_name in self._queue:
			if task_name == name and sequence not in self._cancelled:
				self._cancelled.add(sequence)
				self._count -= 1
				found = True
		return found


def main() -> None:
	"""PriorityTaskScheduler の動作確認を標準出力に表示する。"""
	scheduler = PriorityTaskScheduler()

	print("入力: add_task('通常タスク', 2)")
	scheduler.add_task("通常タスク", 2)
	print("入力: add_task('緊急タスク', 1)")
	scheduler.add_task("緊急タスク", 1)
	print("入力: add_task('後回しタスク', 3)")
	scheduler.add_task("後回しタスク", 3)

	print(f"結果: peek_next() -> {scheduler.peek_next()}")
	print("入力: cancel_task('通常タスク')")
	print(f"結果: {scheduler.cancel_task('通常タスク')}")
	print(f"結果: pending_count() -> {scheduler.pending_count()}")
	print(f"結果: execute_next() -> {scheduler.execute_next()}")
	print(f"結果: execute_next() -> {scheduler.execute_next()}")
	print(f"結果: execute_next() -> {scheduler.execute_next()}")
	print(f"結果: pending_count() -> {scheduler.pending_count()}")


if __name__ == "__main__":
	main()
