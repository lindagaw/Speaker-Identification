from lib import *
from killable_thread import Killable_Thread
import contextlib
import sys

from train_SID_python import *

CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 44100
RECORD_SECONDS = 300

class Train_SID_Tab(ttk.Frame):

    def progress_bar(self, yield_time):

        singles = '2-Training//singles//'

        single_dirs = []

        for folder in os.listdir(singles):

            registered_speaker_folder = singles + folder
            single_dirs.append(registered_speaker_folder)
            try:
                shutil.rmtree(registered_speaker_folder)
            except:
                pass
            os.makedirs(registered_speaker_folder)


        try:
            shutil.rmtree('..//models//')
        except:
            pass
        os.makedirs('..//models//')

        original_num = 180 + 180 + 11

        self.progress['value'] = 0
        
        previous_num = 0
        while True:
            
            #
            current_num = len(os.listdir(single_dirs[0])) + len(os.listdir(single_dirs[1])) + len(os.listdir('..//models//'))

            if current_num < original_num:
                time.sleep(yield_time)
                self.progress['value'] = current_num - previous_num

                previous_num = current_num

            else:
                self.progress['value'] = 1000

                break
    

    def check(self, yield_time):
        model_dirs = '..//models//'

        count = 0
        try:
            shutil.rmtree(model_dirs)
        except Exception as e:
            print(e)

        while True:

            if not len(os.isdir(model_dirs)) == 11:
                count += 1
                self.training_label['text'] = 'Training' + '.' * (count % 4)
                time.sleep(yield_time)
            else:
                self.training_label['text'] = 'Finished'

    def train(self):
        print('Starting the training module initialization...')

        print('training module initialization finished...')
        if self.training_button['text'] == 'Start Training' or self.training_button['text'] == 'Restart Training':


            start_SID_train()
            self.training_button['text'] = 'Restart Training'
        else:
            pass

    def train_and_check(self):
        x = Killable_Thread(target=self.train, args=())
        y = Killable_Thread(target=self.check, args=(1,))
        z = Killable_Thread(target=self.progress_bar, args=(1,))
        x.start()
        y.start()
        z.start()

    def __init__(self, parent):
        tk.Frame.__init__(self, parent)
        # the overview label
        self.overview_label = ttk.Label(self, text="Here we train the speaker identification model.", font=("Times New Roman", 11))
        self.overview_label.pack()

        # the checklist label
        self.checklist_label = ttk.Label(self, text="Please check the following items.", font=("Times New Roman", 11))
        self.checklist_label.pack()

        # caregiver's voice
        self.caregiver_label = ttk.Label(self, text="Is the caregiver's voice collected?", font=("Times New Roman", 11))
        self.caregiver_label.pack()

        MODES = [("Yes", 1), ("No", 0)]

        self.caregiver_var = tk.IntVar()
        self.caregiver_var.set(0) # initialize
        for text, mode in MODES:
            self.caregiver_button = ttk.Radiobutton(self, text=text, variable=self.caregiver_var, value=mode)
            self.caregiver_button.pack(anchor='w')

        # patient's voice
        self.patient_label = ttk.Label(self, text="Is the patient's voice collected?", font=("Times New Roman", 11))
        self.patient_label.pack()

        self.patient_var = tk.IntVar()
        self.patient_var.set(0) # initialize
        for text, mode in MODES:
            self.patient_button = ttk.Radiobutton(self, text=text, variable=self.patient_var, value=mode)
            self.patient_button.pack(anchor='w')

        self.training_label = tk.Label(self, fg="dark green", font=("Times New Roman", 20))
        self.training_label.pack()

        if not os.path.isdir('..//speaker_id_module//speakerID//singles'):
            os.makedirs('..//speaker_id_module//speakerID//singles')
        
        self.max_files = (len(os.listdir('..//speaker_id_module//speakerID//singles')) ) * 2 * 27

        print(os.listdir('..//speaker_id_module//speakerID//singles'))
        
        self.progress = Progressbar(self, orient='horizontal', length=self.max_files*5, mode='determinate')
        self.progress.pack()
        
        self.progressbar_label = tk.Label(self, fg="dark green", font=("Times New Roman", 20))
        self.progressbar_label.pack()


        self.training_button = tk.Button(self, text='Start Training', width=25, command=self.train_and_check)
        self.training_button.pack()


                

