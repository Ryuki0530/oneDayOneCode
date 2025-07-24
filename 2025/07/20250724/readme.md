# 📝 Java課題：MySQLを使ったユーザー管理システム

## 📌 概要

この課題では、JavaからXAMPP上のMySQLに接続し、簡易的なユーザー管理システムを構築します。  
ユーザーの登録と一覧表示をコンソールで行えるようにしてください。

---

## 🛠 前提環境

- OS: Windows 11  
- エディタ: Visual Studio Code  
- データベース: XAMPPのMySQL（`localhost:3306`）  
- Java開発環境が整っていること  
- JDBCドライバ：`mysql-connector-j-8.x.x.jar` をプロジェクトに追加  

---

## 🗃 データベース準備

以下のSQL文を実行して、データベースとテーブルを作成してください。

```
CREATE DATABASE user_db CHARACTER SET utf8mb4;

USE user_db;

CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100),
    email VARCHAR(100),
    age INT
);
```

---

## 🔧 実装要件

### 1. DB接続クラス `DBManager.java`

- MySQLへの接続処理を行う  
- 接続情報はハードコーディングでも可  
- JDBC URL例：  
  `jdbc:mysql://localhost:3306/user_db?useSSL=false&serverTimezone=UTC`

---

### 2. ユーザー登録クラス `UserRegistrar.java`

- コンソールから「名前」「メール」「年齢」を入力し、`users` テーブルに登録する

#### 実行例：

```
== ユーザー登録 ==
名前を入力してください: 山田太郎
メールを入力してください: yamada@example.com
年齢を入力してください: 25
登録しました。
```

---

### 3. ユーザー表示クラス `UserLister.java`

- 登録されている全ユーザー情報を一覧表示する

#### 実行例：

```
== ユーザー一覧 ==
ID: 1 | 名前: 山田太郎 | メール: yamada@example.com | 年齢: 25
ID: 2 | 名前: 佐藤花子 | メール: sato@example.com | 年齢: 30
```

---

## 💡 発展課題（余裕があれば）

- 入力値のバリデーション（例：メール形式、年齢が0以上か）
- ユーザー削除機能（ID指定で削除）
- Swingを使ってGUI化

---

## 📁 提出物（任意）

- `DBManager.java`
- `UserRegistrar.java`
- `UserLister.java`
- 作成したSQLファイル（オプション）

---

## ✅ 学べること

- JavaとMySQLの連携（JDBC）  
- データベースへのCRUD操作  
- 入出力形式の設計  
- 複数ファイルに分けたJavaアプリケーションの構成
