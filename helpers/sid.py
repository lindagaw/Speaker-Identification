from transformers import Wav2Vec2FeatureExtractor, WavLMForXVector
from datasets import load_dataset
import torch
import pretty_errors
from audio2numpy import open_audio
import warnings

from termcolor import colored

warnings.filterwarnings("ignore")

feature_extractor = Wav2Vec2FeatureExtractor.from_pretrained("microsoft/wavlm-base-plus-sv")
model = WavLMForXVector.from_pretrained("microsoft/wavlm-base-plus-sv")

def compare(wav1_path, wav2_path, threshold):
    wav1, sampling_rate = open_audio(wav1_path)
    wav2, sampling_rate = open_audio(wav2_path)
    inputs = feature_extractor(
        [wav1, wav2], sampling_rate=sampling_rate, return_tensors="pt", padding=True
    )

    with torch.no_grad():
        embeddings = model(**inputs).embeddings

    embeddings = torch.nn.functional.normalize(embeddings, dim=-1).cpu()

    # the resulting embeddings can be used for cosine similarity-based retrieval
    cosine_sim = torch.nn.CosineSimilarity(dim=-1)
    similarity = cosine_sim(embeddings[0], embeddings[1])
      # the optimal threshold is dataset-dependent
    if similarity < threshold:
        print(colored("Speakers are not the same!", "red"))
        return False
    else:
        print(colored("Speakers are the same!", "green"))
        return True

    print(similarity.item())


#wav1_path = "audios//1-caregiver//caregiver.wav"
#wav2_path = "audios//2-patient//patient.wav"  # change to the correct path to your file accordingly

#compare(wav1_path, wav2_path, threshold=0.85)
