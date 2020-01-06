from airquam.prediction import Prediction
from lstm.dataset import Dataset
from lstm.model import Model
import matplotlib.pyplot as plt


class Zone:
    def __init__(self, id, data):
        self.id = id
        self.data = data
        self.prediction = Prediction(self.id)

    def predict(self):
        self.prediction.no2 = self.predict_no2()
        self.prediction.co = self.predict_co()
        self.prediction.co2 = self.predict_co2()
        self.prediction.tvoc = self.predict_tvoc()
        self.prediction.temp = self.predict_temp()
        self.prediction.rh = self.predict_rh()

    def predict_no2(self):
        dataset = Dataset(self.data.no2).create_dataset(days_prediction=50, split_proportion=0.80)
        model = Model()
        model.build_model(dataset.x_train)
        model.train(dataset.x_train, dataset.y_train, self.id, 'no2', epochs=10)
        model.evaluate(dataset.x_test, dataset.y_test)
        prediction = model.predict(x_test=dataset.x_test, scaler=dataset.scaler)
        return prediction

    def predict_co(self):
        dataset = Dataset(self.data.co).create_dataset(days_prediction=50, split_proportion=0.80)
        model = Model()
        model.build_model(dataset.x_train)
        model.train(dataset.x_train, dataset.y_train, self.id, 'co', epochs=10)
        model.evaluate(dataset.x_test, dataset.y_test)
        prediction = model.predict(x_test=dataset.x_test, scaler=dataset.scaler)
        return prediction

    def predict_co2(self):
        dataset = Dataset(self.data.co2).create_dataset(days_prediction=50, split_proportion=0.80)
        model = Model()
        model.build_model(dataset.x_train)
        model.train(dataset.x_train, dataset.y_train, self.id, 'co2', epochs=10)
        model.evaluate(dataset.x_test, dataset.y_test)
        prediction = model.predict(x_test=dataset.x_test, scaler=dataset.scaler)
        return prediction

    def predict_tvoc(self):
        dataset = Dataset(self.data.tvoc).create_dataset(days_prediction=50, split_proportion=0.80)
        model = Model()
        model.build_model(dataset.x_train)
        model.train(dataset.x_train, dataset.y_train, self.id, 'tvoc', epochs=10)
        model.evaluate(dataset.x_test, dataset.y_test)
        prediction = model.predict(x_test=dataset.x_test, scaler=dataset.scaler)
        return prediction

    def predict_temp(self):
        dataset = Dataset(self.data.temp).create_dataset(days_prediction=50, split_proportion=0.80)
        model = Model()
        model.build_model(dataset.x_train)
        model.train(dataset.x_train, dataset.y_train, self.id, 'temp', epochs=10)
        model.evaluate(dataset.x_test, dataset.y_test)
        prediction = model.predict(x_test=dataset.x_test, scaler=dataset.scaler)
        return prediction

    def predict_rh(self):
        dataset = Dataset(self.data.rh).create_dataset(days_prediction=50, split_proportion=0.80)
        model = Model()
        model.build_model(dataset.x_train)
        model.train(dataset.x_train, dataset.y_train, self.id, 'rh', epochs=10)
        model.evaluate(dataset.x_test, dataset.y_test)
        prediction = model.predict(x_test=dataset.x_test, scaler=dataset.scaler)
        return prediction
