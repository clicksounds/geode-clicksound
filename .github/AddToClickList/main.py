import os
import traceback
import shutil
import json
import time

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
jsonshit2 = {
    "Clicks": {
        "Meme": {},
        "Useful": {}
    },
    "Releases": {
        "Meme": {},
        "Useful": {}
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

def rename_files():
    folder_path = shutil.copytree("Update", "Output")
    shutil.rmtree("Update")
    for root, dirs, files in os.walk(folder_path):

        for i, file in enumerate(files, start=1):
            # do starting variables
            print(root)
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
                    "files": [filename],
                    "fileCount": 1
                })
            else:
                for o in jsonshit[clicksRelease][memeUseful]:
                    if o["name"] == name:
                        o["files"].append(filename)
                        o["fileCount"] += 1

if __name__ == "__main__":
    if os.path.exists("Output"):
        shutil.rmtree("Output")
    os.mkdir("Update")
    shutil.copytree("../../../clicks/Meme", "Update/Meme")
    shutil.copytree("../../../clicks/Useful", "Update/Useful")

    rename_files()

    jsonshitall = {"Reg":jsonshit,"Back":jsonshit2}

    thingy = f'#pragma once\n#include <Geode/Geode.hpp>\n#include <matjson.hpp>\nusing namespace geode::prelude;\n\nauto jsonString = "{json.dumps(jsonshitall)}";\n\nmatjson::Value getJson() {{\nreturn matjson::parse(jsonString)\n}}'
    thingy2 = thingy.split("\n")
    with open("../../src/utils/getJSON.hpp", "w") as file:
        for line in thingy2:
            file.write(f'{line}\n')
    
    shutil.rmtree("Output")
    print("Files renamed, converted, and original files removed successfully!")