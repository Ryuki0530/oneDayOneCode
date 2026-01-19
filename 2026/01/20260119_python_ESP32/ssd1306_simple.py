# ssd1306_simple.py
# SSD1306 I2C minimal driver (no external deps except framebuf)
#
# 対応: 128x64 or 128x32 (一般的)
# address: 0x3C / 0x3D

import framebuf
from machine import I2C


class SSD1306_I2C:
    def __init__(self, width: int, height: int, i2c: I2C, address: int = 0x3C):
        self.width = width
        self.height = height
        self.i2c = i2c
        self.address = address
        self.pages = self.height // 8
        self.buffer = bytearray(self.width * self.pages)
        self.fb = framebuf.FrameBuffer(self.buffer, self.width, self.height, framebuf.MONO_VLSB)
        self._init_display()

    def _write_cmd(self, cmd: int) -> None:
        # control byte 0x00 = command
        self.i2c.writeto(self.address, bytes([0x00, cmd]))

    def _write_data(self, buf: bytes) -> None:
        # control byte 0x40 = data
        self.i2c.writeto(self.address, b"\x40" + buf)

    def _init_display(self) -> None:
        # Generic SSD1306 init sequence
        self._write_cmd(0xAE)  # display off
        self._write_cmd(0x20)  # set memory addressing mode
        self._write_cmd(0x00)  # horizontal addressing mode
        self._write_cmd(0xB0)  # page start
        self._write_cmd(0xC8)  # COM scan direction remap
        self._write_cmd(0x00)  # low column start
        self._write_cmd(0x10)  # high column start
        self._write_cmd(0x40)  # start line
        self._write_cmd(0x81)  # contrast
        self._write_cmd(0x7F)
        self._write_cmd(0xA1)  # segment re-map
        self._write_cmd(0xA6)  # normal display
        self._write_cmd(0xA8)  # multiplex ratio
        self._write_cmd(self.height - 1)
        self._write_cmd(0xA4)  # output follows RAM
        self._write_cmd(0xD3)  # display offset
        self._write_cmd(0x00)
        self._write_cmd(0xD5)  # display clock
        self._write_cmd(0x80)
        self._write_cmd(0xD9)  # pre-charge period
        self._write_cmd(0xF1)
        self._write_cmd(0xDA)  # com pins
        self._write_cmd(0x12 if self.height == 64 else 0x02)
        self._write_cmd(0xDB)  # vcom detect
        self._write_cmd(0x20)
        self._write_cmd(0x8D)  # charge pump
        self._write_cmd(0x14)
        self._write_cmd(0xAF)  # display on

        self.fill(0)
        self.show()

    # FrameBuffer proxy methods
    def fill(self, c: int) -> None:
        self.fb.fill(c)

    def text(self, s: str, x: int, y: int, c: int = 1) -> None:
        self.fb.text(s, x, y, c)

    def pixel(self, x: int, y: int, c: int = 1) -> None:
        self.fb.pixel(x, y, c)

    def rect(self, x: int, y: int, w: int, h: int, c: int = 1) -> None:
        self.fb.rect(x, y, w, h, c)

    def fill_rect(self, x: int, y: int, w: int, h: int, c: int = 1) -> None:
        self.fb.fill_rect(x, y, w, h, c)

    def show(self) -> None:
        self._write_cmd(0x21)  # column address
        self._write_cmd(0)
        self._write_cmd(self.width - 1)
        self._write_cmd(0x22)  # page address
        self._write_cmd(0)
        self._write_cmd(self.pages - 1)

        # chunked send
        for i in range(0, len(self.buffer), 16):
            self._write_data(self.buffer[i : i + 16])
