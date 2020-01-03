__author__ = "Rafael Samorinha"
__version__ = "1.0.0"

import json
import signal
import time
import pandas as pd
from datetime import timedelta

from core.mysql import MySQLdb
from core.data import Data
from core.task import Job, ProgramKilled, signal_handler
from core.timer import Timer
from core.http import Http

pd.set_option('display.expand_frame_repr', False)   # Show the full DataFrame

ZONE = 1


def program():
    timer = Timer('HTTP').start()
    http = Http()
    http.delete(url='http://localhost:3000/data/station', data={
        "stationID": 2,
    })
    timer.stop()

    """
    con = MySQLdb('root', '1234', 'airquam').start_connection()
    df = con.read_db(f'SELECT * FROM Measurement WHERE ZoneID={ZONE}')
    data = Data(df).initialize().split()
    print(data.df.values)
    con.close_connection()
    """


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
