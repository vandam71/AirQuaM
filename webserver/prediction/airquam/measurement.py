import pandas as pd


class Measurement:
    def __init__(self, dataframe):
        self.df = dataframe
        self.no2 = None
        self.co = None
        self.co2 = None
        self.tvoc = None
        self.temp = None
        self.rh = None

    def initialize(self, drop=True, sort=True):
        """Initialize the data
        :param drop: drop unnecessary columns
        :param sort: sort the data set by datetime
        :return: itself
        """
        self.df.drop(['measurementID', 'stationID', 'GPSlatitude', 'GPSlongitude', 'ZoneID'], inplace=True,
                     axis=1) if drop else self.df
        self.df['date'] = pd.to_datetime(self.df['date'])
        self.df['datetime'] = pd.to_datetime(self.df['time'] + self.df['date'])
        self.df.drop(['date', 'time'], inplace=True, axis=1) if drop else self.df
        self.df.set_index('datetime', inplace=True)
        self.df.sort_values(by='datetime', inplace=True, ascending=True) if sort else self.df
        return self

    def split(self):
        """Convert every column to numpy array and split it
        :return: itself
        """
        self.no2 = self.df['NO2'].values.reshape(-1, 1)
        self.co = self.df['CO'].values.reshape(-1, 1)
        self.co2 = self.df['CO2'].values.reshape(-1, 1)
        self.tvoc = self.df['TVOC'].values.reshape(-1, 1)
        self.temp = self.df['Temperature'].values.reshape(-1, 1)
        self.rh = self.df['Humidity'].values.reshape(-1, 1)
        return self
