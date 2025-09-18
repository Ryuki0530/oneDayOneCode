import argparse
from task_manager import TaskManager
import sys

def main():
    parser = argparse.ArgumentParser(description="ToDoリスト管理アプリ")
    subparsers = parser.add_subparsers(dest="command", required=True)

    # add
    add_parser = subparsers.add_parser("add", help="タスクを追加")
    add_parser.add_argument("title", help="タスクのタイトル")
    add_parser.add_argument("--due", help="期限 (YYYY-MM-DD)", default=None)

    # list
    list_parser = subparsers.add_parser("list", help="タスク一覧表示")
    list_parser.add_argument("--pending", action="store_true", help="未完了タスクのみ表示")

    # done
    done_parser = subparsers.add_parser("done", help="タスクを完了にする")
    done_parser.add_argument("id", type=int, help="完了にするタスクのID")

    args = parser.parse_args()
    manager = TaskManager()

    if args.command == "add":
        manager.add_task(args.title, args.due)
        print("タスクを追加しました。")
    elif args.command == "list":
        tasks = manager.list_tasks(pending_only=args.pending)
        for task in tasks:
            status = "✔" if task["done"] else " "
            due = f" (期限: {task['due']})" if task["due"] else ""
            print(f"[{status}] {task['id']}: {task['title']}{due}")
    elif args.command == "done":
        if manager.mark_done(args.id):
            print("タスクを完了にしました。")
        else:
            print("指定したIDのタスクが見つかりません。")


if __name__ == "__main__":
    main()