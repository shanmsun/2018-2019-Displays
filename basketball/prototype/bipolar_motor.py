import RPi.GPIO as GPIO
import time

by_en = 13
by_r = 26
by_l = 19

rg_en = 17
rg_r = 27
rg_l = 22

out_pins = [by_en, by_l, by_r, rg_en, rg_r, rg_l]

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(out_pins, GPIO.OUT)

seq = [
    [1, 1, 0, 0, 0, 0],
    [0, 0, 0, 1, 1, 0],
    [1, 0, 1, 0, 0, 0],
    [0, 0, 0, 1, 0, 1]
    ]

i = 0
while abs(i) < 200:
    GPIO.output(out_pins, seq[abs(i%len(seq))])
    print(seq[i%len(seq)])
    i -= 1
    time.sleep(0.01)

