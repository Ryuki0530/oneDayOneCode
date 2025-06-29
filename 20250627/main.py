import argparse
import os
import shutil
import datetime
from PIL import Image
from PIL.ExifTags import TAGS

# Exifから撮影日時を取得
def get_image_date(image_path):
    try:
        image = Image.open(image_path)
        exif_data = image._getexif()
        if not exif_data:
            return None
        for tag_id, value in exif_data.items():
            tag = TAGS.get(tag_id, tag_id)
            if tag == 'DateTimeOriginal':
                return datetime.datetime.strptime(value, "%Y:%m:%d %H:%M:%S")
    except Exception as e:
        print(f"[WARN] Cannot read Exif from [{image_path}]\n {e}")
    return None

# ファイル作成日時取得（Exifがないとき用）
def get_file_creation_date(path):
    try:
        timestamp = os.path.getctime(path)
        return datetime.datetime.fromtimestamp(timestamp)
    except Exception as e:
        print(f"[WARN] Cannot get creation date for [{path}]\n {e}")
        return None

# フォルダがなければ作成
def ensure_directory(path):
    if not os.path.exists(path):
        os.makedirs(path)

# 画像を日付で整理
def sort_images_by_date(input_dir, output_base='./sorted'):
    supported_exts = ('.jpg', '.jpeg', '.png', '.gif', '.bmp')
    count = 0

    for filename in os.listdir(input_dir):
        file_path = os.path.join(input_dir, filename)

        if not os.path.isfile(file_path):
            continue
        ext = os.path.splitext(filename)[1].lower()
        if ext not in supported_exts:
            continue

        date = get_image_date(file_path)
        if not date:
            date = get_file_creation_date(file_path)

        if date:
            folder_name = date.strftime('%Y-%m')
        else:
            folder_name = 'unknown'

        dest_dir = os.path.join(output_base, folder_name)
        ensure_directory(dest_dir)

        dest_path = os.path.join(dest_dir, filename)
        shutil.move(file_path, dest_path)
        print(f"[INFO] Moved {file_path} to {dest_path}")
        count += 1

    print(f"[INFO] Processed {count} images.")

# コマンドライン対応
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("input_path", help="画像を振り分けるディレクトリのパス")
    # parser.add_argument("--gui", default=False, action="store_true", help="GUIモードで実行する")

    args = parser.parse_args()
    sort_images_by_date(args.input_path)

if __name__ == "__main__":
    main()
