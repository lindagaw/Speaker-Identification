import tensorflow as tf

from keras.callbacks import ModelCheckpoint, EarlyStopping
from keras.constraints import maxnorm
from keras.layers import Convolution1D, Dense, MaxPooling1D, Flatten, Add, Dropout, Input, Activation
from keras.layers import TimeDistributed, Bidirectional, LSTM, LeakyReLU
from keras.models import Sequential
from keras import optimizers, regularizers
from keras.utils import np_utils, to_categorical
from keras.models import Model, load_model, Sequential
from keras.regularizers import l2

import keras

from IPython.display import clear_output
from tensorflow.python.client import device_lib
from tensorflow.python.keras import backend
import tensorflow as tf


print("Num GPUs Available: ", len(tf.config.experimental.list_physical_devices('GPU')))
tf.keras.backend.clear_session()
tf.compat.v1.reset_default_graph()

def mil_squared_error(y_true, y_pred):
    return tf.keras.backend.square(tf.keras.backend.max(y_pred) - tf.keras.backend.max(y_true))

adam = tf.keras.optimizers.Adam(learning_rate=1e-5)

model = tf.keras.models.load_model('..//models//cnn.hdf5', custom_objects={'mil_squared_error': mil_squared_error, 'adam': adam})