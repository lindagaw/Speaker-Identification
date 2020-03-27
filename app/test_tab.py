from lib import *
from killable_thread import *

CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
RECORD_SECONDS = 10

class Test_SID_Tab(ttk.Frame):

    def sid(self):
        location = get_location(self.role)
        if not len(os.listdir(location)) == 0:
            try:
                fname = location + self.role + '.wav'
                #print('speaker identification is initialized...')
                id = self.eng.SID_main(fname[4:len(fname)], nargout=1)
                print(id)
                if id == 1:
                    self.sid_label['text'] = 'Caregiver Detected'
                elif id == 2:
                    self.sid_label['text'] = 'Patient Detected'
                else:
                    self.sid_label['text'] = 'Please let the ' + self.role + ' speak again and test' 

            except Exception as e:
                print(e)
                print('error - please re-record your voice.')

    def play_voice(self):
        location = get_location(self.role)
        filelist = os.listdir(location)
        fname = location + filelist[0]
        voice = AudioSegment.from_wav(fname)
        play(voice)

    def voice_record(self):
        location = get_location(self.role)
        if self.voice_record_button['text'] == 'Start Recording' or self.voice_record_button['text'] == 'Restart Recording':

            if not len(self.active_threads) == 0:
                for item in threading.enumerate():
                    if item.getName() in self.active_threads:
                        self.active_threads.remove(item.getName())
                        item.raise_exception()
                        print('thread ' + str(item) + ' is stopped.')

                self.remaining = RECORD_SECONDS

            x = Killable_Thread(target=self.countdown, args=(self.remaining, ), daemon=True)
            print('created timer thread ' + str(x.getName()))
            y = Killable_Thread(target=record_single_session, args=(CHUNK, FORMAT, CHANNELS, RATE, \
                        RECORD_SECONDS, self.role, location,), daemon=True)

            print('created recording thread ' + str(y.getName()))
            self.voice_record_button['text'] = 'Restart Recording'

            x.start()
            y.start()

            self.active_threads.append(x.getName())
            self.active_threads.append(y.getName())
        else:
            print("nothing")

    def countdown(self, count):
        # change text in label
        
        while count > 0:
            if self.is_paused:
                time.sleep(1)
            else:
                display = convert(count)
                self.elapsed_time_label['text'] = 'Remaining time: ' + str(display)
                time.sleep(1)
                count -= 1

        self.elapsed_time_label['text'] = 'Recording finished'

    def __init__(self, parent, role):

        tk.Frame.__init__(self, parent)
        self.role = role
        self.active_threads = []
        self.is_paused = False
        self.remaining = RECORD_SECONDS
        self.eng = matlab.engine.start_matlab()

        # the overview label
        self.overview_label = ttk.Label(self, text="Here we test the speaker identification model.", font=("Times New Roman", 11))
        self.overview_label.pack()

        self.overview_label = ttk.Label(self, text="Please allow the speaker to say something in their natural voice.", font=("Times New Roman", 11))
        self.overview_label.pack()

        self.elapsed_time_label = tk.Label(self, fg="dark green", font=("Times New Roman", 20))
        self.elapsed_time_label.pack()

        self.voice_record_button = tk.Button(self, text='Start Recording', width=25, command=self.voice_record)
        self.voice_record_button.pack()


        self.listen_label = ttk.Label(self, text="You can listen to the sample you just collected for testing purposes.", font=("Times New Roman", 11))
        self.listen_label.pack()

        # the checklist label
        self.checklist_label = ttk.Label(self, text="Please check the following items.", font=("Times New Roman", 11))
        self.checklist_label.pack()

        self.placeholder_label = tk.Label(self, fg="dark green", font=("Times New Roman", 20))
        self.placeholder_label.pack()

        self.voice_replay_button = tk.Button(self, text='Play Recorded Sample', width=25, command=self.play_voice)
        self.voice_replay_button.pack()


        # only speaker's voice in the sample
        self.only_speaker_label = ttk.Label(self, text="Is the " + str(role) + " the only person whose voice is recorded in the sample?", font=("Times New Roman", 11))
        self.only_speaker_label.pack()

        MODES = [("Yes", 1), ("No", 0)]

        self.only_speaker_var = tk.IntVar()
        self.only_speaker_var.set(0) # initialize
        for text, mode in MODES:
            self.only_speaker_button = ttk.Radiobutton(self, text=text, variable=self.only_speaker_var, value=mode)
            self.only_speaker_button.pack(anchor='w')

        # full ten seconds
        self.only_speaker_full_label = ttk.Label(self, text="Did the " + str(role) + " speak incessantly, or almost incessantly, in the recorded sample?", font=("Times New Roman", 11))
        self.only_speaker_full_label.pack()

        self.only_speaker_full_var = tk.IntVar()
        self.only_speaker_full_var.set(0) # initialize
        for text, mode in MODES:
            self.only_speaker_full_button = ttk.Radiobutton(self, text=text, variable=self.only_speaker_full_var, value=mode)
            self.only_speaker_full_button.pack(anchor='w')

        # Another label - confirmation
        self.confirmation_label = ttk.Label(self, text="If you have checked the aforementioned items, you can test the speaker identification model.", font=("Times New Roman", 11))
        self.confirmation_label.pack()

        # SID label
        self.sid_label = tk.Label(self, fg="dark green", font=("Times New Roman", 20))
        self.sid_label.pack()

        self.test_button = tk.Button(self, text='Test Speaker Identification Module', command=self.sid, width=25)
        self.test_button.pack()

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
        location = '..//3-Testing//singles//1-caregiver//'
    else:
        location = '..//3-Testing//singles//2-patient//'

    return location

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

    print("Recording finished...")

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
    print(location + role + '.wav' + ' is successfully created -- ' + str(os.path.isfile(location + role + '.wav')))

    return location + role + '.wav'
