import serial

ser = serial.Serial('/dev/ttyACM0', 115200, 8, timeout=1)

def convert_decimal_to_CO2(value1, value2):
  ans = 0

  ans += value1 * 256
  ans += value2
  return ans


try:

  while True:
    if ser.in_waiting > 0:
      data = ser.read(3)
      CO2_value = convert_decimal_to_CO2(data[0], data[1])
      print(f"Got bytes: 0x{data[0]:02x}, 0x{data[1]:02x}, 0x{data[2]:02x}, The CO2 value is {CO2_value:d}")

except KeyboardInterrupt:
  print("Closing Port...")

finally:
  ser.close()