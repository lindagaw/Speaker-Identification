import random
import time
import datetime
import numpy as np
from numpy import array
import pandas as pd
from pydub import AudioSegment
import os
import shutil
import glob
import gc
import sys
#import h5py
from sklearn import model_selection
from sklearn.ensemble import RandomForestClassifier

#imported for testing
import wave
import contextlib

# for outputing file
import scipy.stats.stats as st

from sklearn.datasets import make_classification
from sklearn.ensemble import ExtraTreesClassifier
from sklearn.preprocessing import normalize
from sklearn.neighbors import KNeighborsClassifier
from scipy.cluster.vq import vq, kmeans, whiten
from sklearn.cluster import KMeans
from sklearn.svm import SVC
from sklearn.metrics import f1_score
from sklearn import mixture
from sklearn.model_selection import train_test_split
from sklearn.utils import class_weight
from sklearn.impute import SimpleImputer
import pickle
import librosa
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA as sklearnPCA
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis as LDA
from sklearn.datasets.samples_generator import make_blobs
from sklearn.metrics import auc, confusion_matrix, accuracy_score, precision_score, recall_score, f1_score


from IPython.display import clear_output

import warnings

warnings.filterwarnings('ignore')
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

sample_rate = 44100
hop_length = 441  # frame size= 2 * hop
segment_length = int(sample_rate * 0.2)  # 0.2
segment_pad = int(sample_rate * 0.02)  # 0.02
overlapping = int(sample_rate * 0.1)  # 0.1

NumofFeaturetoUse = 272
frame_number = 48

try:
    NumofFeaturetoUse = int(sys.argv[1])
    print('Number of features to use is set to ' + str(sys.argv[1]))
except:
    print('Number of features are unspecified. Defaut is set to = 272.')


def update_progress(progress):
    bar_length = 100
    if isinstance(progress, int):
        progress = float(progress)
    if not isinstance(progress, float):
        progress = 0
    if progress < 0:
        progress = 0
    if progress >= 1:
        progress = 1
    block = int(round(bar_length * progress))
    clear_output(wait=True)
    text = "Progress: [{0}] {1:.1f}%".format(
        "#" * block + "-" * (bar_length - block), progress * 100)
    print(text)


def str_to_int(input_np):
    output_np = []
    for x in np.nditer(input_np):
        if x == 'H':
            x = 0
        elif x == 'A':
            x = 1
        elif x == 'N':
            x = 2
        else:
            x = 3
        output_np.append(x)
    output_np = np.array(output_np)
    output_np = np.reshape(output_np, (len(output_np), 1))
    return output_np


def float_compatible(input_np):
    input_np = np.nan_to_num(input_np)
    x = np.where(input_np >= np.finfo(np.float32).max)
    for index in range(0, len(x[0])):
        try:
            x_position = x[0][index]
            y_position = x[1][index]
            input_np[x_position, y_position] = 0.0
        except:
            print(x)
            print(x[0])
    return input_np


