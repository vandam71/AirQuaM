__author__ = "Rafael Samorinha"
__version__ = "1.0.0"

import signal
import time
import matplotlib.pyplot as plt
import datetime
import pandas as pd
import numpy as np
from datetime import timedelta

from core.timer import Timer
from core.mysql import MySQLdb
from core.task import Job, ProgramKilled, signal_handler
from core.http import Http
from airquam.measurement import Measurement
from airquam.zone import Zone

pd.set_option('display.expand_frame_repr', False)  # Show the full DataFrame


def populate():
    sine = (np.sin(2 * np.pi * 5 * np.arange(100) / 100) + 1)

    http = Http()
    for i in range(100):
        datetimeObj = datetime.datetime.now()
        udate = datetimeObj.strftime("%Y-%m-%d")
        utime = datetimeObj.strftime("%H:%M:%S")
        try:
            data = {"stationID": 1,
                    "time": str(utime),
                    "date": str(udate),
                    "GPSlatitude": 41.53,
                    "GPSlongitude": -8.30,
                    "NO2": sine[i] * 10,
                    "CO": sine[i] * 100,
                    "CO2": sine[i] * 1000,
                    "TVOC": sine[i] * 10000,
                    "Temperature": sine[i] * 10,
                    "Humidity": sine[i] * 10
                    }
            http.post('http://airquam.herokuapp.com/data/measurement', data=data)
        except:
            pass
        time.sleep(0.1)


def program():
    timer = Timer('Program').start()
    con = MySQLdb(host='eu-cdbr-west-02.cleardb.net', user='bc2f6bc64cfba9', password='0e84f59c',
                  database='heroku_61f6ec79e99ef9a').start_connection()
    result = con.read_db('SELECT * FROM Zone')
    zones = []
    for i in range(result.shape[0]):
        zones.append(result['ZoneID'][i])
    for i in range(len(zones)):
        id = zones[i]
        df = con.read_db(f'SELECT * FROM Measurement WHERE ZoneID={id}')
        data = Measurement(df).initialize().split()
        zones[i] = Zone(id, data)
    zones[3].predict(retrain=False)

    fig, ax2 = plt.subplots(figsize=(8, 4))
    plt.plot(zones[3].data.temp, color='blue', label="True")
    ax2.plot(range(len(zones[3].data.temp), len(zones[3].data.temp) + len(zones[3].prediction.temp)),
             zones[3].prediction.temp, color='red',
             label="Predicted")
    plt.legend()
    plt.show()

    timer.stop()
    plt.show()
    con.close_connection()


def main():
    signal.signal(signal.SIGTERM, signal_handler)
    signal.signal(signal.SIGINT, signal_handler)
    job = Job(interval=timedelta(days=1), execute=program)
    job.start()

    while True:
        try:
            time.sleep(1)
        except ProgramKilled:
            print("Program Killed: running cleanup code")
            job.stop()
            break


if __name__ == '__main__':
    program()
