from lib import *
from killable_thread import Killable_Thread
from clock import Clock

CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
RECORD_SECONDS = 300

class Voice_Collection_Tab(ttk.Frame):

    
    def __init__(self, parent, role)


def dafae():
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