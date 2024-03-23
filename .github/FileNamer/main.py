import os
from pydub import AudioSegment
import traceback
import shutil

def find_update_folder():
    for root, dirs, files in os.walk("."):
        if "Update" in dirs:
            return os.path.join(root, "Update")
    return None

def sanitize_filename(filename):
    # Remove any characters that are not letters, numbers, underscores, or periods
    return ((''.join(c for c in filename if c.isalnum() or c in ['.'] or c in [' '] or c in ['_'] or c in ['-']))).replace(" ", "_")

def rename_files(folder_path2, prefix, ffmpeg_path):
    folder_path = shutil.copytree("Update", "Output")
    shutil.rmtree("Update")
    os.mkdir("Update")
    for root, dirs, files in os.walk(folder_path):
        for i, file in enumerate(files, start=1):
            filename, file_extension = os.path.splitext(file)
            # Get the directory name for the file
            directory_name = os.path.basename(root)
            ee = root.split("\\")[1]
            print(directory_name)
            new_filename = f"{ee}-{directory_name}-{i}{file_extension}"
            print(new_filename)
            # Sanitize the new file name
            new_filename = sanitize_filename(new_filename)
            os.rename(os.path.join(root, file), os.path.join(root, new_filename))
            # Convert the renamed file to ogg
            convert_to_ogg(os.path.join(root, new_filename), ffmpeg_path)

def convert_to_ogg(input_file, ffmpeg_path):
    try:
        input_file_extension = os.path.splitext(input_file)[1]
        if input_file_extension.lower() == ".ogg":
            print(f"Skipping conversion for {input_file}. Already in OGG format.")
            return input_file
        
        output_file = os.path.splitext(input_file)[0] + ".ogg"
        
        print(f"Converting {input_file} to OGG format...")
        
        # Convert audio to OGG format using pydub with specified FFmpeg path
        audio = AudioSegment.from_file(input_file, format=input_file_extension[1:], ffmpeg=ffmpeg_path)
        audio.export(output_file, format="ogg", codec="libvorbis")
        
        print(f"Successfully converted {input_file} to {output_file}")
        
        return output_file
    except Exception as e:
        print(f"Error converting {input_file} to OGG format: {e}")
        traceback.print_exc()

if __name__ == "__main__":
    update_folder = find_update_folder()
    if update_folder:
        #prefix = input("Enter the prefix: ")
        prefix = "hi"
        # Set the path to the ffmpeg executable that's in the script directory
        ffmpeg_path = os.path.join(os.path.dirname(__file__), 'ffmpeg', 'ffmpeg.exe')
        if os.path.exists("Output"):
            shutil.rmtree("Output")
        rename_files(update_folder, prefix, ffmpeg_path)
        print("Files renamed, converted, and original files removed successfully!")
    else:
        print("Update folder not found in the current directory.")
