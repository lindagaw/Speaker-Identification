from threading import Thread
import threading
import time

import random
import string
import ctypes 

class Killable_Thread(threading.Thread):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._stop_event = threading.Event()

    def stop(self):
        self._stop_event.set()

    def stopped(self):
        return self._stop_event.is_set()

    def get_id(self): 
  
        # returns id of the respective thread 
        #if hasattr(self, '_thread_id'): 
        #    return self._thread_id 
        for id, thread in threading._active.items(): 
            if thread is self: 
                return id
    
    def raise_exception(self): 
        thread_id = self.get_id() 
        res = ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id, 
              ctypes.py_object(SystemExit)) 
        if res > 1: 
            ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id, 0) 
            print('Exception raise failure') 