import tkinter as tk
import tkinter.ttk as ttk
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

import logging
import threading
import time

CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
RECORD_SECONDS = 5


class Root(tk.Tk):
    """Container for all frames within the application"""
    
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        
        #initialize menu
        self.config(menu=MenuBar(self)) # the menu bar on top
        
        self.appFrame = Application(self) # the application bar, which is now left for blank
        self.appFrame.pack(side='top', fill='both', expand='True')
        
        self.status = StatusBar(self) # the status bar at the bottom
        self.status.pack(side='bottom', fill='x')
        
class MenuBar(tk.Menu):

    def __init__(self, parent):
        tk.Menu.__init__(self, parent)

        filemenu = tk.Menu(self, tearoff=False)
        self.add_cascade(label="File",underline=0, menu=filemenu)
        filemenu.add_command(label="New", command=self.callback)
        filemenu.add_separator()
        filemenu.add_command(label="Exit", underline=1, command=self.quit)

        helpmenu = tk.Menu(self, tearoff=False)
        self.add_cascade(label="Help", menu=helpmenu)
        helpmenu.add_command(label="About...", command=self.callback)

    def quit(self):
        sys.exit(0)
    
    def callback(self):
        print("called the callback!")

class StatusBar(ttk.Frame):

    def __init__(self, master):
        ttk.Frame.__init__(self, master)
        self.label = ttk.Label(self, relief='sunken', anchor='w')
        self.label.pack(fill='x')

    def set(self, format, *args):
        self.label.config(text=format % args)
        self.label.update_idletasks()

    def clear(self):
        self.label.config(text="")
        self.label.update_idletasks()

            
class Application(ttk.Notebook):

    def __init__(self, root):
        ttk.Notebook.__init__(self, root)

        tab1 = ttk.Frame(self)
        tab2 = ttk.Frame(self)
        tab3 = ttk.Frame(self)
        tab4 = ttk.Frame(self)
        
        self.add(tab1, text = "Caregiver's Voice")
        self.add(tab2, text = "Patient's Voice")
        self.add(tab3, text = "Train the Model")
        self.add(tab4, text = "Test the Model")

        voice_record_button(tab1, 'caregiver')
        voice_record_button(tab2, 'patient')

        train_sid_button(tab3)
        voice_record_button_for_testing(tab4)

        try:
            os.mkdir('2-Training//singles//0-nonFamily//')
        except:
            shutil.rmtree('2-Training//singles//0-nonFamily//')
            os.mkdir('2-Training//singles//0-nonFamily//')

def voice_record_button_for_testing(tab):
    def countdown(count, elapsed_time_label):
        # change text in label       
        display = convert(count) 
        elapsed_time_label['text'] = 'Remaining time: ' + str(display)

        if count > 0:
            # call countdown again after 1000ms (1s)
            root.after(1000, countdown, count-1)
        if count <= 0:
            elapsed_time_label['text'] = 'Recording finished'
    

    def voice_record(voice_record_button, free_speaking_time, elapsed_time_label, role):
            if role == 'caregiver':
                location = '3-Testing//singles//caregiver//'
            else:
                location = '3-Testing//Singles//patient//'

            shutil.rmtree(location)
            os.makedirs(location)

            if voice_record_button['text'] == 'Start Recording' or voice_record_button['text'] == 'Restart Recording':
                x = threading.Thread(target=countdown, args=(free_speaking_time, elapsed_time_label,))
                y = threading.Thread(target=record_single_session, args=(CHUNK, FORMAT, CHANNELS, RATE, \
                       free_speaking_time, role, location,))
                x.start()
                y.start()
                voice_record_button['text'] = 'Restart Recording'
            else:
                pass

    # the overview label
    overview_label = ttk.Label(tab, text="In this tab we test if the speaker identification algorithm is successfully trained.", font=("Times New Roman", 11))
    overview_label.pack()

    # the checklist label
    checklist_label = ttk.Label(tab, text="Please return to the previous tabs to re-collect the voices and retrain, if this attempt is unsccessful.", font=("Times New Roman", 11))
    checklist_label.pack()

    # test the caregiver's voice label
    record_voice_label = ttk.Label(tab, text="Please let the caregiver speak", font=("Times New Roman", 11))
    record_voice_label.pack()

    caregiver_elapsed_time_label = tk.Label(tab, fg="dark green", font=("Times New Roman", 20))
    caregiver_elapsed_time_label.pack()

    caregiver_voice_record_button = tk.Button(tab, text='Start Recording', width=25)
    caregiver_voice_record_button['command'] = voice_record(caregiver_voice_record_button, 10, caregiver_elapsed_time_label, 'caregiver')
    caregiver_voice_record_button.pack()


