import pandas as pd
import numpy as np
import mysql.connector
from mysql.connector import errorcode


class Data:
    def __init__(self, dataframe):
        self.df = dataframe
        self.no2 = []
        self.co = []
        self.co2 = []
        self.tvoc = []
        self.temp = []
        self.rh = []

    def initialize(self, drop=True, sort=True):
        """
        Initialize the data
        :param drop: drop unnecessary columns
        :param sort: sort the dataset by datetime
        :return: itself
        """
        if drop:
            self.df.drop(['measurementID', 'stationID', 'GPSlatitude', 'GPSlongitude', 'ZoneID'], inplace=True, axis=1)

        self.df['date'] = pd.to_datetime(self.df['date'])
        self.df['datetime'] = pd.to_datetime(self.df['time'] + self.df['date'])

        if drop:
            self.df.drop(['date', 'time'], inplace=True, axis=1)

        self.df.set_index('datetime', inplace=True)

        if sort:
            self.df.sort_values(by='datetime', inplace=True, ascending=True)
        return self

    def split(self):
        """
        Convert every column to numpy array and split it
        :return: itself
        """
        self.no2 = self.df['NO2'].values.reshape(-1, 1)
        self.co = self.df['CO'].values.reshape(-1, 1)
        self.co2 = self.df['CO2'].values.reshape(-1, 1)
        self.tvoc = self.df['TVOC'].values.reshape(-1, 1)
        self.temp = self.df['Temperature'].values.reshape(-1, 1)
        self.rh = self.df['Humidity'].values.reshape(-1, 1)

        return self
