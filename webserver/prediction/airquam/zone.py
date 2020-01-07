from airquam.prediction import Prediction
from lstm.dataset import Dataset
from lstm.model import Model


class Zone:
    def __init__(self, id, data):
        self.id = id
        self.data = data
        self.prediction = Prediction(self.id)

    def predict(self, retrain=True):
        self.prediction.no2 = self.predict_no2(retrain)
        self.prediction.co = self.predict_co(retrain)
        self.prediction.co2 = self.predict_co2(retrain)
        self.prediction.tvoc = self.predict_tvoc(retrain)
        self.prediction.temp = self.predict_temp(retrain)
        self.prediction.rh = self.predict_rh(retrain)

    def predict_no2(self, retrain):
        dataset = Dataset(self.data.no2).create_dataset(days_prediction=24, split_proportion=0.80)
        model = Model()
        if retrain:
            model.build_model(dataset.x_train)
            model.train(dataset.x_train, dataset.y_train, self.id, 'no2', epochs=10)
        else:
            model.load_model(self.id, 'no2')
        prediction = model.predict(x_test=dataset.x_test, scaler=dataset.scaler)
        return prediction

    def predict_co(self, retrain):
        dataset = Dataset(self.data.co).create_dataset(days_prediction=24, split_proportion=0.80)
        model = Model()
        if retrain:
            model.build_model(dataset.x_train)
            model.train(dataset.x_train, dataset.y_train, self.id, 'co', epochs=10)
        else:
            model.load_model(self.id, 'co')
        prediction = model.predict(x_test=dataset.x_test, scaler=dataset.scaler)
        return prediction

    def predict_co2(self, retrain):
        dataset = Dataset(self.data.co2).create_dataset(days_prediction=24, split_proportion=0.80)
        model = Model()
        if retrain:
            model.build_model(dataset.x_train)
            model.train(dataset.x_train, dataset.y_train, self.id, 'co2', epochs=10)
        else:
            model.load_model(self.id, 'co2')
        prediction = model.predict(x_test=dataset.x_test, scaler=dataset.scaler)
        return prediction

    def predict_tvoc(self, retrain):
        dataset = Dataset(self.data.tvoc).create_dataset(days_prediction=24, split_proportion=0.80)
        model = Model()
        if retrain:
            model.build_model(dataset.x_train)
            model.train(dataset.x_train, dataset.y_train, self.id, 'tvoc', epochs=10)
        else:
            model.load_model(self.id, 'tvoc')
        prediction = model.predict(x_test=dataset.x_test, scaler=dataset.scaler)
        return prediction

    def predict_temp(self, retrain):
        dataset = Dataset(self.data.temp).create_dataset(days_prediction=24, split_proportion=0.80)
        model = Model()
        if retrain:
            model.build_model(dataset.x_train)
            model.train(dataset.x_train, dataset.y_train, self.id, 'temp', epochs=10)
        else:
            model.load_model(self.id, 'temp')
        prediction = model.predict(x_test=dataset.x_test, scaler=dataset.scaler)
        return prediction

    def predict_rh(self, retrain):
        dataset = Dataset(self.data.rh).create_dataset(days_prediction=24, split_proportion=0.80)
        model = Model()
        if retrain:
            model.build_model(dataset.x_train)
            model.train(dataset.x_train, dataset.y_train, self.id, 'rh', epochs=10)
        else:
            model.load_model(self.id, 'rh')
        prediction = model.predict(x_test=dataset.x_test, scaler=dataset.scaler)
        return prediction
