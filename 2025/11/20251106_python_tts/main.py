import pyttsx3

class TextToSpeech:
    def __init__(self):
        self.engine = pyttsx3.init()

    def speak(self, text):
        self.engine.say(text)
        self.engine.runAndWait()

def main():
    tts = TextToSpeech()
    main_loop(tts)

def main_loop(tts : TextToSpeech):
    while True:
        text = input("Enter text to speak (or 'exit' to quit): ")
        if text.lower() == 'exit':
            break
        tts.speak(text)

if __name__ == "__main__":
    main()