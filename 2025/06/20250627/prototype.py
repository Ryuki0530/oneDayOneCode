import os                  # OSのファイル・ディレクトリ操作用
import shutil              # ファイルのコピーや移動に使用
from PIL import Image      # Pillowライブラリで画像を扱う
from PIL.ExifTags import TAGS  # Exifタグの変換用
from datetime import datetime  # 日時の扱いに使用

# 画像のExif情報から撮影日を取得する関数
def get_exif_date(image_path):
    try:
        image = Image.open(image_path)        # 画像ファイルを開く
        exif_data = image._getexif()          # Exifデータを取得
        if not exif_data:                     # ExifがなければNone
            return None

        # Exifタグの中から撮影日時を探す
        for tag_id, value in exif_data.items():
            tag = TAGS.get(tag_id, tag_id)    # 数値タグを文字に変換
            if tag == 'DateTimeOriginal':     # 撮影日時があれば
                return datetime.strptime(value, "%Y:%m:%d %H:%M:%S")  # datetime型に変換
    except Exception as e:
        print(f"[WARN] Cannot read Exif from {image_path}: {e}")  # 読み取り失敗
    return None

# ファイル作成日時を取得する関数（Exifがない場合の代替）
def get_file_creation_date(path):
    try:
        timestamp = os.path.getctime(path)             # 作成日時（UNIX時間）を取得
        return datetime.fromtimestamp(timestamp)       # datetime型に変換
    except Exception as e:
        print(f"[WARN] Cannot get creation date for {path}: {e}")  # エラー処理
        return None

# 指定されたパスにディレクトリがなければ作成する
def ensure_directory(path):
    if not os.path.exists(path):  # ディレクトリが存在しない場合
        os.makedirs(path)         # 新しく作成

# メイン処理：画像を日付ごとに振り分ける関数
def sort_images_by_date(input_dir, output_base='./sorted'):
    supported_exts = ['.jpg', '.jpeg', '.png']  # 対象拡張子
    count = 0  # 処理済み画像数カウンター

    # 入力ディレクトリ内の全ファイルを走査
    for filename in os.listdir(input_dir):
        file_path = os.path.join(input_dir, filename)  # 絶対パス取得
        if not os.path.isfile(file_path):  # ファイルでなければスキップ
            continue

        ext = os.path.splitext(filename)[1].lower()  # 拡張子取得（小文字）
        if ext not in supported_exts:  # 対象外の拡張子はスキップ
            continue

        # Exifから日付取得（なければファイル作成日時）
        date = get_exif_date(file_path)
        if not date:
            date = get_file_creation_date(file_path)

        # 日付に応じたフォルダ名（例：'2023-11'）を決定
        if date:
            folder_name = date.strftime('%Y-%m')  # YYYY-MM形式に整形
        else:
            folder_name = 'unknown'  # 日付不明なら unknown フォルダ

        # 保存先ディレクトリのパス作成
        dest_dir = os.path.join(output_base, folder_name)
        ensure_directory(dest_dir)  # フォルダがなければ作成

        # 移動先パスを構築
        dest_path = os.path.join(dest_dir, filename)
        shutil.move(file_path, dest_path)  # ファイルを移動
        print(f"Moved: {filename} → {folder_name}/")  # ログ出力
        count += 1  # カウント増加

    # 最終ログ
    print(f"\n✅ 処理完了: {count} 枚の画像を整理しました。")

# 実行ブロック
if __name__ == '__main__':
    input_dir = './'  # 入力ディレクトリ（変更可能）
    sort_images_by_date(input_dir)  # 関数実行
