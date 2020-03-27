from lib import *
from tab import *
from train_tab import *
from test_tab import *

training_caregiver_wav = '..//2-Training//singles//1-caregiver//'
training_patient_wav = '..//2-Training//singles//2-patient//'
testing_caregiver_wav = '..//3-Testing//singles//1-caregiver//'
testing_patient_wav = '..//3-Testing//singles//2-patient//'

wav_dirs = [training_caregiver_wav, training_patient_wav, testing_caregiver_wav, testing_patient_wav]

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

        tab1 = Voice_Collection_Tab(self, 'caregiver')
        tab2 = Voice_Collection_Tab(self, 'patient')
        tab3 = Train_SID_Tab(self)

        tab4 = Test_SID_Tab(self, 'caregiver')
        tab5 = Test_SID_Tab(self, 'patient')

        self.add(tab1, text = "Collect Caregiver's Voice")
        self.add(tab2, text = "Collect Patient's Voice")
        self.add(tab3, text = "Train the Model")
        self.add(tab4, text = "Test the Model on Caregiver")
        self.add(tab5, text = "Test the Model on Patient")

        #voice_record_button(tab1, 'caregiver')
        #voice_record_button(tab2, 'patient')

        #train_sid_button(tab3)
        #test_sid_button(tab4, 'caregiver')
        #test_sid_button(tab5, 'patient')

        try:
            os.mkdir('..//2-Training//singles//0-nonFamily//')
        except:
            shutil.rmtree('..//2-Training//singles//0-nonFamily//')
            os.mkdir('..//2-Training//singles//0-nonFamily//')


def reset_folders():
    for wav_dir in wav_dirs:
        try:
            os.mkdir(wav_dir)
        except:
            shutil.rmtree(wav_dir)
            os.mkdir(wav_dir) 

if __name__ == "__main__":
    root = Root()
    #root.iconbitmap('icon.ico')
    root.title("Speaker Identification")
    root.geometry("800x500+200+200")
    root.mainloop()  