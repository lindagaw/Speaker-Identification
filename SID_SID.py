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
    1, '..//speaker_id_module//SpeakerID//')


from extract_feat import extract_feats_single_wav

if __name__ == '__main__':

    classify_all_clips = False

    eng = matlab.engine.start_matlab()

    path = 'generated_data//audio_storage//is_speech//'
    dest = 'generated_data//audio_storage//is_registered_speaker//'

    generated_speakerid_path = 'generated_data//generated_speaker_id//'
    generated_mood_path = 'generated_data//generated_mood_classification//'
    generated_recommendation_path = 'generated_data//generated_recommendations//'
    generated_audio_cloud = 'generated_data//generated_audios_to_upload//'
    if not os.path.isdir(dest):
        os.mkdir(dest)

    model = keras.models.load_model('models//cnn.hdf5')

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

                '''
                nsamples, nx, ny = vec.shape
                vec = vec.reshape((nsamples,nx*ny))
                rf = 'models//finalized_random_forest.sav'
                svm = 'models//finalized_svm.sav'
                clf = pickle.load(open(rf, 'rb'))
                
                '''
                raw_pred = np.squeeze(model.predict(vec))
                pred = np.argmax(raw_pred)

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
