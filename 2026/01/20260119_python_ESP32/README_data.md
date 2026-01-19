# ESP32 MicroPython: SSD1306 + Servo + LAN Web Controller

## 配線
### OLED (I2C)
- SDA -> GPIO21
- SCL -> GPIO22
- VCC -> 3.3V（モジュールにより5V対応もあるが基本3.3V推奨）
- GND -> GND

### サーボ
- PWM(信号) -> GPIO16
- VCC -> 外部5V推奨（ESP32の3.3Vから取らない）
- GND -> ESP32 GND と共通（重要）

## ファイル
- main.py: 起動、Wi-Fi接続、サーバ開始
- webapp.py: HTTPハンドラ（GET /, GET /api/status, POST /api/angle）
- servo.py: サーボPWM制御
- ssd1306_simple.py: SSD1306 I2C簡易ドライバ

## 使い方
1. `main.py` の `WIFI_SSID`, `WIFI_PASS` を設定
2. 4ファイルをESP32に転送（Thonny等）
3. リセット
4. OLEDに表示された IP に、スマホのブラウザでアクセス
   - 例: http://192.168.0.10/

## API
- GET /api/status
  - JSONで状態を返す
- POST /api/angle
  - JSON: {"angle": 120}
  - またはフォーム: angle=120

## OLEDが映らない場合
- `main.py` の `OLED_ADDR` を 0x3C <-> 0x3D で切り替える
- SDA/SCLの配線確認（GPIO21/22）
