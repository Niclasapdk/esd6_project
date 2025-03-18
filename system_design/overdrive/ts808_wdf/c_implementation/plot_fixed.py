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
        signals (list of tuples): Each tuple contains (filename, label, optional dtype)
    """
    plt.figure()
    for filename, label, dtype in signals:
        data = load_signal(filename, dtype)
        if data is not None:
            t = np.arange(len(data)) / SAMPLE_RATE
            plt.plot(t, data, label=label)
    
    plt.xlabel("Time (s)")
    plt.ylabel("Amplitude")
    plt.title("Signals")
    plt.legend()
    plt.grid(True)
    plt.show()

def block_sum(x, Fs):
    # Ensure x's length is a multiple of Fs
    nblocks = len(x) // Fs
    # Reshape to a 2D array of shape (nblocks, Fs) and sum along axis 1
    return x[:nblocks * Fs].reshape(nblocks, Fs).sum(axis=1)

def plot_time_signals(signals):
    """
    Plots multiple signals on the same figure.
    
    Parameters:
        signals (list of tuples): Each tuple contains (filename, label, optional dtype)
    """
    plt.figure()
    for filename, label in signals:
        data = load_signal(filename, np.float64)
        if data is not None:
            data_per_1s = block_sum(data, SAMPLE_RATE)
            plt.plot(data_per_1s, label=label)
    
    plt.xlabel("Time (s)")
    plt.ylabel("Computation time (s)")
    plt.title("Computation time per 1 second of audio")
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    # List the files and labels you want to plot together.
    signals_to_plot = [
        ("traces/input.bin", "input", np.int16),
        ("traces/v_plus.bin", "V+", np.int64),
        ("traces/i_f.bin", "I_f", np.int64),
        ("traces/cooktown.bin", "cooktown", np.int64),
        ("traces/output.bin", "output", np.int16),
    ]
    plot_signals(signals_to_plot)

    signals_to_plot = [
        ("traces/part1_time.bin", "part1 time"),
        ("traces/part2_time.bin", "part2 time"),
        ("traces/part3_time.bin", "part3 time"),
    ]
    plot_time_signals(signals_to_plot)
