import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf
import math
import os
from functions import SeriesToSupervised
from sklearn.preprocessing import LabelEncoder, MinMaxScaler
from sklearn.metrics import mean_squared_error
from datetime import datetime

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

#PARSE DATASET
def parse(x):
    return datetime.strptime(x, '%Y %m %d %H')

tempdata = pd.read_csv('pollution.csv', parse_dates=[['year', 'month', 'day', 'hour']], index_col=0, date_parser=parse)
tempdata.drop('No', axis=1, inplace=True)

tempdata.columns = ['Pollution', 'Dew Point', 'Temperature', 'Pressure', 'Wind Direction', 'Wind Speed', 'Snow', 'Rain']
tempdata.index.name = 'date'

tempdata['Pollution'].fillna(0, inplace=True)

tempdata = tempdata[24:]

print(tempdata.head(5))

tempdata.to_csv('pollution_parsed.csv')

#------------CODE START------------------

#load dataset
dataset = pd.read_csv('pollution_parsed.csv', header=0, index_col=0)
values = dataset.values

#integer encode direction
encoder = LabelEncoder()
values[:,4] = encoder.fit_transform(values[:,4])

#ensure all data is float
values = values.astype('float32')

#normalize features
scaler = MinMaxScaler(feature_range=(0,1))
scaled = scaler.fit_transform(values)

#specify the number of lag hours
n_hours = 3
n_features = 8
#frame as supervised learning
reframed = SeriesToSupervised(scaled, n_hours, 1)
print(reframed.shape)

#split into train and test sets
values = reframed.values
n_train_hours = 365 * 24
train = values[:n_train_hours, :]
test = values[n_train_hours:, :]

#split into input and outputs
n_obs = n_hours * n_features
train_X, train_y = train[:, :n_obs], train[:, -n_features]
test_X, test_y = test[:, :n_obs], test[:, -n_features]
print(train_X.shape, len(train_X), train_y.shape)

#reshape input to be 3D [samples, timesteps, features]
train_X = train_X.reshape((train_X.shape[0], n_hours, n_features))
test_X = test_X.reshape((test_X.shape[0], n_hours, n_features))
print(train_X.shape, train_y.shape, test_X.shape, test_y.shape)

#design neural network
model = tf.keras.models.Sequential()
model.add(tf.keras.layers.Dense(
    units = 10,
    input_shape=(train_X.shape[1], train_X.shape[2])
))
model.add(tf.keras.layers.LSTM(
    units=20,
    activation='relu',
))
model.add(tf.keras.layers.Dense(
    units=1
))
model.compile(
    loss='mae',
    optimizer='adam'
)

#fit neural network
history = model.fit(train_X, train_y, epochs=50, batch_size=72, validation_data=(test_X, test_y), verbose=1, shuffle=False)

#plot history
plt.plot(history.history['loss'], label='train')
plt.plot(history.history['val_loss'], label='test')
plt.legend()
plt.show()

#make a prediction
yhat = model.predict(test_X)
test_X = test_X.reshape((test_X.shape[0], n_obs))

#invert scaling for forecast
inv_yhat = np.concatenate((yhat, test_X[:, -7:]), axis=1)
inv_yhat = scaler.inverse_transform(inv_yhat)
inv_yhat = inv_yhat[:,0]

#invert scaling for actual
test_y = test_y.reshape((len(test_y), 1))
inv_y = np.concatenate((test_y, test_X[:, -7:]), axis=1)
inv_y = scaler.inverse_transform(inv_y)
inv_y = inv_y[:,0]

#calculate RMSE
rmse = math.sqrt(mean_squared_error(inv_y, inv_yhat))
print('Test RMSE: %.3f' % rmse)

plt.plot(inv_yhat[-100:100], label='Prediction')
plt.plot(inv_y[-100:], label='Real')
plt.legend()
plt.show()