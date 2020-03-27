from lib import *
from killable_thread import Killable_Thread
from clock import Clock

CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
RECORD_SECONDS = 300


class Voice_Collection_Tab(ttk.Frame):

    def upload_cloud(self):
        if len(os.listdir(self.location)) == 0:
            self.upload_cloud_label['text'] = 'please record the speech signal from ' + self.role + ' before uploading'
        else:
            print('uploading the following file to the cloud...')
            for item in os.listdir(self.location):
                if 'caregiver' in item or 'patient' in item:
                    print('uploading ' + item)
                    self.upload_cloud_label['text'] = 'uploaded the training clip of ' + self.role

    def pause(self):
        if self.pause_record_button['text'] == 'Pause':
            self.pause_record_label['text'] = 'Recording Paused'
            self.pause_record_button['text'] = 'Continue'
            self.is_paused = True
        else:
            self.pause_record_label['text'] = ' '
            self.pause_record_button['text'] = 'Pause'
            self.is_paused = False

    def countdown(self, count):
        # change text in label
        
        while count >= 0:
            if self.is_paused:
                time.sleep(1)
            else:
                display = convert(count)
                self.elapsed_time_label['text'] = 'Remaining time: ' + str(display)
                time.sleep(1)
                count -= 1
        

        self.elapsed_time_label['text'] = 'Recording Ended'

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
            y = Killable_Thread(target=self.record_single_session, args=(CHUNK, FORMAT, CHANNELS, RATE, \
                        RECORD_SECONDS, self.role, location,), daemon=True)

            print('created recording thread ' + str(y.getName()))
            self.voice_record_button['text'] = 'Restart Recording'

            x.start()
            y.start()

            self.active_threads.append(x.getName())
            self.active_threads.append(y.getName())

            
        else:
            print("nothing")
        
    def __init__(self, parent, role):
        tk.Frame.__init__(self, parent)

        self.active_threads = []
        self.remaining = RECORD_SECONDS

        self.is_paused = False
        
        
        self.parent = parent
        self.role = role
        self.overview_label = ttk.Label(self, text="Here we collect the " + str(role) + "'s voice.", font=("Times New Roman", 11))
        self.overview_label.pack()

        # the checklist label
        self.checklist_label = ttk.Label(self, text="Please check the following items.", font=("Times New Roman", 11))
        self.checklist_label.pack()

        # only caregiver in the room label
        self.only_role_label = ttk.Label(self, text="Is the " + str(role) + " the only person in the room, except the voice collector?", font=("Times New Roman", 11))
        self.only_role_label.pack()

        MODES = [("Yes", 1), ("No", 0)]

        self.only_role_var = tk.IntVar()
        self.only_role_var.set(0) # initialize
        for text, mode in MODES:
            self.only_role_button = ttk.Radiobutton(self, text=text, variable=self.only_role_var, value=mode)
            self.only_role_button.pack(anchor='w')

        # caregiver informed label
        self.role_informed_label = ttk.Label(self, text="Is the " + str(role) + " aware that they need to talk in their natural voice for 5 minutes?", font=("Times New Roman", 11))
        self.role_informed_label.pack()

        self.role_informed_var = tk.IntVar()
        self.role_informed_var.set(0) # initialize

        for text, mode in MODES:
            self.role_informed_button = ttk.Radiobutton(self, text=text, variable=self.role_informed_var, value=mode)
            self.role_informed_button.pack(anchor='w')


        # record voice label
        self.record_voice_label = ttk.Label(self, text="If ready, you can start recording the " + str(role) + "'s voice.", font=("Times New Roman", 11))
        self.record_voice_label.pack()

        self.elapsed_time_label = tk.Label(self, fg="dark green", font=("Times New Roman", 20))
        self.elapsed_time_label.pack()

        if role == 'caregiver':
            self.location = '..//2-Training//singles//1-caregiver//'
        else:
            self.location = '..//2-Training//singles//2-patient//'

        try:
            os.makedirs(self.location)
        except:
            shutil.rmtree(self.location)
            os.makedirs(self.location)

        # voice record button
        self.voice_record_button = tk.Button(self, text='Start Recording', width=25, command=self.voice_record)
        self.voice_record_button.pack()

        self.pause_record_label = tk.Label(self, fg="dark green", text = ' ', font=("Times New Roman", 20))
        self.pause_record_label.pack()

        # pause recording button
        self.pause_record_button = tk.Button(self, text='Pause', width=25, command=self.pause)
        self.pause_record_button.pack()


        self.upload_cloud_label = tk.Label(self, fg="dark green", text = ' ', font=("Times New Roman", 20))
        self.upload_cloud_label.pack()

        # pause recording button
        self.upload_cloud_button = tk.Button(self, text='Upload to Cloud', width=25, command=self.upload_cloud)
        self.upload_cloud_button.pack()


    def record_single_session(self, CHUNK, FORMAT, CHANNELS, RATE, RECORD_SECONDS, role, location):
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
                if self.is_paused:
                    time.sleep(1)
                else:
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
        location = '..//3-Testing//singles//1-caregiver//'
    else:
        location = '..//3-Testing//singles//2-patient//'


    return location

