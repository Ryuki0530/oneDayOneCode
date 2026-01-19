# webapp.py
# 超軽量HTTPサーバ: GET /, GET /api/status, POST /api/angle
# - フォーム(application/x-www-form-urlencoded) でも JSON(application/json) でも angle を受け取れる
# - 状態をOLEDに表示
#
# NOTE:
#   MicroPython(ESP32)には標準で typing が無いことが多いので、
#   typing.Optional 等は使わない（ImportError回避）。

import socket
import ujson
import utime

from ssd1306_simple import SSD1306_I2C
from servo import Servo


HTML_PAGE = """<!doctype html>
<html lang="ja">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <title>ESP32 Servo Controller</title>
  <style>
    body {{ font-family: system-ui, sans-serif; padding: 16px; }}
    .card {{ border: 1px solid #ddd; border-radius: 12px; padding: 12px; max-width: 520px; }}
    .row {{ display: flex; gap: 12px; align-items: center; flex-wrap: wrap; }}
    input[type=range] {{ width: 260px; }}
    button {{ padding: 10px 12px; border-radius: 10px; border: 1px solid #ccc; background: #f7f7f7; }}
    code {{ background: #f2f2f2; padding: 2px 6px; border-radius: 6px; }}
    .small {{ color: #444; font-size: 13px; }}
  </style>
</head>
<body>
  <div class="card">
    <h2>ESP32 サーボコントローラ</h2>
    <div class="small">このページはESP32がLAN内で配信しています。</div>
    <p>Angle: <strong id="angleText">--</strong>°</p>

    <div class="row">
      <input id="slider" type="range" min="0" max="180" step="1" value="90" />
      <button onclick="setAngle(0)">0°</button>
      <button onclick="setAngle(90)">90°</button>
      <button onclick="setAngle(180)">180°</button>
    </div>

    <p class="small">
      IP: <code id="ipText">--</code> /
      Uptime: <code id="upText">--</code>s
    </p>
    <p class="small">
      API: <code>/api/status</code> / <code>POST /api/angle</code>
    </p>
  </div>

<script>
async function fetchStatus() {{
  try {{
    const r = await fetch('/api/status', {{ cache: 'no-store' }});
    const j = await r.json();
    document.getElementById('angleText').textContent = j.angle;
    document.getElementById('ipText').textContent = j.ip;
    document.getElementById('upText').textContent = j.uptime_sec;
    const s = document.getElementById('slider');
    if (Math.abs(parseInt(s.value) - j.angle) > 2) s.value = j.angle;
  }} catch (e) {{
    // ignore
  }}
}}

async function postAngle(a) {{
  await fetch('/api/angle', {{
    method: 'POST',
    headers: {{ 'Content-Type': 'application/json' }},
    body: JSON.stringify({{ angle: a }})
  }});
}}

function setAngle(a) {{
  const v = Math.max(0, Math.min(180, a|0));
  document.getElementById('slider').value = v;
  postAngle(v).then(fetchStatus);
}}

document.getElementById('slider').addEventListener('input', (ev) => {{
  document.getElementById('angleText').textContent = ev.target.value;
}});

document.getElementById('slider').addEventListener('change', (ev) => {{
  setAngle(parseInt(ev.target.value));
}});

fetchStatus();
setInterval(fetchStatus, 1000);
</script>
</body>
</html>
"""


