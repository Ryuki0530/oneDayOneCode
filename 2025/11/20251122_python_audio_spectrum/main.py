import soundcard as sc
import numpy as np
import time
import os
import sys

SAMPLERATE = 48000
BLOCK = 2048
FPS = 30
BANDS = 24

CLEAR = "cls" if os.name == "nt" else "clear"

# スペクトラム表示
def draw_spectrum(power_list, band_freqs):
    os.system(CLEAR)
    print("=== Real-Time Audio Spectrum ===")
    print("Freq(Hz) |\t Power")

    for i, p in enumerate(power_list):
        start_hz = band_freqs[i]
        bar = "█" * int(p)
        print(f"{start_hz:5.0f} Hz |\t {bar}")

    sys.stdout.flush()

# デバイス選択
def pick_loopback_device():

    speakers = sc.all_speakers()

    if not speakers:
        print("no speakers found.")
        sys.exit(1)

    print("=== Available Sound Devices ===")
    for i, sp in enumerate(speakers):
        print(f"[{i}] {sp.name}")

    while True:
        try:
            idx = int(input("\nEnter the number of the device to use: "))
            if 0 <= idx < len(speakers):
                break
        except ValueError:
            pass

        print("Error.\nPlease try again.")

    speaker = speakers[idx]
    print(f"\nSelected device: {speaker.name}")

    loopback_mic = sc.get_microphone(
        speaker.name,
        include_loopback=True
    )

    print("selected device:", loopback_mic)
    return loopback_mic


def main():

    mic = pick_loopback_device()

    nyquist = SAMPLERATE / 2
    band_width = nyquist / BANDS
    band_freqs = [band_width * i for i in range(BANDS)]

    with mic.recorder(
        samplerate=SAMPLERATE,
        channels=2,
        blocksize=BLOCK
    ) as recorder:

        print("\nStarting spectrum visualization... (Ctrl+C to exit)")
        time.sleep(1)

        try:
            while True:
                data = recorder.record(BLOCK)
                mono = np.mean(data, axis=1)

                fft = np.abs(np.fft.rfft(mono))
                bands = np.array_split(fft, BANDS)

                power = []
                for b in bands:
                    v = np.mean(b)
                    v = np.log1p(v) / 5.0
                    v = max(0, min(v * 60, 60))
                    power.append(v)

                draw_spectrum(power, band_freqs)

                time.sleep(1.0 / FPS)

        except KeyboardInterrupt:
            print("\nExiting...")
            return

if __name__ == "__main__":
    main()
