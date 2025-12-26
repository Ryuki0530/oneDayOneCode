# 今日の課題（Java・JFrame・短め）

## テーマ
**「ワンクリック計測器」**  
JFrame + JButton + JLabel だけで、クリック間隔（ミリ秒）を測るミニアプリを作る。

---

## 仕様（これだけ実装すればOK）
1. 画面に以下を置く  
   - `JLabel`（表示用）  
   - `JButton`「START / LAP」  
   - `JButton`「RESET」

2. 動作
   - 初回に「START / LAP」を押したら **計測開始**  
     - `startTime = System.currentTimeMillis()` を記録  
     - `JLabel` に `Started!` と出す
   - 2回目以降に「START / LAP」を押したら **LAP（前回クリックからの差分）** を表示  
     - `lap = now - lastTime` を計算して `JLabel` に `LAP: 123 ms` のように表示  
     - `lastTime = now` を更新
   - 「RESET」を押したら状態を初期化  
     - `started=false`、時刻変数をリセット  
     - `JLabel` に `Ready` と出す

---

## 制約
- レイアウトは自由（迷ったら `FlowLayout` でOK）
- タイマー（Swing Timer）やスレッドは不要（クリック時刻だけで完結）
- 例外が出ないように null/未開始状態をちゃんと扱う

---

## ヒント（最低限）
- `JFrame` は `setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE)` を忘れがち
- イベントは `addActionListener(e -> { ... })`
- 時刻は `System.currentTimeMillis()`

---

## 追加チャレンジ（余裕があれば）
- LAP回数をカウントして `LAP #3: 120 ms` のように出す
- `JLabel` を大きめフォントにして見やすくする
