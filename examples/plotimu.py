#!/usr/bin/python3
import time, sys, os, csv
from os import path
import pyqtgraph as pg
from PyQt6 import QtCore, QtWidgets

import esp32imu
sampling_freq = 500

At = []
Ax = []
Ay = []
Az = []
last_t_us = 0

# how many seconds of samples to keep in the plotting window?
SAMPLE_WINDOW_SEC = 5

def imu_cb(msg):
    global last_t_us
    dt = (msg.t_us - last_t_us) * 1e-6 # us to s
    last_t_us = msg.t_us
    hz = 1./dt
    print('Got IMU at {} us ({:.0f} Hz): {:.2f}, {:.2f}, {:.2f}, \t {:.2f}, {:.2f}, {:.2f}'
            .format(msg.t_us, hz,
                    msg.accel_x, msg.accel_y, msg.accel_z,
                    msg.gyro_x, msg.gyro_y, msg.gyro_z))

    if 0.1*sampling_freq < hz < 2*sampling_freq: # get rid of spurious stuff at start
        At.append(msg.t_us)
        Ax.append(msg.accel_x)
        Ay.append(msg.accel_y)
        Az.append(msg.accel_z)

    if len(At) > hz*SAMPLE_WINDOW_SEC:
      At.pop(0)
      Ax.pop(0)
      Ay.pop(0)
      Az.pop(0)

def main():
    ports = os.listdir("/dev/")
    cports = [f for f in ports if f.startswith("cu.usb")]
    try: 
        port = "/dev/"+cports[0]
        print(port)
    except:
        print("No ESP")
        os._exit(1)

    driver = esp32imu.SerialDriver(port, 115200)
    # driver = esp32imu.UDPDriver()
    time.sleep(0.1)
    # could use registerCallbackIMU_NoMag or registerCallbackIMU_3DOF
    driver.registerCallbackIMU(imu_cb)
    driver.sendRate(sampling_freq)

    app = QtWidgets.QApplication([])

    # https://pyqtgraph.readthedocs.io/en/latest/plotting.html#examples
    pw = pg.plot(title="Accelerometer")
    timer = pg.QtCore.QTimer()
    def update():
        pw.plot(At, Ax, pen='y', clear=True)
        pw.plot(At, Ay, pen='r')
        pw.plot(At, Az, pen='c')
        app.processEvents()

    timer.timeout.connect(update)
    timer.start(50) # ms
    
    # Block on application window
    app.exec()

    # clean up to prevent error or resource deadlock
    driver.unregisterCallbacks()

if __name__ == '__main__':
    main()