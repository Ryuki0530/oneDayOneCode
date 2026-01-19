# i2c_scan.py
from machine import Pin, I2C
import time

SDA_PIN = 21
SCL_PIN = 22
I2C_ID = 0
I2C_FREQ = 100_000  # まずは低速で安定させる

i2c = I2C(I2C_ID, scl=Pin(SCL_PIN), sda=Pin(SDA_PIN), freq=I2C_FREQ)

print("I2C scan start...")
try:
    devices = i2c.scan()
    print("Found:", [hex(d) for d in devices])
except Exception as e:
    print("Scan error:", repr(e))

time.sleep(1)
print("Done.")
