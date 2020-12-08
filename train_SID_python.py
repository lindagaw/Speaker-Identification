from pydub import AudioSegment
import time
import datetime
import os
import numpy as np

import pickle

import random
from xml.dom import minidom
from librosa import load
import shutil

from extract_feat import extract_feats_single_wav
from sklearn import svm
from sklearn.ensemble import RandomForestClassifier


from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, f1_score, precision_score, recall_score, confusion_matrix

from mlxtend.feature_selection import SequentialFeatureSelector as sfs

path_caregiver = 'speaker_id_module//SpeakerID//singles//1-caregiver//'
dest_caregiver = 'app//2-Training//singles//1-caregiver//'

path_patient = 'speaker_id_module//SpeakerID//singles//2-patient//'
dest_patient = 'app//2-Training//singles//2-patient//'

def slice_audios(path, dest):
    for old_audio in os.listdir(dest):
        os.remove(dest + old_audio)
        
    for audio_index in range(0, len(os.listdir(path))):
        target_audio_path =  path + os.listdir(path)[audio_index]
        print('input audio: ' + target_audio_path)

        target_audio = AudioSegment.from_wav(target_audio_path)
        target_duration = target_audio.duration_seconds

        folds = int(target_duration/5.0)
        for fold in range(0, folds+1):

            start_time = time.time()

            start_time = fold * 5000  # Works in milliseconds
            end_time = (fold + 1) * 5000
            new_audio = target_audio[start_time:end_time]

            components = target_audio_path.split('/')
            filename = components[len(components)-1]

            new_audio_path = dest + filename[:len(filename)-4] + '_' + str(fold) + '.wav'
            print('generated the slice of the audio segment at index ' + str(fold) )
            new_audio.export(new_audio_path, format="wav")

def change_amplitude(emotionfile, d1, newSoundFile, d2):
    
    if d1 <= d2:
        sound = AudioSegment.from_file(emotionfile) - np.random.randint(0, (6 * d2/d1 - 1))
        sound.export(newSoundFile, format='wav')  ### save the new generated file in a folder
    else:
        print('Invalid distance parameters. d1 should be <= d2.')

def change_amplitude_range(emotionfile, newSoundFile, threshold):
    #amount = np.random.randint(0, threshold)
    amount = random.uniform(0, threshold)
    #print('Deamplify ' + str(emotionfile) + ' by ' + str(amount) + ' db.')
    sound = AudioSegment.from_file(emotionfile) - amount
    sound.export(newSoundFile, format='wav')  ### save the new generated file in a folder
    return amount

def deamplify_per_folder(directory):
    for file in os.listdir(directory):
        if file.endswith('.wav'):
            soundFile = directory + file
            newSoundFile = directory + 'deamplified_' + file
            change_amplitude_range(soundFile, newSoundFile, 12)

def add_noise_and_deamplify_per_folder(directory, extension, noise_directory):
    for file in os.listdir(directory):
        if file.endswith(extension) and not file[1] == '_':
            for i in range(0, 1):
                soundFile = directory + file
                amount = change_amplitude_range(soundFile, soundFile, 12)
                noise = random.choice(os.listdir(noise_directory))
                random_noise = noise_directory + noise
                newSoundFile = directory + 'deamp_' + str(amount) + '_noise_' + noise[:len(noise)-5] + '_' + file
                add_noise_per_file(soundFile, random_noise, newSoundFile)
                print(newSoundFile)

def add_noise_per_file(emotionfile, bgnoise, newSoundFile):
    
    emotionsound = AudioSegment.from_wav(emotionfile)
    emotion_duration = emotionsound.duration_seconds * 1000
    noise = AudioSegment.from_wav(bgnoise)
    noise_duration = noise.duration_seconds * 1000
    
    threshold = noise_duration - emotion_duration
    
    if threshold > 0:
        overlay_start = np.random.randint(0, threshold)
    else:
        overlay_start = 0
        
    targeted_chunk = noise[overlay_start:overlay_start + emotion_duration]
    newSound = emotionsound.overlay(targeted_chunk, position=0)
    newSound=newSound[0:5000]
    newSound.export(newSoundFile, format='wav')  ### save the new generated file in a folder

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

def random_forest():
    clf = RandomForestClassifier(n_estimators=100, 
                            bootstrap = True,
                            max_features = 'sqrt')

    # Fit on training data
    clf.fit(X_train, np.squeeze(y_train))

    y_train_pred = clf.predict(X_train)
    print('Training accuracy on selected features: %.3f' % accuracy_score(y_train, y_train_pred))

    y_test_pred = clf.predict(X_test)
    print('Testing accuracy on selected features: %.3f' % accuracy_score(y_test, y_test_pred))

    filename = 'models//finalized_random_forest.sav'
    pickle.dump(clf, open(filename, 'wb'))

def support_vector_machine():
    clf = svm.SVC()
    # Fit on training data
    
    clf = RandomForestClassifier(n_estimators=100, 
                            bootstrap = True,
                            max_features = 'sqrt')
    # Fit on training data
    clf.fit(X_train, np.squeeze(y_train))

    y_train_pred = clf.predict(X_train)
    print('Training accuracy on selected features: %.3f' % accuracy_score(y_train, y_train_pred))

    y_test_pred = clf.predict(X_test)
    print('Testing accuracy on selected features: %.3f' % accuracy_score(y_test, y_test_pred))

    filename = 'models//finalized_svm.sav'
    pickle.dump(clf, open(filename, 'wb'))


# STEP 1: slice into 5-second wavs
slice_audios(path_caregiver, dest_caregiver)
slice_audios(path_patient, dest_patient)
noise_directory = 'D://Background_noise//noise_home//'

add_noise_and_deamplify_per_folder(dest_caregiver, '.wav', noise_directory)
add_noise_and_deamplify_per_folder(dest_patient, '.wav', noise_directory)

# Step 2: get the vecs of shape (X, 48, 272)
X_caregiver, y_caregiver = extract_features_for_all_wavs(dest_caregiver, 0)
X_patient, y_patient = extract_features_for_all_wavs(dest_patient, 1)

X = np.vstack((X_caregiver, X_patient))
y = np.vstack((y_caregiver, y_patient))

nsamples, nx, ny = X.shape
X = X.reshape((nsamples,nx*ny))

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.33, random_state=42)

support_vector_machine()
random_forest()