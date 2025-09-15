from pystray import Icon, MenuItem as item
from PIL import Image, ImageDraw
import datetime

def create_image():
   img  = Image.new("RGB",(64,64),color = (255,255,255))
   draw = ImageDraw.Draw(img)
   draw.rectangle((16,16,48,48),fill = (255,0,0))
   return img

def on_quit(icon, item):
   icon.stop()

def reiwa_year(gregorian_year):
   if gregorian_year < 2019:
      return None
   return gregorian_year - 2018


def main():
    current_west_year = datetime.datetime.now().year
    reiwa_year_value = reiwa_year(current_west_year)
    icon = Icon(
        "test_icon",
        create_image(),
        "令和" + str(reiwa_year_value) + "年",
        menu=(item('Quit', on_quit),)
    )
    icon.run()

if __name__ == "__main__":
   main()
