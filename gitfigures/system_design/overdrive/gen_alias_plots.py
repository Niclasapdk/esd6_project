import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft, fftfreq

def genplot(freq_str):
    freq = 0
    for i, v in enumerate(freq_str.split("k")):
        freq += float(v+".0")*(10**(-i))
    freq *= 1000
    path = f'./overdrive_non_linear_long_{freq_str}_test.csv'
    with open(path, "r", encoding="ISO-8859-1") as f:
        l = f.readlines()
        for x in l:
            if x.startswith("#Sample rate:"):
                fs = float(x.split(": ")[1][:-3])

    df = pd.read_csv(path,skiprows=19,header=1,encoding="ISO-8859-1")
    # Assuming columns are named 'time' and 'voltage'
    time = df.iloc[:,0].values
    voltage = df.iloc[:,1].values

    # Number of samples
    N = len(voltage)

    # Apply FFT
    yf = fft(voltage, N)
    xf = fftfreq(N, 1/fs)

    # Take only positive frequencies
    idx = xf >= 0
    xf = xf[idx]
    yf = np.abs(yf[idx])
    yf = yf/np.max(yf)
    yf = 20*np.log10(yf)
    # Plot
    plt.figure(figsize=(10, 5))
    # plt.plot(xf, yf)
    plt.semilogx(xf, yf)
    plt.xticks(fontsize=14)
    plt.yticks(fontsize=14)
    plt.xlim(1,22050)
    plt.ylim(-100,10)
    plt.plot((freq, freq), (-100, -70), "r--")
    plt.legend(["FFT", "Fundamental"], fontsize=16)
    plt.title("FFT of Overdrive Output", fontsize=18)
    plt.xlabel("Frequency (Hz)", fontsize=16)
    plt.ylabel("Amplitude (dB)", fontsize=16)
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f"fft_overdrive_output_{freq_str}.png", dpi=300)

freqs = ["0k6", "1k", "2k5", "5k"]
for f in freqs:
    genplot(f)
