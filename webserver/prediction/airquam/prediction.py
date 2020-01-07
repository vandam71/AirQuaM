import datetime as dt
import pandas as pd


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
        time = None
        df = pd.DataFrame(columns=['ZoneID', 'date', 'time', 'NO2', 'CO', 'CO2', 'TVOC', 'Temperature', 'Humidity'])
        for i in range(24):
            time += dt.timedelta(hours=1)
            df = df.append(
                {'ZoneID': self.zone, 'date': self.date, 'time': time, 'NO2': self.no2[i], 'CO': self.co[i],
                 'CO2': self.co[i],
                 'TVOC': self.tvoc[i], 'Temperature': self.temp[i], 'Humidity': self.rh[i]}, ignore_index=True)
        return df

    def add_to_db(self, db):
        db.write_sql(f'INSERT INTO Prediction (ZoneID, time, date, NO2, CO, CO2, TVOC')

