#!/usr/bin/env python
# coding: utf-8
import keras
import tensorflow as tf

import numpy as np
import pickle
import os
import sys
import shutil

import scipy.io as sio

import librosa
import soundfile as sf

# Used to record audio streams
import wave
import datetime
import time

# Used to process audio data
import contextlib
import pyaudio
from pydub import AudioSegment

# Used to read Matlab files from python
import matlab.engine

# Import the voice activity detection module
import speech_recognition as sr

# disable warnings
import warnings
warnings.filterwarnings('ignore')
os.environ['TF_CPP_MIN_VLOG_LEVEL'] = '3'

import sys
sys.path.insert(
    1, '..//..//speaker_id_module//SpeakerID//')


from extract_feat import extract_feats_single_wav

def load_emp_miu(y):
    path = '..//models//emp_miu_class_' + str(y) + '.npy'
    return np.load(path)

def load_inv_emp_covar():
    path = '..//models//inv_emp_sigma.npy'
    return np.load(path)

def load_mahalanobis_mean(y):
    path = '..//models//mahalanobis_mean_class_' + str(y) + '.npy'
    return np.load(path)

def load_mahalanobis_std(y):
    path = '..//models//mahalanobis_std_class_' + str(y) + '.npy'
    return np.load(path)

def load_mahalanobis_coeff(y):
    path = '..//models//mahalanobis_threshold_coefficient_class_' + str(y) + '.npy'
    return np.load(path)

def detect_ood(raw_penult_x, predicted_y):
       
    emp_miu = load_emp_miu(predicted_y)
    inv_emp_sigma = load_inv_emp_covar()
    m_mean = load_mahalanobis_mean(predicted_y)
    m_std = load_mahalanobis_std(predicted_y)
    coeff = load_mahalanobis_coeff(predicted_y)
    
    upper = m_mean + coeff*m_std
    lower = m_mean - coeff*m_std

    x = np.linalg.norm(raw_penult_x)

    try:
        m = np.transpose(x-emp_miu) @ inv_emp_sigma @ (x-emp_miu)
    except:
        m = (x-emp_miu) * inv_emp_sigma * (x-emp_miu)
    
    if lower < m and m < upper:
        return False
    else:
        return True

if __name__ == '__main__':

    classify_all_clips = False

    eng = matlab.engine.start_matlab()

    path = '..//generated_data//audio_storage//is_speech//'
    dest = '..//generated_data//audio_storage//is_registered_speaker//'

    generated_speakerid_path = '..//generated_data//generated_speaker_id//'
    generated_mood_path = '..//generated_data//generated_mood_classification//'
    generated_recommendation_path = '..//generated_data//generated_recommendations//'
    generated_audio_cloud = '..//generated_data//generated_audios_to_upload//'
    if not os.path.isdir(dest):
        os.mkdir(dest)

    model = keras.models.load_model('..//models//cnn.hdf5')

    intermediate_layer_model = keras.Model(inputs=model.input,
                                    outputs=model.get_layer(index=len(model.layers)-2).output)


    emp_miu_0 = np.load('..//models//emp_miu_class_0.npy')
    emp_miu_1 = np.load('..//models//emp_miu_class_1.npy')
    inv_emp_sigma = np.load('..//models//inv_emp_sigma.npy')
    #coeff = np.load('models//mahalanobis_threshold_coefficient.npy') 
    m_mean_0 = np.load('..//models//mahalanobis_mean_class_0.npy')
    m_mean_1 = np.load('..//models//mahalanobis_mean_class_1.npy')
    m_std_0 = np.load('..//models//mahalanobis_std_class_0.npy')
    m_std_1 = np.load('..//models//mahalanobis_std_class_1.npy')
    print('speaker identification is initialized...')

    while True:
        if len(os.listdir(path)) == 0:
            time.sleep(2.5)
            continue
        for fname in os.listdir(path):
            if not fname.endswith('.wav'):
                continue
            try:
                audio = path + fname
                vec = np.expand_dims(extract_feats_single_wav(audio), axis=0)
                raw_pred = np.squeeze(model.predict(vec))

                penult_raw_pred = np.squeeze(intermediate_layer_model.predict(vec))
                pred = np.argmax(raw_pred)

                if detect_ood(penult_raw_pred, pred):
                    print('out of distribution sample' + str(raw_pred))
                elif raw_pred[pred] <= 0.675:
                    print('not registered speaker w/ vector' + str(raw_pred))

                else:
                    if pred == 0:
                        speaker = 'Jack'
                    elif pred == 1:
                        speaker = 'Ashley'
                    else:
                        speaker = 'unrecognized'

                    print(fname + ': ' + speaker + ' w/ vec ' + str(raw_pred) )

                os.remove(path + fname)

                    
            except Exception as e:  # intelligible
                print(e)
                os.remove(path + fname)
