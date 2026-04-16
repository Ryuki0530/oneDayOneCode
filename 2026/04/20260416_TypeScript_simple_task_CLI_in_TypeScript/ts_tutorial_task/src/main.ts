import * as fs from "fs";
import * as path from "path";

type Task = {
    id: number;
    title: string;
    dueDate: string;
    done: boolean;
};

const TASKS_FILE = path.join(__dirname, "tasks.json");

function loadTasks(): Task[] {
    if (!fs.existsSync(TASKS_FILE)) {
        return [];
    }
    const data = fs.readFileSync(TASKS_FILE, "utf-8");
    return JSON.parse(data);
}

function saveTasks(tasks: Task[]): void {
    fs.writeFileSync(TASKS_FILE, JSON.stringify(tasks, null, 2));
}

function addTask(title: string, dueDate: string): void {
    const tasks = loadTasks();
    const newId = tasks.length > 0 ? Math.max(...tasks.map(t => t.id)) + 1 : 1;
    const newTask: Task = {
        id: newId,
        title,
        dueDate,
        done: false,
    };
    tasks.push(newTask);
    saveTasks(tasks);
    console.log(`タスクを追加しました:`);
    console.log(`[${newId}] ${title} (期限: ${dueDate})`);
}

function listTasks(): void {
    const tasks = loadTasks();
    if (tasks.length === 0) {
        console.log("タスクはありません。");
        return;
    }
    tasks.forEach(task => {
        const checkbox = task.done ? "[x]" : "[ ]";
        console.log(`${checkbox} ${task.id}: ${task.title} (期限: ${task.dueDate})`);
    });
}

function completeTask(id: number): void {
    const tasks = loadTasks();
    const task = tasks.find(t => t.id === id);
    if (!task) {
        console.log(`ID ${id} のタスクは存在しません。`);
        return;
    }
    task.done = true;
    saveTasks(tasks);
    console.log(`タスク ${id} を完了にしました。`);
}

function main(): void {
    const args = process.argv.slice(2);
    const command = args[0];

    if (command === "add" && args.length >= 3) {
        addTask(args[1], args[2]);
    } else if (command === "list") {
        listTasks();
    } else if (command === "done" && args.length >= 2) {
        completeTask(parseInt(args[1], 10));
    } else {
        console.log("コマンドが不正です。");
        console.log("使用方法:");
        console.log("  node src/main.js list");
        console.log("  node src/main.js add <title> <dueDate>");
        console.log("  node src/main.js done <id>");
    }
}

main();