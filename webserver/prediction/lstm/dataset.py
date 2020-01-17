import numpy as np
from sklearn.preprocessing import MinMaxScaler


class Dataset:
    def __init__(self, values):
        """Dataset Initialization
        :param values: that will be used for prediction
        """
        self.values = values
        self.scaler = MinMaxScaler(feature_range=(0, 1))
        self.x_train = []
        self.y_train = []
        self.x_test = []
        self.y_test = []

    def create_dataset(self, days_prediction=24, future_days=24):
        """Create the x and y values, both for training and for testing
        :param days_prediction: days to be used to predict future
        :param future_days: Days it predicts into the future
        """
        if self.values.shape[0] < days_prediction + future_days:
            print('Not enough values to predict')
        # Split the data set in the training set and test set
        train = np.array(self.values[:int(self.values.shape[0] - future_days)])
        test = np.array(self.values[int(self.values.shape[0] - future_days - days_prediction):])

        # Normalize data set in range [0-1]
        train = self.scaler.fit_transform(train)
        test = self.scaler.transform(test)

        # Create x, y variables for train and test
        for i in range(days_prediction, train.shape[0]):
            self.x_train.append(train[i - days_prediction: i, 0])
            self.y_train.append(train[i, 0])

        for i in range(days_prediction, test.shape[0]):
            self.x_test.append(test[i - days_prediction: i, 0])
            self.y_test.append(test[i, 0])

        # Transform to numpy arrays
        self.x_train = np.array(self.x_train)
        self.y_train = np.array(self.y_train)
        self.x_test = np.array(self.x_test)
        self.y_test = np.array(self.y_test)

        # Prepare data for neural network
        self.x_train = np.reshape(self.x_train, (self.x_train.shape[0], self.x_train.shape[1], 1))
        self.x_test = np.reshape(self.x_test, (self.x_test.shape[0], self.x_test.shape[1], 1))
        return self
