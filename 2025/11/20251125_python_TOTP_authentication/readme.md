# Python 課題：pyotp を用いた TOTP 認証システムの実装

## 概要
あなたのタスクは、TOTP（Time-based One-Time Password）を使った
簡易的な二段階認証プログラムを作成することである。

今回は **pyotp** ライブラリを用いて、
- 秘密鍵（secret）の生成
- ワンタイムパスワードの生成
- パスワードの確認（検証）
- 任意ユーザーの登録機能（secretを記録）
- 認証処理

以上を満たす CLI ベースの認証システムを実装せよ。

---

## 要件

### 1. ユーザー登録
- 新規ユーザー名を受け取り、TOTP 用の secret を生成して保存する。
- secret の保存形式は JSON ファイルでよい（例: `users.json`）。
- secret 生成には pyotp.random_base32() を使うこと。

### 2. QR コードの生成（任意）
- できれば、ユーザーが Google Authenticator に登録できるよう
  otpauth URL を生成し、qrcode ライブラリなどで QR コードを作成してもよい。
- 生成は任意。文字列出力だけでも可。

### 3. 認証処理
- 登録済みユーザー名を受け取り、現在時刻の TOTP を比較して認証する。
- 正しい場合は「ログイン成功」、違う場合は「失敗」と表示。

### 4. CLI 構成
以下のようなメニューを作ること：
```
1. register  
2. login  
3. exit
```
---

## 実行例
```
$ python totp_app.py

1. register
2. login
3. exit
:1

Enter username: ryuki
User 'ryuki' registered.
Secret: JBSWY3DPEHPK3PXP
Scan this URL in Google Authenticator:
otpauth://totp/ryuki?secret=JBSWY3DPEHPK3PXP&issuer=MyService
```
```
2
Enter username: ryuki
Enter TOTP: 123456
Login success!
```

---

## 提出物
- 課題実装の Python ファイル（1ファイルでよい）
- users.json（起動後に自動生成）
- 実行方法を README に簡単に記載せよ
