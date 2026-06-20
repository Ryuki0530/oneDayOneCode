# OneDayOneCode

## Title
CAN Timeout Monitor

## 問題

CAN通信で受信しているECUが、一定時間メッセージを受信できなかった場合に
異常と判定する簡易監視プログラムを作成してください。

## 要件

1. 以下の配列を入力データとして使用する。

```c
int timestamps[] = {
    0, 100, 200, 300, 400,
    500, 600, 700, 1200, 1300
};
```

単位は ms とする。

2. 連続する受信時刻の差を計算する。

3. 差分が 300ms を超えた場合、

```text
TIMEOUT DETECTED
```

を出力する。

4. 最後に検出回数を表示する。

## 出力例

```text
TIMEOUT DETECTED
timeout_count = 1
```

## 制約

- 配列サイズは sizeof を使って求めること
- 関数化すること

## 推奨関数

```c
int detect_timeout(
    const int timestamps[],
    int size,
    int timeout_ms
);
```

## 学習ポイント

- 配列とループ
- 関数分割
- 組込みでよく使われるタイムアウト監視の考え方
- CAN受信監視の基本実装