import csv
import sys
from datetime import datetime, timedelta
from pathlib import Path

class TodoList:
    """TODOリスト管理クラス"""
    
    def __init__(self, csv_file="tasks.csv"):
        self.csv_file = Path(csv_file)
        self.tasks = []
        self.load_tasks()
    
    def load_tasks(self):
        """CSVファイルからタスクを読み込む"""
        if self.csv_file.exists():
            with open(self.csv_file, 'r', encoding='utf-8') as f:
                reader = csv.reader(f)
                self.tasks = [row for row in reader]
    
    def save_tasks(self):
        """タスクをCSVファイルに保存"""
        with open(self.csv_file, 'w', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)
            writer.writerows(self.tasks)
    
    def get_next_task_id(self):
        """次のタスクIDを取得"""
        if not self.tasks:
            return 1
        return max(int(task[0]) for task in self.tasks) + 1
    
    def add_task(self, name, deadline, message=""):
        """タスクを追加"""
        if not name or not self._validate_deadline(deadline):
            raise ValueError("無効なタスク名または期限です")
        
        task_id = self.get_next_task_id()
        self.tasks.append([str(task_id), "0", name, deadline, message])
        self.save_tasks()
        print(f"タスク追加: [{task_id}] {name} ({deadline})")
    
    def add_recurring_tasks(self, name, deadline, count, message=""):
        """毎週繰り返しでタスクを追加"""
        if count < 1:
            raise ValueError("回数は1以上である必要があります")
        
        dt = datetime.strptime(deadline, "%Y%m%d%H%M")
        for i in range(count):
            current_deadline = (dt + timedelta(weeks=i)).strftime("%Y%m%d%H%M")
            self.add_task(name, current_deadline, message)
    
    def mark_complete_by_id(self, task_id):
        """タスクIDで完了マーク"""
        for task in self.tasks:
            if task[0] == str(task_id):
                task[1] = "1"
                self.save_tasks()
                print(f"タスク完了: [{task_id}] {task[2]}")
                return
        print(f"エラー: タスクID {task_id} が見つかりません")
    
    def mark_complete_by_name(self, name):
        """タスク名で完了マーク"""
        matches = [task for task in self.tasks if task[2] == name]
        
        if not matches:
            print(f"エラー: タスク名 '{name}' が見つかりません")
            return
        
        if len(matches) > 1:
            print(f"警告: '{name}' に一致するタスクが複数あります:")
            for task in matches:
                print(f"  [{task[0]}] {task[2]} ({task[3]})")
            print("すべてのタスクを完了状態にしました")
        
        for task in matches:
            task[1] = "1"
        self.save_tasks()
    
    def display_tasks(self, show_all=False):
        """タスクを表示（既定では期限切れかつ完了済みを非表示）"""
        tasks_to_display = []
        for task in self.tasks:
            if show_all or not self._is_overdue_and_completed(task):
                tasks_to_display.append(task)

        if not tasks_to_display:
            print("タスクがありません")
            return
        
        print("\n=== TODOリスト ===")
        for task in tasks_to_display:
            task_id, status, name, deadline, message = task
            status_str = "✓" if status == "1" else " "
            formatted_deadline = self._format_deadline(deadline)
            
            print(f"[{task_id}] {status_str} {name} ({formatted_deadline})", end="")
            if message:
                print(f" - {message}", end="")
            print()
        print()
    
    @staticmethod
    def _validate_deadline(deadline):
        """期限の形式を検証"""
        try:
            datetime.strptime(deadline, "%Y%m%d%H%M")
            return True
        except ValueError:
            return False
    
    @staticmethod
    def _format_deadline(deadline):
        """期限を見やすい形式に整形"""
        try:
            dt = datetime.strptime(deadline, "%Y%m%d%H%M")
            return dt.strftime("%Y-%m-%d %H:%M")
        except ValueError:
            return deadline

    @staticmethod
    def _is_overdue_and_completed(task):
        """期限切れかつ完了済みかどうかを判定"""
        if len(task) < 4:
            return False

        status = task[1]
        deadline = task[3]

        if status != "1":
            return False

        try:
            dt = datetime.strptime(deadline, "%Y%m%d%H%M")
            return dt < datetime.now()
        except ValueError:
            return False


def main():
    """メイン処理"""
    todo = TodoList()
    
    if len(sys.argv) == 1:
        todo.display_tasks(show_all=False)
        return

    if len(sys.argv) == 2 and sys.argv[1] == "--all":
        todo.display_tasks(show_all=True)
        return
    
    command = sys.argv[1]
    
    try:
        if command == "add":
            if len(sys.argv) < 4:
                print("エラー: add コマンドにはタスク名と期限が必要です")
                return
            
            name = sys.argv[2]
            deadline = sys.argv[3]
            message = ""
            count = 1
            
            i = 4
            while i < len(sys.argv):
                if sys.argv[i] == "-m" and i + 1 < len(sys.argv):
                    message = sys.argv[i + 1]
                    i += 2
                elif sys.argv[i] == "-r" and i + 1 < len(sys.argv):
                    count = int(sys.argv[i + 1])
                    i += 2
                else:
                    i += 1
            
            if count > 1:
                todo.add_recurring_tasks(name, deadline, count, message)
            else:
                todo.add_task(name, deadline, message)
        
        elif command == "fin":
            if len(sys.argv) < 3:
                print("エラー: fin コマンドにはタスクIDまたは --name オプションが必要です")
                return
            
            if sys.argv[2] == "--name":
                if len(sys.argv) < 4:
                    print("エラー: --name の後にタスク名を指定してください")
                    return
                todo.mark_complete_by_name(sys.argv[3])
            else:
                todo.mark_complete_by_id(sys.argv[2])
        
        else:
            print(f"エラー: 不明なコマンド '{command}'")
    
    except ValueError as e:
        print(f"エラー: {e}")
    except Exception as e:
        print(f"予期しないエラー: {e}")


if __name__ == "__main__":
    main()

    