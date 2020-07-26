from lib import *
from killable_thread import Killable_Thread

import os
import shutil

CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 44100
RECORD_SECONDS = 300

class Confirmation_Tab(ttk.Frame):

    def transfer_model(self):
        newly_trained_sid = '..//speaker_id_module//SpeakerID//models_1024.mat'
        dest = '..//..//Patient-Caregiver-Relationship//speaker_id_module//SpeakerID//models_1024.mat'
        if not os.path.isfile(newly_trained_sid):
            self.confirm_status['text'] = 'You must train the SID model.'
            print('You must train the SID model.')
        else:
            try:
                os.remove(dest)
                print('deleted previous SID model')
                shutil.copyfile(newly_trained_sid, dest)
                shutil.copyfile(newly_trained_sid, '..//2-Training//models_1024.mat')
                print('successfully installed the newly trained SID model')

                self.confirm_status['text'] = 'The newly trained SID model is installed. You may close this application window.'
            except Exception as e:
                self.confirm_status['text'] = 'The SID model is corrputed. Please retrain the SID model.'
                print(e)


    def __init__(self, parent):
        tk.Frame.__init__(self, parent)
        # the overview label
        self.overview_label = ttk.Label(self, text="Please indicate if you are done with training the speaker identification module.", font=("Times New Roman", 11))
        self.overview_label.pack()

        # the checklist label
        self.checklist_label = ttk.Label(self, text="Please check the following items.", font=("Times New Roman", 11))
        self.checklist_label.pack()

        # caregiver's voice
        self.caregiver_label = ttk.Label(self, text="Can the SID model successfully identify the voice of the Caregiver?", font=("Times New Roman", 11))
        self.caregiver_label.pack()

        MODES = [("Yes", 1), ("No", 0)]

        self.caregiver_var = tk.IntVar()
        self.caregiver_var.set(0) # initialize
        for text, mode in MODES:
            self.caregiver_button = ttk.Radiobutton(self, text=text, variable=self.caregiver_var, value=mode)
            self.caregiver_button.pack(anchor='w')

        # patient's voice
        self.patient_label = ttk.Label(self, text="Can the SID model successfully identify the voice of the Patient?", font=("Times New Roman", 11))
        self.patient_label.pack()

        self.patient_var = tk.IntVar()
        self.patient_var.set(0) # initialize
        for text, mode in MODES:
            self.patient_button = ttk.Radiobutton(self, text=text, variable=self.patient_var, value=mode)
            self.patient_button.pack(anchor='w')


        # the confirmation label
        self.confirm = ttk.Label(self, text="Press the Confirm button to install the newly trained SID model.", font=("Times New Roman", 11))
        self.confirm.pack()

        self.finish = tk.Button(self, text='Confirm', width=25, command=self.transfer_model)
        self.finish.pack()

        # the status label
        self.confirm_status = tk.Label(self, text=" ", fg="dark green", font=("Times New Roman", 20))
        self.confirm_status.pack()



