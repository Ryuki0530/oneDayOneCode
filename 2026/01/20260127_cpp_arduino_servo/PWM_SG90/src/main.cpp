// src/main.cpp
// 目的：Arduino Uno(ATmega328P/16MHz)で Timer1 のハードPWM(OC1A=D9)を使い、サーボ(50Hz)を制御する。
// 制約：Servo.h 禁止 / delay()で周期生成禁止（=タイマで50Hz）
//
// 配線：サーボ信号線 -> D9(OC1A)
//      サーボVCC -> 5V（※サーボによっては別電源推奨）
//      サーボGND -> GND（別電源ならGND共通）
//
// シリアル：115200
//  コマンド：
//   A <angle>   (0..180) 例: A 90
//   S <min_us> <max_us> 例: S 900 2100
//  応答：OK ... / ERR

#include <Arduino.h>
#include <avr/io.h>
#include <stdlib.h>
#include <ctype.h>

static int g_min_us = 1000;
static int g_max_us = 2000;

static inline int clamp_int(int x, int lo, int hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

// Timer1: prescaler=8 -> 1 tick = 0.5us
// 20ms = 20000us -> TOP(ICR1)=40000
static void pwm_init_timer1_servo_50hz_oc1a() {
  // D9 = PB1 = OC1A
  DDRB |= (1 << DDB1);  // pinMode(9, OUTPUT) 相当

  // Timer1停止
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  // Fast PWM, TOP=ICR1 (Mode 14: WGM13:0 = 1110)
  // WGM11=1, WGM12=1, WGM13=1
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13);

  // OC1A non-inverting (clear on compare match, set at BOTTOM)
  TCCR1A |= (1 << COM1A1);

  // TOP = 40000 ticks (20ms)
  ICR1 = 40000;

  // 初期は中央 1500us -> 3000 ticks
  OCR1A = 3000;

  // prescaler = 8 (CS11=1)
  TCCR1B |= (1 << CS11);
}

static void set_pulse_us(int us) {
  // 0.5us刻み -> ticks = us * 2
  us = clamp_int(us, 500, 2500);  // 極端値の安全弁（サーボ破壊回避）
  uint16_t ticks = (uint16_t)(us * 2);

  // TOP=40000なので範囲内に収める
  if (ticks < 1) ticks = 1;
  if (ticks > (uint16_t)(ICR1 - 1)) ticks = (uint16_t)(ICR1 - 1);

  OCR1A = ticks;
}

static void set_angle_deg(int angle) {
  angle = clamp_int(angle, 0, 180);

  // 線形補間：min_us + angle*(max_us-min_us)/180
  long span = (long)g_max_us - (long)g_min_us;
  long us = (long)g_min_us + (long)angle * span / 180L;

  set_pulse_us((int)us);
}

static bool parse_int(const char* s, long* out) {
  // 先頭の空白を飛ばす
  while (*s && isspace((unsigned char)*s)) s++;

  if (!*s) return false;

  char* endp = nullptr;
  long v = strtol(s, &endp, 10);
  if (endp == s) return false; // 1文字も読めてない

  // 残りが「空白だけ」ならOK（改行はここに来る前に潰してる想定）
  while (*endp) {
    if (!isspace((unsigned char)*endp)) return false;
    endp++;
  }

  *out = v;
  return true;
}

static void rstrip_newline(char* line) {
  size_t n = strlen(line);
  while (n > 0 && (line[n - 1] == '\n' || line[n - 1] == '\r')) {
    line[n - 1] = '\0';
    n--;
  }
}

static void handle_command_line(char* line) {
  rstrip_newline(line);

  // 空行無視
  const char* p = line;
  while (*p && isspace((unsigned char)*p)) p++;
  if (!*p) return;

  // コマンド1文字目
  char cmd = (char)toupper((unsigned char)*p);
  p++;

  if (cmd == 'A') {
    // A <angle>
    long angle_l;
    if (!parse_int(p, &angle_l)) {
      Serial.println("ERR");
      return;
    }
    if (angle_l < 0 || angle_l > 180) {
      Serial.println("ERR");
      return;
    }
    set_angle_deg((int)angle_l);
    Serial.print("OK ");
    Serial.println((int)angle_l);
    return;
  }

  if (cmd == 'S') {
    // S <min_us> <max_us>
    // ざっくり2整数を抜く（厳密パーサじゃなくても課題として十分）
    // 先頭空白スキップ
    while (*p && isspace((unsigned char)*p)) p++;
    if (!*p) { Serial.println("ERR"); return; }

    char* end1 = nullptr;
    long min_us_l = strtol(p, &end1, 10);
    if (end1 == p) { Serial.println("ERR"); return; }

    p = end1;
    while (*p && isspace((unsigned char)*p)) p++;
    if (!*p) { Serial.println("ERR"); return; }

    char* end2 = nullptr;
    long max_us_l = strtol(p, &end2, 10);
    if (end2 == p) { Serial.println("ERR"); return; }

    // 残りが空白のみチェック
    while (*end2) {
      if (!isspace((unsigned char)*end2)) { Serial.println("ERR"); return; }
      end2++;
    }

    // 妥当性チェック（安全弁込み）
    if (min_us_l < 500 || min_us_l > 2500) { Serial.println("ERR"); return; }
    if (max_us_l < 500 || max_us_l > 2500) { Serial.println("ERR"); return; }
    if (min_us_l >= max_us_l) { Serial.println("ERR"); return; }

    g_min_us = (int)min_us_l;
    g_max_us = (int)max_us_l;

    Serial.print("OK S ");
    Serial.print(g_min_us);
    Serial.print(" ");
    Serial.println(g_max_us);
    return;
  }

  Serial.println("ERR");
}

void setup() {
  Serial.begin(115200);

  pwm_init_timer1_servo_50hz_oc1a();

  Serial.println("READY");
  Serial.println("CMD: A <0..180>  or  S <min_us> <max_us>");
}

void loop() {

  static char buf[64];
  static size_t idx = 0;

  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\n' || c == '\r') {
      if (idx > 0) {
        buf[idx] = '\0';
        handle_command_line(buf);
        idx = 0;
      } else {}
    } else {
      if (idx < sizeof(buf) - 1) {
        buf[idx++] = c;
      } else {
        idx = 0;
        Serial.println("ERR");
      }
    }
  }
}
