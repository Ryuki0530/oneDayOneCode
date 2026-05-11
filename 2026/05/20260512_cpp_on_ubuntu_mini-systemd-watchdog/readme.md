# 今日の課題: Mini Systemd Watchdog

## 概要

Linux の `systemd` サービスを監視する C++ プログラムを作成してください。

指定されたサービス名について、一定間隔で状態を確認し、サービスが停止していた場合は再起動を試みます。

ただし、安全のため、再起動できるサービス名は `oodc-` で始まるものだけとします。

例:

```bash
./watchdog oodc-test.service 5
```

この場合、`oodc-test.service` を 5 秒ごとに監視します。

---

## 実行形式

```bash
./watchdog <service-name> <interval-sec>
```

---

## 入力

コマンドライン引数で以下を受け取る。

| 引数 | 内容 |
|---|---|
| service-name | 監視対象の systemd サービス名 |
| interval-sec | 監視間隔 秒 |

---

## 出力

以下のようなログを標準出力に表示してください。

```text
[INFO] target service: oodc-test.service
[INFO] interval: 5 sec
[CHECK] oodc-test.service is active
[CHECK] oodc-test.service is inactive
[ACTION] restart oodc-test.service
[RESULT] restart success
```

---

## 仕様

### 1. サービス状態の取得

`systemctl is-active <service-name>` を実行し、結果を判定する。

戻り値または出力を使って、以下を判定する。

| 出力 | 状態 |
|---|---|
| active | 起動中 |
| inactive | 停止中 |
| failed | 異常終了 |
| unknown | 存在しない可能性あり |

---

### 2. 再起動処理

サービスが `inactive` または `failed` の場合、以下を実行する。

```bash
systemctl restart <service-name>
```

ただし、サービス名が `oodc-` で始まらない場合は、再起動してはいけない。

```text
[WARN] restart skipped: service name must start with oodc-
```

と表示すること。

---

### 3. 監視回数

無限ループではなく、まずは 10 回だけ監視すること。

例:

```text
[INFO] check count: 1/10
[INFO] check count: 2/10
...
[INFO] check count: 10/10
[INFO] watchdog finished
```

---

### 4. エラー処理

以下の場合はエラーメッセージを表示して終了すること。

- 引数が足りない
- interval-sec が 1 未満
- service-name が `.service` で終わっていない
- systemctl コマンドの実行に失敗した

---

## 実行例

### 起動中の場合

```bash
./watchdog oodc-test.service 3
```

出力例:

```text
[INFO] target service: oodc-test.service
[INFO] interval: 3 sec
[INFO] check count: 1/10
[CHECK] oodc-test.service is active
[INFO] check count: 2/10
[CHECK] oodc-test.service is active
```

---

### 停止中の場合

```text
[INFO] check count: 3/10
[CHECK] oodc-test.service is inactive
[ACTION] restart oodc-test.service
[RESULT] restart success
```

---

## テスト用サービス例

以下のようなテストサービスを作成してよい。

```bash
sudo nano /etc/systemd/system/oodc-test.service
```

内容:

```ini
[Unit]
Description=One Day One Code Test Service

[Service]
Type=simple
ExecStart=/bin/sleep infinity
Restart=no

[Install]
WantedBy=multi-user.target
```

反映:

```bash
sudo systemctl daemon-reload
sudo systemctl start oodc-test.service
sudo systemctl status oodc-test.service
```

停止テスト:

```bash
sudo systemctl stop oodc-test.service
```

watchdog 実行:

```bash
sudo ./watchdog oodc-test.service 3
```

削除:

```bash
sudo systemctl stop oodc-test.service
sudo rm /etc/systemd/system/oodc-test.service
sudo systemctl daemon-reload
```

---

## 実装条件

- 言語: C++
- Linux ネイティブ環境で実行
- `system()` だけでなく、可能なら `popen()` を使って `systemctl is-active` の出力を取得すること
- `std::this_thread::sleep_for()` を使って監視間隔を作ること
- `std::string` でサービス名を扱うこと

---

## 追加課題

余裕があれば、以下も実装してください。

1. `--dry-run` オプション  
   再起動コマンドを実行せず、実行予定の内容だけ表示する。

2. ログファイル出力  
   `/tmp/oodc-watchdog.log` にログを追記する。

3. 現在の systemd target 表示  
   起動時に以下を実行して表示する。

```bash
systemctl get-default
```

出力例:

```text
[INFO] default target: graphical.target
```

---

## ゴール

C++ から Linux の systemd を操作し、サービス状態の取得・判定・再起動・ログ出力を行えるようになること。