class DeviceState:
    def __init__(
        self,
        ip,
        ssid,
        angle,
        start_ms,
        last_action_ms,
        oled,
        servo,
    ):
        self.ip = ip
        self.ssid = ssid
        self.angle = angle
        self.start_ms = start_ms
        self.last_action_ms = last_action_ms
        self.oled = oled
        self.servo = servo

    def uptime_sec(self):
        return int(utime.ticks_diff(utime.ticks_ms(), self.start_ms) // 1000)

    def last_action_ago_sec(self):
        return int(utime.ticks_diff(utime.ticks_ms(), self.last_action_ms) // 1000)

    def set_angle(self, angle):
        # clamp
        if angle < 0:
            angle = 0
        if angle > 180:
            angle = 180

        self.angle = angle
        self.last_action_ms = utime.ticks_ms()

        # 物理出力
        self.servo.write_angle(angle)

        # 表示更新
        self.refresh_oled()

    def refresh_oled(self):
        o = self.oled
        o.fill(0)
        o.text("ESP32 Servo Web", 0, 0, 1)
        o.text("IP:", 0, 12, 1)
        o.text(self.ip[:16], 24, 12, 1)
        o.text("Angle:{:3d}".format(self.angle), 0, 26, 1)
        o.text("Up:{:5d}s".format(self.uptime_sec()), 0, 40, 1)
        o.text("Last:{:4d}s".format(self.last_action_ago_sec()), 0, 52, 1)

        # バー表示（角度）
        bar_w = int((self.angle / 180) * 40)
        o.rect(84, 26, 44, 8, 1)
        o.fill_rect(85, 27, max(0, min(42, bar_w)), 6, 1)

        o.show()


def _read_until(sock, marker, max_len=4096):
    data = b""
    while marker not in data:
        chunk = sock.recv(512)
        if not chunk:
            break
        data += chunk
        if len(data) > max_len:
            break
    return data


def _parse_request(raw):
    # returns: method, path, headers(dict), body(bytes)
    try:
        head, body = raw.split(b"\r\n\r\n", 1)
    except ValueError:
        head = raw
        body = b""

    lines = head.split(b"\r\n")
    if not lines:
        return None, None, {}, b""

    # request line: METHOD PATH HTTP/1.1
    try:
        parts = lines[0].decode().split()
        method = parts[0]
        path = parts[1]
    except Exception:
        return None, None, {}, b""

    headers = {}
    for line in lines[1:]:
        if b":" in line:
            k, v = line.split(b":", 1)
            headers[k.decode().strip().lower()] = v.decode().strip()

    return method, path, headers, body


def _parse_form_urlencoded(body):
    # minimal parser: key=value&key=value
    out = {}
    try:
        s = body.decode()
    except Exception:
        return out
    pairs = s.split("&")
    for p in pairs:
        if "=" in p:
            k, v = p.split("=", 1)
            out[_url_decode(k)] = _url_decode(v)
    return out


def _url_decode(s):
    # minimal URL decode: + and %xx
    s = s.replace("+", " ")
    res = ""
    i = 0
    while i < len(s):
        c = s[i]
        if c == "%" and i + 2 < len(s):
            hx = s[i + 1 : i + 3]
            try:
                res += chr(int(hx, 16))
                i += 3
                continue
            except Exception:
                pass
        res += c
        i += 1
    return res


def _json_response(conn, code, obj):
    body = ujson.dumps(obj).encode()
    conn.send(
        "HTTP/1.1 {} OK\r\nContent-Type: application/json\r\nContent-Length: {}\r\nConnection: close\r\n\r\n".format(
            code, len(body)
        ).encode()
    )
    conn.send(body)


def _text_response(conn, code, text, content_type="text/plain; charset=utf-8"):
    body = text.encode()
    conn.send(
        "HTTP/1.1 {} OK\r\nContent-Type: {}\r\nContent-Length: {}\r\nConnection: close\r\n\r\n".format(
            code, content_type, len(body)
        ).encode()
    )
    conn.send(body)


def _html_response(conn, html):
    _text_response(conn, 200, html, content_type="text/html; charset=utf-8")


def _read_body_if_needed(conn, headers, already_body):
    # Content-Lengthがあれば、その分だけ読む（already_bodyに含まれてる分を考慮）
    cl = headers.get("content-length")
    if not cl:
        return already_body

    try:
        total = int(cl)
    except Exception:
        return already_body

    body = already_body
    while len(body) < total:
        chunk = conn.recv(min(512, total - len(body)))
        if not chunk:
            break
        body += chunk
    return body


def _handle(conn, state):
    raw = _read_until(conn, b"\r\n\r\n", max_len=4096)
    if not raw:
        return

    method, path, headers, body = _parse_request(raw)
    if method is None:
        _json_response(conn, 400, {"ok": False, "error": "bad request"})
        return

    body = _read_body_if_needed(conn, headers, body)

    # routing
    if method == "GET" and path == "/":
        _html_response(conn, HTML_PAGE)
        return

    if method == "GET" and path.startswith("/api/status"):
        _json_response(
            conn,
            200,
            {
                "ok": True,
                "ip": state.ip,
                "ssid": state.ssid,
                "angle": state.angle,
                "uptime_sec": state.uptime_sec(),
                "last_action_ago_sec": state.last_action_ago_sec(),
            },
        )
        return

    if method == "POST" and path.startswith("/api/angle"):
        ctype = headers.get("content-type", "")
        angle_val = None

        if "application/json" in ctype:
            try:
                data = ujson.loads(body.decode() if body else "{}")
                if "angle" in data:
                    angle_val = int(data["angle"])
            except Exception:
                angle_val = None
        else:
            # フォーム想定
            form = _parse_form_urlencoded(body)
            if "angle" in form:
                try:
                    angle_val = int(form["angle"])
                except Exception:
                    angle_val = None

        if angle_val is None:
            _json_response(conn, 400, {"ok": False, "error": "angle missing"})
            return

        state.set_angle(angle_val)
        _json_response(conn, 200, {"ok": True, "angle": state.angle})
        return

    # not found
    _json_response(conn, 404, {"ok": False, "error": "not found", "path": path})


def run_server(state, port=80):
    addr = socket.getaddrinfo("0.0.0.0", port)[0][-1]
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(addr)
    s.listen(2)

    # OLEDにサーバ情報
    state.refresh_oled()

    while True:
        try:
            conn, _ = s.accept()
            conn.settimeout(2.5)
            try:
                _handle(conn, state)
            finally:
                try:
                    conn.close()
                except Exception:
                    pass
        except Exception:
            # ループ継続（落ちないのが大事）
            utime.sleep_ms(50)
