import pandas as pd
import mysql.connector as connector
from mysql.connector import errorcode


class MySQLdb:
    def __init__(self, user, password, database, host):
        self.db = None
        self.user = user
        self.password = password
        self.database = database
        self.host = host

    def start_connection(self):
        """Start mysql connection
        :return: itself
        """
        try:
            self.db = connector.connect(host=self.host, user=self.user, password=self.password, database=self.database)
        except connector.Error as err:
            if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
                print("Something is wrong with your user name or password")
            if err.errno == errorcode.ER_BAD_DB_ERROR:
                print("Database does not exist")
            else:
                print(err)
        return self

    def read_db(self, query):
        """Read from a database
        :param query: MySQL database query
        :return: string in the form of pandas data frame
        """
        return pd.read_sql(query, self.db)

    def write_sql(self, query):
        cursor = self.db.cursor()
        cursor.execute(query)
        self.db.commit()

    def close_connection(self):
        """Close connection to the database"""
        self.db.close()
