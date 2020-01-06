import requests
from requests.exceptions import HTTPError


class Http:
    def __init__(self):
        self.url = None
        self.params = None
        self.response = None
        self.json = None

    def get(self, url=None, params=None):
        self.url = url if url is not None else self.url
        self.params = params if params is not None else self.params
        try:
            self.response = requests.get(url=self.url, params=self.params)
            self.response.raise_for_status()  # If the response was successful, no Exception will be raised
        except HTTPError as err:
            print(f'[HTTP] Error occurred: {err}')
        except Exception as err:
            print(f'[HTTP] Other error occurred: {err}')
        else:
            print(f'[HTTP] GET from {self.url} successful')
            self.response.encoding = 'utf-8'
            self.json = self.response.json()
        return self

    def post(self, url=None, data=None):
        self.url = url if url is not None else self.url
        try:
            self.response = requests.post(url=self.url, json=data)
            self.response.raise_for_status()  # If the response was successful, no Exception will be raised
        except HTTPError as err:
            print(f'[HTTP] Error occurred: {err}')
        except Exception as err:
            print(f'[HTTP] Other error occurred: {err}')
        else:
            print(f'[HTTP] POST at {self.url} successful')
            self.response.encoding = 'utf-8'
            self.json = self.response.json()
        return self

    def put(self, url=None, data=None):
        self.url = url if url is not None else self.url
        try:
            self.response = requests.put(url=self.url, data=data)
            self.response.raise_for_status()  # If the response was successful, no Exception will be raised
        except HTTPError as err:
            print(f'[HTTP] Error occurred: {err}')
        except Exception as err:
            print(f'[HTTP] Other error occurred: {err}')
        else:
            print(f'[HTTP] PUT at {self.url} successful')
            self.response.encoding = 'utf-8'
            self.json = self.response.json()
        return self

    def delete(self, url=None, data=None):
        self.url = url if url is not None else self.url
        try:
            self.response = requests.delete(url=self.url, data=data)
            self.response.raise_for_status()  # If the response was successful, no Exception will be raised
        except HTTPError as err:
            print(f'[HTTP] Error occurred: {err}')
        except Exception as err:
            print(f'[HTTP] Other error occurred: {err}')
        else:
            print(f'[HTTP] DELETE at {self.url} successful')
            self.response.encoding = 'utf-8'
            self.json = self.response.json()
        return self
