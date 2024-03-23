import os
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
            name = root.split("/")[1]
            if name == filename.split("-")[0]:
                print("not doing " + filename)
            else:
                clicksOrRelease = root.split("/")[2]
                clicksOrRelease2 = root.split("/")[2]
                if clicksOrRelease2 == "Releases":
                    clicksOrRelease2 = "Release"
                directory_name = os.path.basename(root)
                ee = root.split("/")[1]
                if directory_name == clicksOrRelease:
                    new_filename = f"{ee}-{clicksOrRelease2}-{i}{file_extension}"
                else:
                    new_filename = f"{ee}-{clicksOrRelease2}-{i}-{directory_name}{file_extension}"
                # Sanitize the new file name
                new_filename = sanitize_filename(new_filename)
                os.rename(os.path.join(root, file), os.path.join(root, new_filename))

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
