import numpy as np
import matplotlib.pyplot as plt

# Define the sample rate in Hz (e.g., 44100 for 44.1 kHz)
SAMPLE_RATE = 44100

def load_signal(filename, dtype=np.float32):
    """
    Loads raw binary data from a file.
    Adjust dtype if the C sample TYPE is not float32.
    """
    try:
        data = np.fromfile(filename, dtype=dtype)
        return data
    except Exception as e:
        print(f"Error reading {filename}: {e}")
        return None

def plot_signals(signals):
    """
    Plots multiple signals on the same figure.
    
    Parameters:
        signals (list of tuples): Each tuple contains (filename, label)
    """
    plt.figure()
    for filename, label in signals:
        data = load_signal(filename)
        if data is not None:
            t = np.arange(len(data)) / SAMPLE_RATE
            plt.plot(t, data, label=label)
    
    plt.xlabel("Time (s)")
    plt.ylabel("Amplitude")
    plt.title("Signals")
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    # List the files and labels you want to plot together.
    signals_to_plot = [
        ("input.bin", "input"),
        ("v_plus.bin", "V+")
    ]
    plot_signals(signals_to_plot)
