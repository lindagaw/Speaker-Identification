import pickle
import numpy as np

rf = 'models//finalized_random_forest.sav'
svm = 'models//finalized_svm.sav'

loaded_model = pickle.load(open(svm, 'rb'))