def train_sid_button(tab):
    # the overview label
    overview_label = ttk.Label(tab, text="Here we train the speaker identification model.", font=("Times New Roman", 11))
    overview_label.pack()

    # the checklist label
    checklist_label = ttk.Label(tab, text="Please check the following items.", font=("Times New Roman", 11))
    checklist_label.pack()

    # caregiver's voice
    caregiver_label = ttk.Label(tab, text="Is the caregiver's voice collected?", font=("Times New Roman", 11))
    caregiver_label.pack()

    MODES = [("Yes", 1), ("No", 0)]

    caregiver_var = tk.IntVar()
    caregiver_var.set(0) # initialize
    for text, mode in MODES:
        caregiver_button = ttk.Radiobutton(tab, text=text, variable=caregiver_var, value=mode)
        caregiver_button.pack(anchor='w')

    # patient's voice
    patient_label = ttk.Label(tab, text="Is the patient's voice collected?", font=("Times New Roman", 11))
    patient_label.pack()

    MODES = [("Yes", 1), ("No", 0)]

    patient_var = tk.IntVar()
    patient_var.set(0) # initialize
    for text, mode in MODES:
        patient_button = ttk.Radiobutton(tab, text=text, variable=patient_var, value=mode)
        patient_button.pack(anchor='w')


    training_label = tk.Label(tab, fg="dark green", font=("Times New Roman", 20))
    training_label.pack()

    def check(yield_time):
        count = 0
        while True:
            if not os.path.exists("2-Training//models_1024.mat"):
                count += 1
                training_label['text'] = 'Training' + '.' * (count % 4)

                time.sleep(yield_time)
            else:
                training_label['text'] = 'Finished'

    def train():
        if training_button['text'] == 'Start Training' or training_button['text'] == 'Restart Training':
            subprocess.Popen([r"cmd"])
            subprocess.Popen([r"2-Training//M2FEDTraining.exe"])
            training_button['text'] = 'Restart Training'
        else:
            pass

    def train_and_check():
        x = threading.Thread(target=train, args=())
        y = threading.Thread(target=check, args=(1,))
        x.start()
        y.start()
            
    training_button = tk.Button(tab, text='Start Training', width=25, command=train_and_check)
    training_button.pack()

def voice_record_button(tab1, role):
    # the overview label
    overview_label = ttk.Label(tab1, text="Here we collect the " + str(role) + "'s voice.", font=("Times New Roman", 11))
    overview_label.pack()

    # the checklist label
    checklist_label = ttk.Label(tab1, text="Please check the following items.", font=("Times New Roman", 11))
    checklist_label.pack()

    # only caregiver in the room label
    only_caregiver_label = ttk.Label(tab1, text="Is the " + str(role) + " the only person in the room, except the voice collector?", font=("Times New Roman", 11))
    only_caregiver_label.pack()

    MODES = [("Yes", 1), ("No", 0)]

    only_caregiver_var = tk.IntVar()
    only_caregiver_var.set(0) # initialize
    for text, mode in MODES:
        only_caregiver_button = ttk.Radiobutton(tab1, text=text, variable=only_caregiver_var, value=mode)
        only_caregiver_button.pack(anchor='w')

    # caregiver informed label
    caregiver_informed_label = ttk.Label(tab1, text="Is the " + str(role) + " aware that they need to talk in their natural voice for 5 minutes?", font=("Times New Roman", 11))
    caregiver_informed_label.pack()

    MODES = [("Yes", 1), ("No", 0)]

    caregiver_informed_var = tk.IntVar()
    caregiver_informed_var.set(0) # initialize
    for text, mode in MODES:
        caregiver_informed_button = ttk.Radiobutton(tab1, text=text, variable=caregiver_informed_var, value=mode)
        caregiver_informed_button.pack(anchor='w')

    # record voice label
    record_voice_label = ttk.Label(tab1, text="If ready, you can start recording the " + str(role) + "'s voice.", font=("Times New Roman", 11))
    record_voice_label.pack()

    
    elapsed_time_label = tk.Label(tab1, fg="dark green", font=("Times New Roman", 20))
    elapsed_time_label.pack()

    if role == 'caregiver':
        location = '2-Training//singles//1-caregiver//'
    else:
        location = '2-Training//singles//2-patient//'

    try:
        os.makedirs(location)
    except:
        shutil.rmtree(location)
        os.makedirs(location)

    def countdown(count):
        # change text in label       
        display = convert(count) 
        elapsed_time_label['text'] = 'Remaining time: ' + str(display)

        if count > 0:
            # call countdown again after 1000ms (1s)
            root.after(1000, countdown, count-1)
        if count <= 0:
            elapsed_time_label['text'] = 'Recording finished'
    

    def voice_record():
        if voice_record_button['text'] == 'Start Recording' or  voice_record_button['text'] == 'Restart Recording':
            x = threading.Thread(target=countdown, args=(RECORD_SECONDS,))
            y = threading.Thread(target=record_single_session, args=(CHUNK, FORMAT, CHANNELS, RATE, \
                        RECORD_SECONDS, role, location))
            x.start()
            y.start()
            voice_record_button['text'] = 'Restart Recording'
        else:
            pass
            
    voice_record_button = tk.Button(tab1, text='Start Recording', width=25, command=voice_record)
    voice_record_button.pack()

def record_single_session(CHUNK, FORMAT, CHANNELS, RATE, RECORD_SECONDS, role, location):
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

        try:
            os.remove(location + role + '.wav')
            print('Deleted previously generated file with the same name.')
        except:
            pass
        os.rename(WAVE_OUTPUT_FILENAME, location + role + '.wav')

        return WAVE_OUTPUT_FILENAME

def replace_special_chars(z, special_chars, new_char):
    removeSpecialChars = z.translate({ord(c): new_char for c in special_chars})
    return removeSpecialChars

def convert(seconds): 
    seconds = seconds % (24 * 3600) 
    hour = seconds // 3600
    seconds %= 3600
    minutes = seconds // 60
    seconds %= 60
      
    return "%d:%02d:%02d" % (hour, minutes, seconds) 

root = Root()
root.title("Speaker Identification")
root.geometry("500x500+200+200")
root.mainloop()