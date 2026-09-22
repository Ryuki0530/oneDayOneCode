from rich.console import Console
from rich.table import Table
from rich.text import Text


missions = [
	{"name": "月面探査", "priority": 3, "done": False},
	{"name": "通信点検", "priority": 1, "done": True},
	{"name": "燃料補給", "priority": 2, "done": False},
	{"name": "航路計算", "priority": 3, "done": True},
]

table = Table(title="MissionBoard")
table.add_column("ミッション")
table.add_column("優先度", justify="center")
table.add_column("状態")

for mission in sorted(missions, key=lambda item: item["priority"], reverse=True):
	name = mission["name"]
	if not mission["done"] and mission["priority"] == 3:
		name = Text(name, style="red")

	table.add_row(
		name,
		str(mission["priority"]),
		"完了" if mission["done"] else "未完了",
	)

console = Console()
console.print(table)
completed = sum(mission["done"] for mission in missions)
console.print(f"完了：{completed} / {len(missions)}")
