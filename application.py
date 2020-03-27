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
from pydub.playback import play

import matlab.engine

import logging
import threading
import time
import ctypes

import speech_recognition as sr

CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
RECORD_SECONDS = 300

record_threads_caregiver = []
timer_caregiver_flag = True
record_threads_patient = []
timer_patient_flag = True

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
        tab5 = ttk.Frame(self)

        self.add(tab1, text = "Collect Caregiver's Voice")
        self.add(tab2, text = "Collect Patient's Voice")
        self.add(tab3, text = "Train the Model")
        self.add(tab4, text = "Test the Model on Caregiver")
        self.add(tab5, text = "Test the Model on Patient")

        voice_record_button(tab1, 'caregiver')
        voice_record_button(tab2, 'patient')

        train_sid_button(tab3)
        test_sid_button(tab4, 'caregiver')
        test_sid_button(tab5, 'patient')

        try:
            os.mkdir('2-Training//singles//0-nonFamily//')
        except:
            shutil.rmtree('2-Training//singles//0-nonFamily//')
            os.mkdir('2-Training//singles//0-nonFamily//')

def get_id(my_thread): 
  
    # returns id of the respective thread 
    if hasattr(my_thread, '_thread_id'): 
        return my_thread._thread_id 
    for id, thread in threading._active.items(): 
        if thread is my_thread: 
            return id
   
def raise_exception(my_thread): 
    thread_id = get_id(my_thread) 
    res = ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id, 
            ctypes.py_object(SystemExit)) 
    if res > 1: 
        ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id, 0) 
        print('Exception raise failure') 

