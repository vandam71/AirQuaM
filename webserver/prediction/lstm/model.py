import os
from core.timer import Timer
from keras.models import Sequential, load_model
from keras.layers import Dense, Dropout, LSTM
from keras.callbacks import EarlyStopping, ModelCheckpoint


class Model:
    """Class for building and using an LSTM model"""

    def __init__(self):
        """Start the model as sequential"""
        self.model = Sequential()

    def load_model(self, id, el):
        """Load an existing trained model from file
        :param id: zone id
        :param el: element tag
        """
        timer = Timer('Model').start()
        if os.path.exists(f'prediction/{id}-{el}.h5'):
            print(f'[Model] Loading model from prediction/{id}-{el}.h5')
            self.model = load_model(f'prediction/{id}-{el}.h5')
        else:
            print(f'[Model] No trained file stored for Zone {id} - {el}')
        timer.stop()

    def build_model(self, x_train):
        """Build the model and compile it
        :param x_train: to check how many values it will have to train
        """
        timer = Timer('Model').start()
        self.model.add(LSTM(units=96, return_sequences=True, input_shape=(x_train.shape[1], 1)))
        self.model.add(Dropout(rate=0.2))
        self.model.add(LSTM(units=96, return_sequences=True))
        self.model.add(Dropout(rate=0.2))
        self.model.add(LSTM(units=96, return_sequences=True))
        self.model.add(Dropout(rate=0.2))
        self.model.add(LSTM(units=96))
        self.model.add(Dropout(rate=0.2))
        self.model.add(Dense(units=1))
        self.model.compile(loss='mean_squared_error', optimizer='adam')
        print('[Model] Model Compiled')
        timer.stop()

    def train(self, x_train, y_train, id, el, epochs=3, batch_size=32):
        """Train the neural network and save it
        :param x_train: 'x' values to have for a single data set
        :param y_train: next value which will be the predicted one
        :param id: zone id
        :param el: element tag
        :param epochs: number of epochs that it will have to run
        :param batch_size: size of values it will use for train at each time
        """
        timer = Timer('Model').start()
        print('[Model] Training Started')
        print(f'[Model] {epochs} epochs, {batch_size} batch size')
        callbacks = [
            EarlyStopping(monitor='loss', patience=2),
            ModelCheckpoint(filepath=f'prediction/{id}-{el}.h5', monitor='loss', save_best_only=True)
        ]
        self.model.fit(x_train, y_train, epochs=epochs, batch_size=batch_size, callbacks=callbacks, verbose=0)
        self.model.save(f'prediction/{id}-{el}.h5')
        print(f'[Model] Training completed, saved as prediction/{id}-{el}.h5')
        timer.stop()

    def evaluate(self, x_test, y_test):
        """Evaluate the train
        :param x_test: 'x' values used for testing
        :param y_test: value that is it supposed to predict
        """
        timer = Timer('Model').start()
        print('[Model] Evaluation:', self.model.evaluate(x_test, y_test))
        timer.stop()

    def predict(self, x_test, scaler):
        """Predict Next value
        :param x_test: 'x' values used for testing
        :param scaler: scaled used to normalize the set
        :return: returns predicted value
        """
        timer = Timer('Model').start()
        print('[Model] Predicting...')
        prediction = self.model.predict(x_test)
        prediction = scaler.inverse_transform(prediction)
        timer.stop()
        return prediction
