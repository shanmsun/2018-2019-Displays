import RPi.GPIO as GPIO


enable = 18
in1 = 24
ctrl_pins = [enable, in1]
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

GPIO.setup(ctrl_pins, GPIO.OUT)

GPIO.output(ctrl_pins, 1)

print('hello world')