def function_FeatureExtractfromSinglewindow(y, hop_length, sr):
    genFeatures = np.array([])

    mfcc0 = librosa.feature.mfcc(
        y=y, sr=sr, n_fft=hop_length*2, hop_length=hop_length, n_mfcc=13)
    mfcc = np.transpose(mfcc0)
    genFeatures = np.hstack((genFeatures, np.amin(mfcc, 0)))
    genFeatures = np.hstack((genFeatures, np.amax(mfcc, 0)))
    genFeatures = np.hstack((genFeatures, np.median(mfcc, 0)))
    genFeatures = np.hstack((genFeatures, np.mean(mfcc, 0)))
    genFeatures = np.hstack((genFeatures, np.std(mfcc, 0)))
    genFeatures = np.hstack((genFeatures, np.var(mfcc, 0)))
    genFeatures = np.hstack((genFeatures, st.skew(mfcc, 0)))
    genFeatures = np.hstack((genFeatures, st.kurtosis(mfcc, 0)))
    #print(genFeatures.shape)

    mfcc_delta = librosa.feature.delta(mfcc0)
    mfcc_delta = np.transpose(mfcc_delta)
    genFeatures = np.hstack((genFeatures, np.amin(mfcc_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.amax(mfcc_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.median(mfcc_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.mean(mfcc_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.std(mfcc_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.var(mfcc_delta, 0)))
    genFeatures = np.hstack((genFeatures, st.skew(mfcc_delta, 0)))
    genFeatures = np.hstack((genFeatures, st.kurtosis(mfcc_delta, 0)))
    #print(genFeatures.shape)

    zcr0 = librosa.feature.zero_crossing_rate(
        y=y, frame_length=hop_length*2, hop_length=hop_length)
    zcr = np.transpose(zcr0)
    genFeatures = np.hstack((genFeatures, np.amin(zcr, 0)))
    genFeatures = np.hstack((genFeatures, np.amax(zcr, 0)))
    genFeatures = np.hstack((genFeatures, np.median(zcr, 0)))
    genFeatures = np.hstack((genFeatures, np.mean(zcr, 0)))
    genFeatures = np.hstack((genFeatures, np.std(zcr, 0)))
    genFeatures = np.hstack((genFeatures, np.var(zcr, 0)))
    genFeatures = np.hstack((genFeatures, st.skew(zcr, 0)))
    genFeatures = np.hstack((genFeatures, st.kurtosis(zcr, 0)))
    #print(genFeatures.shape)

    zcr_delta = librosa.feature.delta(zcr0)
    zcr_delta = np.transpose(zcr_delta)
    genFeatures = np.hstack((genFeatures, np.amin(zcr_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.amax(zcr_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.median(zcr_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.mean(zcr_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.std(zcr_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.var(zcr_delta, 0)))
    genFeatures = np.hstack((genFeatures, st.skew(zcr_delta, 0)))
    genFeatures = np.hstack((genFeatures, st.kurtosis(zcr_delta, 0)))
    #print(genFeatures.shape)

    Erms0 = librosa.feature.rms(
        y=y, frame_length=hop_length*2, hop_length=hop_length)
    Erms = np.transpose(Erms0)
    genFeatures = np.hstack((genFeatures, np.amin(Erms, 0)))
    genFeatures = np.hstack((genFeatures, np.amax(Erms, 0)))
    genFeatures = np.hstack((genFeatures, np.median(Erms, 0)))
    genFeatures = np.hstack((genFeatures, np.mean(Erms, 0)))
    genFeatures = np.hstack((genFeatures, np.std(Erms, 0)))
    genFeatures = np.hstack((genFeatures, np.var(Erms, 0)))
    genFeatures = np.hstack((genFeatures, st.skew(Erms, 0)))
    genFeatures = np.hstack((genFeatures, st.kurtosis(Erms, 0)))
    #print(genFeatures.shape)

    Erms_delta = librosa.feature.delta(Erms0)
    Erms_delta = np.transpose(Erms_delta)
    genFeatures = np.hstack((genFeatures, np.amin(Erms_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.amax(Erms_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.median(Erms_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.mean(Erms_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.std(Erms_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.var(Erms_delta, 0)))
    genFeatures = np.hstack((genFeatures, st.skew(Erms_delta, 0)))
    genFeatures = np.hstack((genFeatures, st.kurtosis(Erms_delta, 0)))
    #print(genFeatures.shape)

    cent0 = librosa.feature.spectral_centroid(
        y=y, sr=sr, n_fft=hop_length*2, hop_length=hop_length)
    cent = np.transpose(cent0)
    genFeatures = np.hstack((genFeatures, np.amin(cent, 0)))
    genFeatures = np.hstack((genFeatures, np.amax(cent, 0)))
    genFeatures = np.hstack((genFeatures, np.median(cent, 0)))
    genFeatures = np.hstack((genFeatures, np.mean(cent, 0)))
    genFeatures = np.hstack((genFeatures, np.std(cent, 0)))
    genFeatures = np.hstack((genFeatures, np.var(cent, 0)))
    genFeatures = np.hstack((genFeatures, st.skew(cent, 0)))
    genFeatures = np.hstack((genFeatures, st.kurtosis(cent, 0)))
    #print(genFeatures.shape)

    cent_delta = librosa.feature.delta(cent0)
    cent_delta = np.transpose(cent_delta)
    genFeatures = np.hstack((genFeatures, np.amin(cent_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.amax(cent_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.median(cent_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.mean(cent_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.std(cent_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.var(cent_delta, 0)))
    genFeatures = np.hstack((genFeatures, st.skew(cent_delta, 0)))
    genFeatures = np.hstack((genFeatures, st.kurtosis(cent_delta, 0)))
    #print(genFeatures.shape)
    #Each frame of a magnitude spectrogram is normalized and treated as a distribution over frequency bins, from which the mean (centroid) is extracted per frame.

    ############### pitch at certain frame
    pitches, magnitudes = librosa.core.piptrack(
        y=y, sr=sr, fmin=75, fmax=8000, n_fft=hop_length*2, hop_length=hop_length)
    p = [pitches[magnitudes[:, i].argmax(), i]for i in range(0, pitches.shape[1])]
    pitch0 = np.array(p)  # shape (305,)
    pitch = np.transpose(pitch0)
    genFeatures = np.hstack((genFeatures, np.amin(pitch, 0)))
    genFeatures = np.hstack((genFeatures, np.amax(pitch, 0)))
    genFeatures = np.hstack((genFeatures, np.median(pitch, 0)))
    genFeatures = np.hstack((genFeatures, np.mean(pitch, 0)))
    genFeatures = np.hstack((genFeatures, np.std(pitch, 0)))
    genFeatures = np.hstack((genFeatures, np.var(pitch, 0)))
    genFeatures = np.hstack((genFeatures, st.skew(pitch, 0)))
    genFeatures = np.hstack((genFeatures, st.kurtosis(pitch, 0)))
    #print(genFeatures.shape)

    pitch_delta = librosa.feature.delta(pitch0)
    pitch_delta = np.transpose(pitch_delta)
    genFeatures = np.hstack((genFeatures, np.amin(pitch_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.amax(pitch_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.median(pitch_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.mean(pitch_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.std(pitch_delta, 0)))
    genFeatures = np.hstack((genFeatures, np.var(pitch_delta, 0)))
    genFeatures = np.hstack((genFeatures, st.skew(pitch_delta, 0)))
    genFeatures = np.hstack((genFeatures, st.kurtosis(pitch_delta, 0)))

    #print(genFeatures.shape)    #272
    return genFeatures


# Extract specified amount of features from an audio file
def extract_feats_single_wav(audiofile):
    flag_start_all = 0
    All = np.array([])
    audio, s_rate = librosa.load(audiofile, sr=sample_rate)
    segment_start_flag = 0
    start_seg = 0
    while(start_seg + segment_length) < len(audio):
        sound_window = audio[start_seg:(start_seg + segment_length)]
        featureSet = function_FeatureExtractfromSinglewindow(
            sound_window, hop_length, s_rate)
        if segment_start_flag == 0:
            SegAllFeat = featureSet
            segment_start_flag = 1
        else:
            SegAllFeat = np.vstack((SegAllFeat, featureSet))
        start_seg = start_seg + overlapping
    
    SegAllFeat = float_compatible(SegAllFeat)
    SegAllFeat = normalize(SegAllFeat, norm='l2', axis=0)

    if flag_start_all == 0:
        All = SegAllFeat
        flag_start_all = 1
    else:
        All = np.vstack((All, SegAllFeat))
    
    #audio_npy = (audiofile[:len(audiofile)-4] + '.npy').split('//')[len((audiofile[:len(audiofile)-4] + '.npy').split('//'))-1]
    All = float_compatible(All)
    '''
    if str(All.shape) == '(48, 272)':
        np.save(audio_npy, All)
        print('saved')
    '''
    return All