def test_sid_button(tab, role):

    location = get_location(role)
    eng = matlab.engine.start_matlab()

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
        last = 10
        if voice_record_button['text'] == 'Start Recording' or  voice_record_button['text'] == 'Restart Recording':
            x = threading.Thread(target=countdown, args=(last, ))
            y = threading.Thread(target=record_single_session, args=(CHUNK, FORMAT, CHANNELS, RATE, \
                        last, role, location, ))
            x.start()
            y.start()
            voice_record_button['text'] = 'Restart Recording'
        else:
            pass

    def play_voice():
        filelist = os.listdir(location)
        fname = location + filelist[0]
        voice = AudioSegment.from_wav(fname)
        play(voice)


    def sid():
        
        if not len(os.listdir(location)) == 0:
            try:
                fname = location + role + '.wav'
                #print('speaker identification is initialized...')
                
                id = eng.PCR_main(fname, nargout=1)
                print(id)
                if id == 1:
                    sid_label['text'] = 'Caregiver Detected'
                elif id == 2:
                    sid_label['text'] = 'Patient Detected'
                else:
                    sid_label['text'] = 'Please let the ' + role + ' speak again and test' 

            except:
                print('error - please re-record your voice.')
            

    # the overview label
    overview_label = ttk.Label(tab, text="Here we test the speaker identification model.", font=("Times New Roman", 11))
    overview_label.pack()

    overview_label = ttk.Label(tab, text="Please allow the speaker to say something in their natural voice.", font=("Times New Roman", 11))
    overview_label.pack()

    elapsed_time_label = tk.Label(tab, fg="dark green", font=("Times New Roman", 20))
    elapsed_time_label.pack()

    voice_record_button = tk.Button(tab, text='Start Recording', width=25, command=voice_record)
    voice_record_button.pack()


    listen_label = ttk.Label(tab, text="You can listen to the sample you just collected for testing purposes.", font=("Times New Roman", 11))
    listen_label.pack()

    # the checklist label
    checklist_label = ttk.Label(tab, text="Please check the following items.", font=("Times New Roman", 11))
    checklist_label.pack()

    placeholder_label = tk.Label(tab, fg="dark green", font=("Times New Roman", 20))
    placeholder_label.pack()

    voice_replay_button = tk.Button(tab, text='Play Recorded Sample', width=25, command=play_voice)
    voice_replay_button.pack()


    # only speaker's voice in the sample
    only_speaker_label = ttk.Label(tab, text="Is the " + str(role) + " the only person whose voice is recorded in the sample?", font=("Times New Roman", 11))
    only_speaker_label.pack()

    MODES = [("Yes", 1), ("No", 0)]

    only_speaker_var = tk.IntVar()
    only_speaker_var.set(0) # initialize
    for text, mode in MODES:
        only_speaker_button = ttk.Radiobutton(tab, text=text, variable=only_speaker_var, value=mode)
        only_speaker_button.pack(anchor='w')

    # full ten seconds
    only_speaker_full_label = ttk.Label(tab, text="Did the " + str(role) + " speak incessantly, or almost incessantly, in the recorded sample?", font=("Times New Roman", 11))
    only_speaker_full_label.pack()

    MODES = [("Yes", 1), ("No", 0)]

    only_speaker_full_var = tk.IntVar()
    only_speaker_full_var.set(0) # initialize
    for text, mode in MODES:
        only_speaker_full_button = ttk.Radiobutton(tab, text=text, variable=only_speaker_full_var, value=mode)
        only_speaker_full_button.pack(anchor='w')

    # Another label - confirmation
    confirmation_label = ttk.Label(tab, text="If you have checked the aforementioned items, you can test the speaker identification model.", font=("Times New Roman", 11))
    confirmation_label.pack()

    # SID label
    sid_label = tk.Label(tab, fg="dark green", font=("Times New Roman", 20))
    sid_label.pack()

    test_button = tk.Button(tab, text='Test Speaker Identification Module', command=sid, width=25)
    test_button.pack()



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
        if voice_record_button['text'] == 'Start Recording' or voice_record_button['text'] == 'Restart Recording':
            global record_threads_patient
            global record_threads_caregiver
            try:
                if role == 'caregiver':
                    raise_exception(record_threads_caregiver[0])
                    record_threads_caregiver[0]._is_stopped = True
                    record_threads_caregiver[0].join(timeout=1)
                    #num_threads[0].join()
                    print('terminated the previous timer for ' + role)
                
                    raise_exception(record_threads_caregiver[1])
                    record_threads_caregiver[1]._is_stopped = True
                    #num_threads[1].join()
                    record_threads_caregiver[1].join(timeout=1)
                    print('terminated the previous recording for ' + role)
                    record_threads_caregiver = []

                else:
                    raise_exception(record_threads_patient[0])
                    #num_threads[0].join()
                    print('terminated the previous timer for ' + role)
                
                    raise_exception(record_threads_patient[1])
                    #num_threads[1].join()
                    print('terminated the previous recording for ' + role)
                    record_threads_patient = []

            except Exception as e:
                print(e)


            x = threading.Thread(target=countdown, args=(RECORD_SECONDS, ), daemon=True)
            y = threading.Thread(target=record_single_session, args=(CHUNK, FORMAT, CHANNELS, RATE, \
                        RECORD_SECONDS, role, location,), daemon=True)

            if role == 'caregiver': 
                record_threads_caregiver.append(x)
                record_threads_caregiver.append(y)

                print(record_threads_caregiver)
            if role == 'patient': 
                record_threads_patient.append(x)
                record_threads_patient.append(y)

                print(record_threads_patient)

            voice_record_button['text'] = 'Restart Recording'

            x.start()
            y.start()
            
        else:
            print("nothing")
            
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
        print(location + role + '.wav')
        return location + role + '.wav'

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

def get_location(role):
    if role == 'caregiver':
        location = '3-Testing//singles//1-caregiver//'
    else:
        location = '3-Testing//singles//2-patient//'

    try:
        os.makedirs(location)
    except:
        shutil.rmtree(location)
        os.makedirs(location)

    return location

    

def voice_activity_detection(location):
    r = sr.Recognizer()

    while True:

        if len(os.listdir(location)) == 0:
            continue
    
        for filename in os.listdir(location):
            try:
                with sr.AudioFile(location + filename) as source:
                    audio = r.record(source)
            except:
                continue

            try:
                transcription = r.recognize_google(audio)
                print(location + filename + ' is speech.')
                print(transcription)

                return 1

            except: # intelligible
                os.remove(location + filename)


root = Root()
root.iconbitmap('icon.ico')
root.title("Speaker Identification")
root.geometry("800x500+200+200")
root.mainloop()