# 今日の課題: TypeScript チュートリアル課題
**タイトル:** Simple Task CLI in TypeScript

## 概要
TypeScript の基本を一通り触るために、**コマンドラインで動く簡易タスク管理ツール**を作成してください。

この課題では、次の内容を一通り体験することを目的にします。

- TypeScript の環境構築
- `type` / `interface` の利用
- 関数の引数と戻り値の型注釈
- 配列操作
- 文字列処理
- JSON ファイルの読み書き
- コマンドライン引数の処理

---

## 想定環境
- OS: Windows 11
- エディタ: VSCode
- 実行環境: Node.js
- 言語: TypeScript

---

## 環境構築手順

### 1. Node.js をインストール
まず Node.js の **LTS 版** をインストールしてください。

インストール後、PowerShell で以下を実行して確認します。

```powershell
node -v
npm -v
```

バージョンが表示されればOKです。

---

### 2. 作業フォルダを作成
任意の場所に作業フォルダを作成してください。

例:
```powershell
mkdir ts_tutorial_task
cd ts_tutorial_task
```

---

### 3. npm プロジェクトを初期化
以下を実行してください。

```powershell
npm init -y
```

`package.json` が作成されればOKです。

---

### 4. TypeScript をインストール
以下を実行してください。

```powershell
npm install --save-dev typescript @types/node
```

---

### 5. TypeScript 設定ファイルを作成
以下を実行してください。

```powershell
npx tsc --init
```

`tsconfig.json` が作成されればOKです。

---

### 6. 最初の TypeScript ファイルを作成
`src` フォルダを作成し、その中に `main.ts` を作成してください。

例:
```powershell
mkdir src
code .
```

VSCode でフォルダを開いたら、`src/main.ts` を作成します。

---

### 7. コンパイル確認
最初は `src/main.ts` に次のような簡単なコードを書いて動作確認してもよいです。

```ts
const message: string = "Hello TypeScript";
console.log(message);
```

コンパイル:

```powershell
npx tsc
```

実行:

```powershell
node src/main.js
```

`Hello TypeScript` と表示されれば環境構築完了です。

---

## 課題
簡易タスク管理CLIを作成してください。

### 実装するコマンド
以下の3つを実装してください。

#### 1. タスク一覧表示
```powershell
node src/main.js list
```

保存されているタスク一覧を表示する。

#### 2. タスク追加
```powershell
node src/main.js add "レポート提出" 20260420
```

新しいタスクを追加する。

#### 3. タスク完了
```powershell
node src/main.js done 1
```

指定したIDのタスクを完了状態にする。

---

## データ仕様
タスクは `tasks.json` に保存してください。

### 例
```json
[
  {
    "id": 1,
    "title": "レポート提出",
    "dueDate": "20260420",
    "done": false
  },
  {
    "id": 2,
    "title": "発表資料作成",
    "dueDate": "20260425",
    "done": true
  }
]
```

---

## 型定義要件
少なくとも以下のような型定義を使ってください。

```ts
type Task = {
  id: number;
  title: string;
  dueDate: string;
  done: boolean;
};
```

`interface` を使っても構いません。

---

## 要件
- タスクは JSON ファイルに保存すること
- 起動時に JSON ファイルを読み込むこと
- 追加時にIDを自動採番すること
- 完了時に該当IDがない場合はエラーメッセージを出すこと
- 一覧表示では、完了済みか未完了か分かるように表示すること

---

## 表示例

### 追加
```text
タスクを追加しました:
[1] レポート提出 (期限: 20260420)
```

### 一覧
```text
[ ] 1: レポート提出 (期限: 20260420)
[x] 2: 発表資料作成 (期限: 20260425)
```

### 完了
```text
タスク 1 を完了にしました。
```

### エラー例
```text
ID 99 のタスクは存在しません。
```

---

## ヒント

### 1. コマンドライン引数
Node.js では、コマンドライン引数は `process.argv` から取得できます。

例:
```ts
const args = process.argv.slice(2);
```

---

### 2. JSON ファイル操作
Node.js の `fs` モジュールを使うと、ファイルの読み書きができます。

使いそうなもの:
- `readFileSync`
- `writeFileSync`
- `existsSync`

---

### 3. 最初の分割方針
最初は `main.ts` 1ファイルでも構いません。余裕があれば次のように分割してください。

- `src/main.ts`
- `src/taskStore.ts`
- `src/taskTypes.ts`

---

## 余裕があれば
以下の拡張にも挑戦してください。

- `delete` コマンドを追加
- 期限順に並べて表示
- 期限切れタスクを表示
- `undone` コマンドを追加
- タイトル部分一致検索

---

## 学習ポイント
この課題で慣れてほしい点:

- TypeScript の基本的な型の書き方
- Node.js 上で TypeScript を扱う流れ
- 「文字列で受け取った引数」を適切に処理する感覚
- 小さなCLIツールを型付きで組み立てる感覚

---

## クリア条件
以下を満たせばクリアです。

- `add` で追加できる
- `list` で一覧表示できる
- `done` で完了にできる
- JSON ファイル保存ができる
- TypeScript の型注釈が適切に付いている

---

## 追加チャレンジ
最後に、次の点を自分の言葉で説明できるようにしてください。

1. JavaScript ではなく TypeScript を使う利点は何か
2. `type` と `interface` はどう使い分けられそうか
3. 今回の課題で型が役立った場面はどこか