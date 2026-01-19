# main.py
# ESP32 MicroPython: Wi-Fi + HTTP server + SSD1306 OLED + Servo PWM
#
# 配線:
#   OLED(I2C): SDA=GPIO21, SCL=GPIO22
#   Servo(PWM): GPIO16

from machine import Pin, I2C
import utime
import network

from ssd1306_simple import SSD1306_I2C
from servo import Servo

# webappは後でimport（例外をOLEDに出すため）


# =========================
# ユーザー設定（ここだけ編集）
# =========================
WIFI_SSID = "TP-Link_6E8E"
WIFI_PASS = "03488263"

# I2C (OLED)
SDA_PIN = 21
SCL_PIN = 22
I2C_ID = 0
I2C_FREQ = 400_000

# OLED
OLED_W = 128
OLED_H = 64
OLED_ADDR = 0x3C  # 0x3D の場合もある。映らない場合はここを変える。

# Servo
SERVO_PIN = 16

# HTTP
HTTP_PORT = 80  # 80でOK。気になる場合は 8080 にしても良い。


def make_oled():
    i2c = I2C(I2C_ID, scl=Pin(SCL_PIN), sda=Pin(SDA_PIN), freq=I2C_FREQ)
    oled = SSD1306_I2C(OLED_W, OLED_H, i2c, address=OLED_ADDR)
    return oled


def draw_oled_lines(oled, lines):
    oled.fill(0)
    y = 0
    for s in lines[:6]:
        oled.text(str(s)[:21], 0, y, 1)
        y += 10
    oled.show()


def wifi_connect(ssid, password, oled, timeout_ms=20_000):
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)

    attempt = 0
    while True:
        attempt += 1

        if not wlan.isconnected():
            try:
                wlan.disconnect()
            except Exception:
                pass

            draw_oled_lines(oled, ["Wi-Fi connecting", "SSID:", ssid[:16], "Try: {}".format(attempt)])

            try:
                wlan.connect(ssid, password)
            except Exception:
                draw_oled_lines(oled, ["Wi-Fi connect err", "retry..."])
                utime.sleep_ms(800)
                continue

            wait_start = utime.ticks_ms()
            while not wlan.isconnected():
                utime.sleep_ms(200)
                if utime.ticks_diff(utime.ticks_ms(), wait_start) > timeout_ms:
                    break

        if wlan.isconnected():
            return wlan.ifconfig()[0]

        utime.sleep_ms(600)


def main():
    oled = make_oled()
    draw_oled_lines(oled, ["ESP32 MicroPython", "OLED+Servo Web", "Booting..."])

    servo = Servo(SERVO_PIN)
    servo.write_angle(90)

    try:
        ip = wifi_connect(WIFI_SSID, WIFI_PASS, oled)
    except Exception as e:
        draw_oled_lines(oled, ["Wi-Fi failed", repr(e)])
        raise

    try:
        from webapp import run_server, DeviceState
    except Exception as e:
        draw_oled_lines(oled, ["Import failed", "webapp.py", repr(e)])
        raise

    state = DeviceState(
        ip=ip,
        ssid=WIFI_SSID,
        angle=90,
        start_ms=utime.ticks_ms(),
        last_action_ms=utime.ticks_ms(),
        oled=oled,
        servo=servo,
    )
    state.refresh_oled()

    run_server(state, port=HTTP_PORT)


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        # ここに来るのは致命的例外。REPLにも出るのでOLEDにだけ短く出す。
        # OLEDが生きていれば見える。
        try:
            oled = make_oled()
            draw_oled_lines(oled, ["FATAL", repr(e)])
        except Exception:
            pass
        raise
