import board
import pwmio
import time

servo = pwmio.PWMOut(board.GP1, variable_frequency=True)
servo.frequency = 50
servo.duty_cycle = 0

def set_angle(angle):
    level = 35 * angle + 900
    servo.duty_cycle = level

while True:
    for i in range(180):
        set_angle(i)
        time.sleep(0.01)
    for i in reversed(range(180)):
        set_angle(i)
        time.sleep(0.01)
