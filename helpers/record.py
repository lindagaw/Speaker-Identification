import os
import sys
import shutil

import scipy.io as sio
import librosa
import soundfile as sf
import os
# Used to record audio streams
import wave
import datetime
import time
import shutil
from tqdm import tqdm
import sys
# Used to process audio data
import contextlib
import pyaudio
from pydub import AudioSegment

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

def replace_special_chars(z, special_chars, new_char):
    removeSpecialChars = z.translate({ord(c): new_char for c in special_chars})
    return removeSpecialChars

def record_single_session(RECORD_SECONDS, raw_audio_dir, speaker):

    p = pyaudio.PyAudio()

    stream = p.open(format=FORMAT,
                    channels=CHANNELS,
                    rate=RATE,
                    input=True,
                    frames_per_buffer=CHUNK)

    print("Recording in process...")

    CURRENT_TIME = str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

    #WAVE_OUTPUT_FILENAME = replace_special_chars(CURRENT_TIME, ': ', '-') + '.wav'
    WAVE_OUTPUT_FILENAME = speaker + '.wav'

    frames = []

    #for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
    #    data = stream.read(CHUNK)
    #    frames.append(data)

    for i in tqdm (range(int(RATE / CHUNK * RECORD_SECONDS)), desc="Recording..."):
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

    shutil.move(WAVE_OUTPUT_FILENAME, os.path.join(raw_audio_dir, WAVE_OUTPUT_FILENAME))

    return WAVE_OUTPUT_FILENAME
