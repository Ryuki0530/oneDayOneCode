# pyotp 使い方ドキュメント

## 概要
pyotp は、Google Authenticator 互換の TOTP（Time-based One-Time Password）を生成・検証するための Python ライブラリです。本ドキュメントでは、基礎的な使用方法・実装例・応用方法についてまとめます。

---

## 1. SECRET（秘密鍵）の生成

```python
import pyotp

secret = pyotp.random_base32()
print(secret)
```

TOTP は **secret + 現在時刻** を組み合わせることで毎回異なる6桁コードを生成します。

---

## 2. TOTP の生成

```python
totp = pyotp.TOTP(secret)
code = totp.now()
print(code)  # 6桁のTOTPコード
```

内部では HMAC-SHA1 と Unix 時刻（30秒単位の時刻スロット）が使用されます。

---

## 3. TOTP の検証

```python
user_input = "123456"
if totp.verify(user_input):
    print("OK")
else:
    print("INVALID")
```

デフォルトで ±30秒（1ステップ）程度の時間ズレは許容されます。

---

## 4. Google Authenticator 連携用の otpauth URL

Google Authenticator に登録するには次の URL を生成します。

```python
uri = totp.provisioning_uri(name="alice", issuer_name="MyService")
print(uri)
```

出力例：

```
otpauth://totp/alice?secret=JBSWY3DPEHPK3PXP&issuer=MyService
```

これを QR コード化すればスマホアプリで読み取れます。

---

## 5. ユーザー情報の保存（JSON）

```python
import json

users = {"alice": secret}
with open("users.json", "w") as f:
    json.dump(users, f, indent=2)
```

読み込み例：

```python
with open("users.json") as f:
    data = json.load(f)
secret = data["alice"]
```

---

## 6. CLI 構成例（推奨アーキテクチャ）

- `load_users()`
- `save_users(users)`
- `register_user(username)`
- `authenticate(username, code)`
- `main()`

関数単位で責務を分割することで、拡張性が高くなります。

---

## 7. 応用例
- Webサービスの二段階認証
- API トークンによるパスワードレスログイン
- IoTデバイスの認証プロトコル試作
- スクリプトへの簡易セキュリティ追加

---

## ライセンス
本ドキュメントは自由に使用できます。
