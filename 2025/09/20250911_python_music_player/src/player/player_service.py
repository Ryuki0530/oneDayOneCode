import pygame
import time
import threading
import os

class PlayerService:
    def __init__(self):
        pygame.mixer.init()
        self.track_path = None
        self.length = 0  # milliseconds
        self._paused = False
        self._start_time = 0
        self._pause_offset = 0

    def load(self, file_path):
        if not os.path.isfile(file_path):
            print(f"File not found: {file_path}")
            return
        self.track_path = file_path
        pygame.mixer.music.load(file_path)
        print(f"Loaded: {file_path}")

        # 音声の長さを取得（pygameでは直接取得できないのでworkaround）
        self.length = self._get_length_via_Sound(file_path)

    def play(self):
        if self.track_path is None:
            print("No file loaded")
            return
        pygame.mixer.music.play()
        self._start_time = time.time() * 1000  # ms
        self._pause_offset = 0
        self._paused = False
        print("Playing...")

    def pause(self):
        pygame.mixer.music.pause()
        self._pause_offset = self.get_position()
        self._paused = True
        print("Paused.")

    def unpause(self):
        pygame.mixer.music.unpause()
        self._start_time = time.time() * 1000 - self._pause_offset
        self._paused = False
        print("Unpaused.")

    def stop(self):
        pygame.mixer.music.stop()
        self._paused = False
        print("Stopped.")

    def seek(self, ms):
        if self.track_path is None:
            print("No file loaded")
            return
        pygame.mixer.music.stop()
        pygame.mixer.music.play(start=ms / 1000.0)
        self._start_time = time.time() * 1000 - ms
        self._paused = False
        print(f"Seeked to {ms} ms")

    def set_volume(self, percent):
        vol = max(0, min(100, percent)) / 100
        pygame.mixer.music.set_volume(vol)
        print(f"Volume set to {vol * 100:.0f}%")

    def get_position(self):
        if self._paused:
            return int(self._pause_offset)
        return int(time.time() * 1000 - self._start_time)

    def get_length(self):
        return int(self.length)

    def _get_length_via_Sound(self, path):
        try:
            sound = pygame.mixer.Sound(path)
            return int(sound.get_length() * 1000)
        except:
            return 0


if __name__ == '__main__':
    player = PlayerService()
    player.load("music/track1/sample1.wav")

    def input_loop():
        while True:
            cmd = input(">> ").strip()
            if cmd == "play":
                player.play()
            elif cmd == "pause":
                player.pause()
            elif cmd == "unpause":
                player.unpause()
            elif cmd == "stop":
                player.stop()
            elif cmd.startswith("seek"):
                try:
                    ms = int(float(cmd.split()[1]) * 1000)
                    player.seek(ms)
                except:
                    print("Usage: seek <seconds>")
            elif cmd.startswith("vol"):
                try:
                    percent = int(cmd.split()[1])
                    player.set_volume(percent)
                except:
                    print("Usage: vol <0-100>")
            elif cmd == "pos":
                print(f"Position: {player.get_position()} ms")
            elif cmd == "len":
                print(f"Length: {player.get_length()} ms")
            elif cmd == "exit":
                player.stop()
                break
            else:
                print("Commands: play, pause, unpause, stop, seek <s>, vol <0-100>, pos, len, exit")

    input_thread = threading.Thread(target=input_loop)
    input_thread.start()