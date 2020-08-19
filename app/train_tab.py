from lib import *
from killable_thread import Killable_Thread

CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 44100
RECORD_SECONDS = 300

class Train_SID_Tab(ttk.Frame):

    def progress_bar(self, yield_time):
        # singles = 'Speaker-Identification\speaker_id_module\SpeakerID\singles'
        #singles = '..//speaker_id_module//speakerID//singles'
        singles = '..//2-Training//singles'
        for folder in os.listdir(singles):
            if len(singles+'//'+folder) == 0:
                shutil.rmtree(singles+'//'+dir)

        original_num = len(os.listdir(singles))

        start_waxing = False
        start_waning = False
        self.progress['value'] = 0
            
        while True:
            current_num = len(os.listdir(singles))

            if current_num > original_num:
                start_waxing = True
                original_num = current_num
                time.sleep(yield_time)
                self.progress['value'] = self.progress['value'] + 3

            elif current_num < original_num:
                start_waning = True
                original_num = current_num
                time.sleep(yield_time)
                self.progress['value'] = self.progress['value'] + 3

            else:
                if not (start_waning and start_waxing):
                    continue
                else:
                    self.progress['value'] = self.max_files*5
                    src = '..//speaker_id_module//speakerID//models_1024.mat'
                    dest = '..//2-Training//models_1024.mat'
                    shutil.copyfile(src, dest)
                    break
        

    def check(self, yield_time):
        count = 0
        try:
            os.remove("..//2-Training//models_1024.mat")
        except Exception as e:
            print(e)

        while True:
            if not os.path.exists("..//2-Training//models_1024.mat"):
                count += 1
                self.training_label['text'] = 'Training' + '.' * (count % 4)
                time.sleep(yield_time)
            else:
                self.training_label['text'] = 'Finished'

    def train(self):
        print('Starting the training module initialization...')
        eng = matlab.engine.start_matlab()
        print('training module initialization finished...')
        if self.training_button['text'] == 'Start Training' or self.training_button['text'] == 'Restart Training':
            #subprocess.Popen([r"cmd"])
            #subprocess.Popen([r"..//2-Training//M2FEDTraining.exe"])
            eng.SID_train(nargout=0)
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
        
        self.max_files = (len(os.listdir('..//speaker_id_module//speakerID/singles')) - 1) * 2 * 27
        
        self.progress = Progressbar(self, orient='horizontal', length=self.max_files*5, mode='determinate')
        self.progress.pack()
        
        self.progressbar_label = tk.Label(self, fg="dark green", font=("Times New Roman", 20))
        self.progressbar_label.pack()


        self.training_button = tk.Button(self, text='Start Training', width=25, command=self.train_and_check)
        self.training_button.pack()


                

