#!/usr/bin/env python
# coding: utf-8

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

CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
RECORD_SECONDS = 5

# Used to receive a single session of audio input from the microphone
# Used to produce a less messy formatting for the current time

def replace_special_chars(z, special_chars, new_char):
    removeSpecialChars = z.translate({ord(c): new_char for c in special_chars})
    return removeSpecialChars

def record_single_session(CHUNK, FORMAT, CHANNELS, RATE, RECORD_SECONDS):

    p = pyaudio.PyAudio()

    stream = p.open(format=FORMAT,
                    channels=CHANNELS,
                    rate=RATE,
                    input=True,
                    frames_per_buffer=CHUNK)

    print("Recording in process...")

    CURRENT_TIME = str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

    WAVE_OUTPUT_FILENAME = replace_special_chars(CURRENT_TIME, ': ', '-') + '.wav'

    frames = []

    for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
        data = stream.read(CHUNK)
        frames.append(data)

    # print("Recording finished...")

    stream.stop_stream()
    stream.close()
    p.terminate()

    wf = wave.open(WAVE_OUTPUT_FILENAME, 'wb')
    wf.setnchannels(CHANNELS)
    wf.setsampwidth(p.get_sample_size(FORMAT))
    wf.setframerate(RATE)
    wf.writeframes(b''.join(frames))
    wf.close()
    
    print("Generated audio file " + WAVE_OUTPUT_FILENAME)

    os.rename(WAVE_OUTPUT_FILENAME, raw_audio_dir + WAVE_OUTPUT_FILENAME)

    return WAVE_OUTPUT_FILENAME

def update_log(log_path, status):
    record_time = str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

    log = open(log_path,"w")
    log.write(str(record_time) + ' ' + str(status))
    log.close()


if __name__ == '__main__':

    # is the microphone still plugged?
    unplugged = False
    pipeline_log_dir = './/pipeline_log//'
    if not os.path.isdir(pipeline_log_dir):
        os.makedirs(pipeline_log_dir)


    raw_audio_dir = '..//generated_data//audio_storage//raw//'

    if not os.path.isdir(raw_audio_dir):
        os.makedirs(raw_audio_dir)

    while True:
        try:
            record_single_session(CHUNK, FORMAT, CHANNELS, RATE, RECORD_SECONDS)
            unplugged = False
        except Exception as e:
            print(e)
            time.sleep(5.0)
            unplugged = True

        log_path = pipeline_log_dir + 'microphone_status.txt'
        update_log(log_path, unplugged)
    # after a window is created, signal the VAD module
