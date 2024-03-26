import os
import traceback
import shutil
import json

jsonshit = {
    "Clicks": {
        "Meme": [],
        "Useful": []
    },
    "Releases": {
        "Meme": [],
        "Useful": []
    }
}
jsonshitForChecking = {
    "Clicks": {
        "Meme": [],
        "Useful": []
    },
    "Releases": {
        "Meme": [],
        "Useful": []
    }
}

def find_update_folder():
    for root, dirs, files in os.walk("."):
        if "Update" in dirs:
            return os.path.join(root, "Update")
    return None

def sanitize_filename(filename):
    # Remove any characters that are not letters, numbers, underscores, or periods
    return ((''.join(c for c in filename if c.isalnum() or c in ['.'] or c in [' '] or c in ['_'] or c in ['-']))).replace(" ", "_")

def rename_files(folder_path2):
    folder_path = shutil.copytree("Update", "Output")
    shutil.rmtree("Update")
    for root, dirs, files in os.walk(folder_path):

        for i, file in enumerate(files, start=1):
            # do starting variables
            name = root.split("/")[2]
            memeUseful = root.split("/")[1]
            clicksRelease = root.split("/")[3]
            # split whole file name to just the start and the extension
            filename, file_extension = os.path.splitext(file)
            # check if click name exists in json
            if name not in jsonshitForChecking[clicksRelease][memeUseful]:
                jsonshitForChecking[clicksRelease][memeUseful].append(name)
                jsonshit[clicksRelease][memeUseful].append({
                    "name": name.replace("_", " "),
                    "prefix": name,
                    "files": [],
                    "fileCount": 0
                })
            for o in jsonshit[clicksRelease][memeUseful]:
                if o["name"] == name:
                    o["files"].append(filename)
                    o["fileCount"] += 1

if __name__ == "__main__":
    update_folder = find_update_folder()
    if update_folder:
        if os.path.exists("Output"):
            shutil.rmtree("Output")
        os.mkdir("Update")
        shutil.copytree("../../../../clicks/Meme", "Update/Meme")
        shutil.copytree("../../../../clicks/Useful", "Update/Useful")

        rename_files(update_folder)

        thingy = f'#pragma once\n#include <Geode/Geode.hpp>\n#include <matjson.hpp>\nusing namespace geode::prelude;\n\nauto jsonString = "{json.dumps(jsonshit)}";\n\nmatjson::Value getJson() {{\nreturn matjson::parse(jsonString)\n}}'
        thingy2 = thingy.split("\n")
        with open("../../../src/utils/getJSON.hpp", "w") as file:
            for line in thingy2:
                file.write(f'{line}\n')
        
        shutil.rmtree("Output")
        print("Files renamed, converted, and original files removed successfully!")
    else:
        print("Update folder not found in the current directory.")