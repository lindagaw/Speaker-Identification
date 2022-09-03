from tkinter import *
import os
import shutil
from helpers import record_single_session
from helpers import compare

class MyWindow:
    def __init__(self, win):
        self.lbl1=Label(win, text='Speaker')
        self.lbl1.place(x=100, y=50)
        self.t1=Entry()
        self.t1.place(x=200, y=50)

        self.lbl2=Label(win, text="Now record the voice of the speaker for 5 seconds by clicking the 'Record' button.")
        self.lbl2.place(x=100, y=100)

        self.b1=Button(win, text='Record Reference', command=lambda: self.record_reference(self.t1.get()))
        self.b1.place(x=100, y=150)

        self.lbl3=Label(win, text="Now let's run a test to verify that the speaker id works correctly. Record another 5-second sample.")
        self.lbl3.place(x=100, y=200)

        self.b2=Button(win, text='Record Sample', command=lambda: self.record_test())
        self.b2.place(x=100, y=250)

        self.lbl4=Label(win, text="Now performing identification...")
        self.lbl4.place(x=100, y=300)

        self.b3=Button(win, text='Verify', command=lambda: self.verify(self.t1.get()))
        self.b3.place(x=100, y=350)

        self.lbl5=Label(win, text="")
        self.lbl5.place(x=200, y=350)


    def record_reference(self, speaker):

        if os.path.isdir('audios//'+speaker):
            shutil.rmtree('audios//'+speaker)
        os.makedirs('audios//'+speaker)

        record_single_session(5, 'audios//'+speaker, speaker)

    def record_test(self):
        if os.path.isdir('audios//test'):
            shutil.rmtree('audios//test')
        os.makedirs('audios//test')
        record_single_session(5, 'audios//test', 'test')

    def verify(self, speaker):
        reference = os.path.join('audios//'+speaker, speaker + '.wav')
        sample = os.path.join('audios//test', 'test.wav')
        similarity = compare(reference, sample, 0.85)
        if similarity:
            text = 'Detected as the ' + speaker
            self.lbl5.config(text = text, fg='green')
        else:
            text = 'Detected as not the ' + speaker
            self.lbl5.config(text = text, fg='red')



window=Tk()
mywin=MyWindow(window)
window.title('Speaker Identification')
window.geometry("650x450+500+500")
window.mainloop()
