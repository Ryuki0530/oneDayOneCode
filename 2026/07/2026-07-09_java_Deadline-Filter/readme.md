# Daily Java Challenge: Deadline Filter

## 目的
`List` と `LocalDate` を使って、締切が近いタスクだけを抽出する。

## 問題
タスク名と締切日を持つ `Task` クラスを作成し、今日から **3日以内** に締切が来るタスクだけを表示してください。

## 条件
- 言語: Java
- `Task` クラスを作る
- フィールドは以下
  - `String name`
  - `LocalDate deadline`
- `main` メソッドでタスクを5件以上作る
- 今日の日付は `LocalDate.now()` で取得する
- 今日から3日以内のタスクだけ表示する
- 締切超過タスクは表示しない

## 実行例
```text
Upcoming tasks:
ES提出: 2026-07-10
面接準備: 2026-07-12