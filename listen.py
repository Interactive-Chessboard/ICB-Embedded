import serial
import time

ser = serial.Serial('/dev/ttyACM1', 115200, timeout=1)
time.sleep(1)  # allow ESP32-S3 to reset
print("Listening...")

while True:
    if ser.in_waiting > 0:
        print(ser.readline().decode(errors='replace').strip())
