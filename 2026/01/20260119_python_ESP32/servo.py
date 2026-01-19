# servo.py
# ESP32 MicroPython PWM servo driver
# - 50Hz PWM
# - angle (0..180) -> pulse width (us) -> duty

from machine import Pin, PWM


class Servo:
    """
    一般的なホビーサーボ想定:
      50Hz (周期20ms)
      0°   :  500us
      90°  : 1500us
      180° : 2500us

    MicroPythonの環境差を吸収:
      - duty_ns() があればそれを優先
      - なければ duty_u16() で近似
    """

    def __init__(self, pin_no: int, freq: int = 50, min_us: int = 500, max_us: int = 2500):
        self.pwm = PWM(Pin(pin_no), freq=freq)
        self.freq = freq
        self.min_us = min_us
        self.max_us = max_us
        self.period_us = int(1_000_000 / freq)  # 20,000us at 50Hz

        # feature detect
        self._has_duty_ns = hasattr(self.pwm, "duty_ns")
        self._has_duty_u16 = hasattr(self.pwm, "duty_u16")

    def write_angle(self, deg: int) -> None:
        if deg < 0:
            deg = 0
        if deg > 180:
            deg = 180

        pulse_us = self.min_us + (self.max_us - self.min_us) * (deg / 180.0)

        if self._has_duty_ns:
            # us -> ns
            self.pwm.duty_ns(int(pulse_us * 1000))
            return

        # fallback: duty_u16 (0..65535)
        # duty ratio = pulse_us / period_us
        ratio = pulse_us / self.period_us
        duty_u16 = int(max(0, min(65535, ratio * 65535)))

        if self._has_duty_u16:
            self.pwm.duty_u16(duty_u16)
        else:
            # 古いAPI: duty(0..1023) の可能性
            duty_10 = int(max(0, min(1023, ratio * 1023)))
            self.pwm.duty(duty_10)

    def deinit(self) -> None:
        self.pwm.deinit()
