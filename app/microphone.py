from lib import *
from killable_thread import Killable_Thread

import os
import shutil

CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 44100
RECORD_SECONDS = 300

class Microphone_Tab(ttk.Frame):


    def detect_mic(self):
        p = pyaudio.PyAudio()
        default_device = p.get_default_input_device_info()

        self.confirm_status['text'] = default_device['name']

        print(default_device['name'])

        print(default_device)
    

    def __init__(self, parent):
        tk.Frame.__init__(self, parent)
        # the overview label
        self.overview_label = ttk.Label(self, text="In this tab, you will make sure that the correct microphone is being used.", font=("Times New Roman", 11))
        self.overview_label.pack()

        self.placeholder_1_label = ttk.Label(self, text=" ", font=("Times New Roman", 11))
        self.placeholder_1_label.pack()

        # the warning label
        self.warning_label = ttk.Label(self, text="DO NOT proceed to the next tab before confirming that the correct microphone is being used.", font=("Times New Roman", 11))
        self.warning_label.pack()

        self.placeholder_2_label = ttk.Label(self, text=" ", font=("Times New Roman", 11))
        self.placeholder_2_label.pack()

       
        # the confirmation label
        self.confirm = ttk.Label(self, text="Press the following button to detect the active microphone.", font=("Times New Roman", 11))
        self.confirm.pack()

        self.finish = tk.Button(self, text='Detect Microphone', width=25, command=self.detect_mic)
        self.finish.pack()


        # the status label
        self.confirm_status = tk.Label(self, fg="dark green", font=("Times New Roman", 20))
        self.confirm_status.pack()

        #
        self.description_status = ttk.Label(self, text="If the displayed microphone's name is not Microphone (USB audio CODEC),", font=("Times New Roman", 11))
        self.description_1_status = ttk.Label(self, text = "please plug in or re-plug in the XML microphone that came with the laptop.", font=("Times New Roman", 11))
        self.description_status.pack()

        self.mic_good = ttk.Label(self, text="Before proceeding, you must make sure the correct microphone is being used.", font=("Times New Roman", 11))
        self.mic_good.pack()

        MODES = [("Yes", 1), ("No", 0)]
        self.mic_var = tk.IntVar()
        self.mic_var.set(0) # initialize
        for text, mode in MODES:
            self.mic_button = ttk.Radiobutton(self, text=text, variable=self.mic_var, value=mode)
            self.mic_button.pack(anchor='w')


