#!/usr/bin/env python
# coding: utf-8

# In[1]:


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

from extract_feat import extract_feats_single_wav
import os
import numpy as np

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


# In[30]:


def load_emp_miu(y):
    path = '..//models//emp_miu_class_' + str(y) + '.npy'
    return np.load(path, allow_pickle=True)

def load_inv_emp_covar():
    path = '..//models//inv_emp_sigma.npy'
    return np.load(path, allow_pickle=True)

def load_mahalanobis_mean(y):
    path = '..//models//mahalanobis_mean_class_' + str(y) + '.npy'
    return np.load(path, allow_pickle=True)

def load_mahalanobis_std(y):
    path = '..//models//mahalanobis_std_class_' + str(y) + '.npy'
    return np.load(path, allow_pickle=True)

def load_mahalanobis_coeff(y):
    path = '..//models//mahalanobis_threshold_coefficient_class_' + str(y) + '.npy'
    return np.load(path, allow_pickle=True)


# In[3]:


def is_in_distribution(x, predicted_y):
    
    assert(predicted_y == 0 or predicted_y == 1)
       
    emp_miu = load_emp_miu(predicted_y)
    inv_emp_sigma = load_inv_emp_covar()
    
    m_mean = load_mahalanobis_mean(predicted_y)
    m_std = load_mahalanobis_std(predicted_y)
    coeff = load_mahalanobis_coeff(predicted_y)
    
    upper = m_mean + coeff*m_std
    lower = m_mean - coeff*m_std
    
    penult_act = intermediate_layer_model.predict(x)

    try:
        m_dist = np.transpose(penult_act-emp_miu) @ inv_emp_sigma @ (penult_act-emp_miu)
    except:
        m_dist = (penult_act-emp_miu) * inv_emp_sigma * (penult_act-emp_miu)
    
    m_dist_norm = np.linalg.norm(m_dist)
    
    if lower < m_dist_norm and m_dist_norm < upper:
        return True
    else:
        return False


# In[4]:


print("Num GPUs Available: ", len(tf.config.experimental.list_physical_devices('GPU')))
tf.keras.backend.clear_session()
tf.compat.v1.reset_default_graph()

def mil_squared_error(y_true, y_pred):
    return tf.keras.backend.square(tf.keras.backend.max(y_pred) - tf.keras.backend.max(y_true))

adam = tf.keras.optimizers.Adam(learning_rate=1e-5)

model = tf.keras.models.load_model('..//models//cnn.hdf5', custom_objects={'mil_squared_error': mil_squared_error, 'adam': adam})
intermediate_layer_model = tf.keras.models.load_model('..//models//intermediate_layer_model_cnn.hdf5', custom_objects={'mil_squared_error': mil_squared_error, 'adam': adam})


# In[5]:


def extract_features_for_all_wavs(dest, label):
    result = np.expand_dims(np.zeros((48, 272)), axis=0)

    for wav in os.listdir(dest):
        vec = extract_feats_single_wav(dest + wav)
        if not str(vec.shape) == '(48, 272)':
            continue
        result = np.vstack((result, np.expand_dims(vec, axis=0)))

    result = result[1:]
    labels = np.expand_dims(np.asarray([label] * len(result)), axis=1)
    print(result.shape)
    print(labels.shape)

    return result, labels


# In[24]:


# this part assumes that the model is trained.
# import sounds

import shutil

def identify(test_dir, threshold, distribution_measure_dir):
    in_dist = []
    out_dist = []
    
    for wav in os.listdir(test_dir):
        
        if not wav.endswith('wav'):
            continue
        else:
            feat_vec = np.expand_dims(extract_feats_single_wav(test_dir + wav), axis=0)
            softmax = np.squeeze(model.predict(feat_vec))
            predicted_label = np.argmax(softmax)
            
            if is_in_distribution(feat_vec, predicted_label) and softmax[predicted_label] > threshold:
                in_dist.append(feat_vec)
                
                shutil.copyfile(test_dir+wav, distribution_measure_dir + wav)
                
                #print('in distribution sample ' + wav)
            else:
                out_dist.append(feat_vec)

            #print(str(len(in_dist) + len(out_dist)) + ' out of ' + str(len(os.listdir(test_dir))))
            
    #print('in distribution samples: ' + str(len(in_dist)))
    #print('out of distribution samples: ' + str(len(out_dist)))
    
    return in_dist, out_dist


# In[ ]:


test_dirs = "D://Karen's parents//"
distribution_measure_dirs = "D://Karen's parents//in_distribution//"

for test_dir in os.listdir(test_dirs):
    if "parents" in test_dir:
        continue
    else:
        distribution_measurement_dir = distribution_measure_dirs + test_dir + '//'
        dir_name = test_dir
        test_dir = test_dirs + test_dir + '//'

        print(test_dir)
        
        try:
            shutil.rmtree(distribution_measurement_dir)
        except:
            pass

        os.makedirs(distribution_measurement_dir)
        
        in_dist, out_dist = identify(test_dir, 0.875, distribution_measurement_dir)
        
        
        try:
            os.mkdirs(distribution_measure_dirs + dir_name)
        except:
            print(distribution_measure_dirs + dir_name + ' already exists.')
        
        np.save(distribution_measure_dirs + dir_name + '//distribution.npy', np.asarray([in_dist, out_dist]))


# In[ ]:




