import pandas as pd
import mysql.connector as connector
from mysql.connector import errorcode


class MySQLdb:
    def __init__(self):
        self.db = None

    def start_connection(self, user, password, database):
        """
        Start mysql connection
        :return: itself
        """
        try:
            self.db = connector.connect(user=user, password=password, database=database)
        except connector.Error as err:
            if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
                print("Something is wrong with your user name or password")
            if err.errno == errorcode.ER_BAD_DB_ERROR:
                print("Database does not exist")
            else:
                print(err)
        return self

    def read_db(self, query):
        return pd.read_sql(query, self.db)

    def close_connection(self):
        self.db.close()
