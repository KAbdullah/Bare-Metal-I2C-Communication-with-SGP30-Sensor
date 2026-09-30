import serial
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from collections import deque
import datetime

import matplotlib
matplotlib.use('TkAgg')

ser = serial.Serial('/dev/ttyACM0', 115200, 8, timeout=1)

#allow real time data
plt.ion()
fig, ax = plt.subplots()

#only allow 5 0points at a time, automatic pop
dataforplot = deque(maxlen=50)

def convert_decimal_to_CO2(value1, value2):
  ans = 0

  ans += value1 * 256
  ans += value2
  return ans


try:

  while True:
    if ser.in_waiting >= 3:
      data = ser.read(3)
      CO2_value = convert_decimal_to_CO2(data[0], data[1])
      current_time = datetime.datetime.now()
      print(f"Got bytes: 0x{data[0]:02x}, 0x{data[1]:02x}, 0x{data[2]:02x}, The CO2 value is {CO2_value:d}")

      dataforplot.append((current_time, CO2_value))

      times = [item[0] for item in dataforplot]
      co2 = [item[1] for item in dataforplot]

      ax.clear()
      ax.plot(times, co2, marker='*', color='r', linestyle='-', label='CO2 (ppm)')

      #format the time to display min:sec
      ax.xaxis.set_major_formatter(mdates.DateFormatter('%M:%S'))
      

      #The Y lim is from 0 to either 500 or whatever the highest CO2 value is
      ax.set_ylim(0, max(max(co2) * 1.2, 500))

      #Labeling the graph
      ax.set_title('Real-Time CO2 Monitoring')
      ax.set_xlabel('Time')
      ax.set_ylabel('CO2 Value (ppm)')
      ax.legend(loc='upper left')

      #Add a little delay in between to let the graph process new data
      plt.pause(0.01)

except KeyboardInterrupt:
  print("Closing Port...")

finally:
  ser.close()
  plt.ioff()
  plt.show()