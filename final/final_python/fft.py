import librosa
import librosa.display
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy import signal

sr = 22050

audio_file = 'ui.wav'

def spectrogram(S,sr):
    fig, ax = plt.subplots()
    img = librosa.display.specshow(librosa.amplitude_to_db(S, ref=np.max), y_axis='log', x_axis='time', ax=ax)
    ax.set_title('Power spectrogram')
    fig.colorbar(img, ax=ax, format="%+2.0f dB")
    return img


file1 = open('ui.bin', 'wb')
file2 = open('ui.txt', 'w')
  
count = 0

landmark = 60/141

for i in range(24*118):
    time_per_frame = 1/24
    
    d = time_per_frame*i
    #loading audio file and displaying spectrogram
    x_all, sr = librosa.load(audio_file, sr=sr)
    #x_all, sr = librosa.load(audio_file, sr=sr, offset = d+2, duration = time_per_frame)
    
    fn = 0.5 * sr
    nc = 200/fn
    b, a = signal.butter(1, nc, btype='high', analog=False)
    x_all = signal.filtfilt(b, a, x_all)
    
    x_fft = np.abs(librosa.stft(x_all))
    
    
    #spectrogram(x_fft, sr)
    #break
    #L = librosa.get_duration(audio_file, sr=sr)
    
    #df = pd.DataFrame(x_fft)
    #df.to_excel('matrix.xlsx', index=False)
    
    #(x_fft, sr)
    
    #print(len(x_fft))
    x_fft = x_fft[0:255]
    T = 1
    HZ = 8192/4
    x = np.linspace(0.0, HZ, len(x_fft))

    x_fft = np.average(x_fft, axis = 1)
    
    
    '''
    plt.figure(2)
    plt.plot(x,np.abs(x_fft))
    plt.title("Complex Magnitude of fft Spectrum")
    plt.xlabel("f (Hz)")
    plt.ylabel("|fft(X)|")
    #plt.xlim((0,1000))
    '''
    
    binned = np.histogram(x_fft, bins=60, weights = x_fft)[0]
    
    
    max_c = 40
    c = 0
    
    #print(binned)
    beattime = 60/141
    count = (60/141)*i/24
    
    if count > landmark:
        file1.write(int('1').to_bytes(1, "big"))
        file2.write("1")
        landmark = beattime + landmark
    else:
        file2.write('0')
        
    file2.write(',')
    
    for bi in binned:
        # file1.write(str(int(bi)))

        file1.write(int(bi).to_bytes(1, "big"))
        
        file2.write(str(int(bi)))
        
        if c<39:
            file2.write(',')
        c+=1
    
    file2.write('\n')
          
    
file1.close()
