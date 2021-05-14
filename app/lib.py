import tkinter as tk
import tkinter.ttk as ttk
from tkinter.ttk import *
from tkinter import *
import sys

import subprocess
import os

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
from pydub.playback import play

#import matlab.engine

import logging
import threading
import time
import ctypes

import speech_recognition as sr
