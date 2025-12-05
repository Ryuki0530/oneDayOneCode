# Java 課題：ログファイルからユーザー統計を生成せよ

以下は Web アプリケーションのアクセスログの一部とする。  
ログは 1 行につき 1 件のアクセス記録を表し、以下の形式である。

```
<timestamp> <userId> <endpoint> <status>
```

例：
```
2025-12-04T20:12:33 user42 /login 200
2025-12-04T20:12:34 user13 /item/55 200
2025-12-04T20:12:35 user42 /logout 200
2025-12-04T20:12:37 user42 /login 403
2025-12-04T20:12:40 user13 /cart 500
```

## **【あなたが作るプログラム】**
標準入力（System.in）から複数行のログを読み取り、  
以下の統計情報を作って標準出力に表示せよ。

## **【出力内容】**

1. **ユーザーごとのアクセス回数**  
   `userId count`

2. **ステータスコードごとの件数**  
   `status count`

3. **最もアクセス回数が多いユーザー（複数なら全員）**  
   `Most Active: userId1 userId2 ...`

4. **500 番台エラーを出したユーザー一覧（1 回でもあれば）**  
   `Errored Users: userId1 userId2 ...`  
   ※並び順は任意

---

## **【入出力例】**

### 入力
```
2025-12-04T20:12:33 user42 /login 200
2025-12-04T20:12:34 user13 /item/55 200
2025-12-04T20:12:35 user42 /logout 200
2025-12-04T20:12:37 user42 /login 403
2025-12-04T20:12:40 user13 /cart 500
```

### 出力
```
# User Access Count
user42 3
user13 2

# Status Count
200 3
403 1
500 1

# Most Active
Most Active: user42

# Errored Users
Errored Users: user13
```

---

## 要求される Java 技術
- 標準入力の読み取り (`Scanner` など)
- `Map` / `Set` の利用
- 文字列処理（`split()`）
- 集計ロジックの実装
- 可読性の高い出力整形
