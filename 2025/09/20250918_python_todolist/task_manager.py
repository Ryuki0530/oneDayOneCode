import json
import os
from datetime import datetime
from typing import List, Dict, Any

class TaskManager:
    def __init__(self, filename: str = "tasks.json"):
        self.filename = filename
        self.tasks: List[Dict[str, Any]] = []
        self.load_tasks()
        self._next_id = self._get_next_id()

    def _get_next_id(self):
        if not self.tasks:
            return 1
        return max(task["id"] for task in self.tasks) + 1

    def load_tasks(self):
        if os.path.exists(self.filename):
            with open(self.filename, "r", encoding="utf-8") as f:
                self.tasks = json.load(f)
        else:
            self.tasks = []

    def save_tasks(self):
        with open(self.filename, "w", encoding="utf-8") as f:
            json.dump(self.tasks, f, ensure_ascii=False, indent=2)

    def add_task(self, title: str, due: str = None):
        task = {
            "id": self._next_id,
            "title": title,
            "due": due,
            "done": False,
        }
        self.tasks.append(task)
        self._next_id += 1
        self.save_tasks()

    def list_tasks(self, pending_only: bool = False) -> List[Dict[str, Any]]:
        if pending_only:
            return [task for task in self.tasks if not task["done"]]
        return self.tasks

    def mark_done(self, task_id: int) -> bool:
        for task in self.tasks:
            if task["id"] == task_id:
                task["done"] = True
                self.save_tasks()
                return True
        return False