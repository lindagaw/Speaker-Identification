#!/usr/bin/env python
# coding: utf-8
from func_timeout import func_timeout, FunctionTimedOut

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

import urllib3

def internet_on():
    try:
        urllib3.urlopen('http://216.58.192.142', timeout=0.5)
        return True
    except urllib3.URLError as err:
        print('internet is down!')
        return False

if __name__ == '__main__':
    classify_all_clips = True
    r = sr.Recognizer()

    path = 'generated_data//audio_storage//raw//'
    dest = 'generated_data//audio_storage//is_speech//'
    
    if not os.path.isdir(dest):
        os.makedirs(dest)

    print('voice activity detection is initialized...')
    while True:

        try:

            if len(os.listdir(path)) == 0:
                continue
            #time.sleep(1)
            for fname in os.listdir(path):

                if not fname.endswith('.wav'):
                    os.remove(path + fname)
                    continue
                
                with sr.AudioFile(path + fname) as source:
                    try:
                        audio = func_timeout(10, r.record, args=(source, 7.884e+6))
                        #audio = r.record(source, duration=7.884e+6)
                    except Exception as e:
                        print(e)

                try:
                    transcription = func_timeout(5.5, r.recognize_google, args=[audio])
                    #transcription = r.recognize_google(audio)
                    print(fname + ' is speech.')
                    #print('speech detected')
                    print(transcription)
                    os.rename(path + fname, dest + fname)
                except Exception as e:
                    print(e)
                    os.remove(path + fname)
                    print('deleted non-speech seg ' + fname)

        except:
            print('passing ... outer loop')

'''
recognize_bing(): Microsoft Bing Speech
recognize_google(): Google Web Speech API
recognize_google_cloud(): Google Cloud Speech - requires installation of the google-cloud-speech package
recognize_houndify(): Houndify by SoundHound
recognize_ibm(): IBM Speech to Text
recognize_sphinx(): CMU Sphinx - requires installing PocketSphinx and swig
recognize_wit(): Wit.ai
'''
