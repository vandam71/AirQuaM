import datetime as dt
import pandas as pd
from core.http import Http


class Prediction:
    def __init__(self, id):
        self.zone = id
        self.date = dt.date.today() + dt.timedelta(days=1)
        self.no2 = None
        self.co = None
        self.co2 = None
        self.tvoc = None
        self.temp = None
        self.rh = None

    def to_df(self):
        time = dt.datetime(self.date.year, self.date.month, self.date.day, 0, 0, 0, 0)
        df = pd.DataFrame(columns=['ZoneID', 'date', 'time', 'NO2', 'CO', 'CO2', 'TVOC', 'Temperature', 'Humidity'])
        for i in range(24):
            df = df.append(
                {'ZoneID': self.zone, 'date': self.date, 'time': time.strftime("%H:%M:%S"),
                 'NO2': int(self.no2.item(i)),
                 'CO': int(self.co.item(i)),
                 'CO2': int(self.co.item(i)),
                 'TVOC': int(self.tvoc.item(i)), 'Temperature': int(self.temp.item(i)),
                 'Humidity': int(self.rh.item(i))},
                ignore_index=True)
            time += dt.timedelta(hours=1)
        return df

    def add_to_db(self, df):
        http = Http()
        for i in range(24):
            try:
                data = {
                    "ZoneID": df.loc[i, 'ZoneID'],
                    "date": str((df.loc[i, 'date']).strftime("%Y-%m-%d")),
                    "time": str(df.loc[i, 'time']),
                    "NO2": df.loc[i, 'NO2'],
                    "CO": df.loc[i, 'CO'],
                    "CO2": df.loc[i, 'CO2'],
                    "TVOC": df.loc[i, 'TVOC']
                }
                http.post('http://airquam.herokuapp.com/prediction/post', data)
            except:
                pass
