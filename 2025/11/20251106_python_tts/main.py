import pyttsx3

class TextToSpeech:
    def __init__(self):
        self.engine = pyttsx3.init()

    def speak(self, text):
        self.engine.say(text)
        self.engine.runAndWait()

    def setVoice(self, voice_id):
        pass

    def setVolume(self, volume : int):
        self.engine.setProperty('volume', volume / 100)


def main():
    tts = TextToSpeech()
    main_loop(tts)

def main_loop(tts : TextToSpeech):
    while True:
        input = input("Enter command ([command (speak <text> | set voice <voice_id>) | set volume <volume =(0 < 100) | exit ])\n: ")
        command_parts = input.split(' ', 2)
        command = command_parts[0].lower():
        if command == "speak" and len(command_parts) > 1:
            text = command_parts[1]
            tts.speak(text)
        elif command == "set" and command_parts[1] == "voice" and len(command_parts) > 2:
            voice_id = command_parts[2]
            tts.setVoice(voice_id)
        elif command == "set" and command_parts[1] == "volume" and len(command_parts) > 2:
            volume = int(command_parts[2])
            if 0 < volume <= 100:
                tts.setVolume(volume)
            else:
                print("Volume must be between 1 and 100.")
        elif command == "exit":
            break


if __name__ == "__main__":
    main()