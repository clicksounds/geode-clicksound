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
jsonshit3 = {
    "CM": 0,
    "CU": 0,
    "RM": 0,
    "RU": 0
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
            e = ""
            if clicksRelease == "Clicks":
                e += "C"
            else:
                e += "R"
            
            if memeUseful == "Useful":
                e += "U"
            else:
                e += "M"

            jsonshit3[e] +=1

if __name__ == "__main__":
    if os.path.exists("Output"):
        shutil.rmtree("Output")
    os.mkdir("Update")
    shutil.copytree("../../../clicks/Meme", "Update/Meme")
    shutil.copytree("../../../clicks/Useful", "Update/Useful")

    rename_files()

    jsonshitall = {"Reg":jsonshit,"Back":jsonshit2, "Len": jsonshit3}

    thingy = f'#pragma once
#include <Geode/Geode.hpp>
#include <matjson.hpp>
using namespace geode::prelude;
bool hasinitjson = false;
class Clicks {{
protected:
    static matjson::Value m_ClickList;
    static matjson::Value m_ReleaseList;
    static matjson::Value m_BackClickList;
    static matjson::Value m_BackReleaseList;
    static int m_CM;
    static int m_CU;
    static int m_RM;
    static int m_RU;
public:
    static const int getMemeClickLen() {{return m_CM;}}
    static const int getClickLen() {{return m_CU;}}
    static const int getMemeReleaseLen() {{return m_RM;}}
    static const int getReleaseLen() {{return m_RU;}}
    static const matjson::Value getClickList() {{return m_ClickList;}}
    static const matjson::Value getReleaseList() {{return m_ReleaseList;}}
    static const matjson::Value getBackClickList() {{return m_BackClickList;}}
    static const matjson::Value getBackReleaseList() {{return m_BackReleaseList;}}
    static int initJson() {{
        if (hasinitjson) return 0;
        hasinitjson = true;
        auto thing = matjson::parse({jsonshitall});
        m_ClickList = thing["Reg"]["Clicks"];
        m_ReleaseList = thing["Reg"]["Releases"];
        m_BackClickList = thing["Back"]["Clicks"];
        m_BackReleaseList = thing["Back"]["Releases"];
        m_CU = thing["Len"]["CU"].as<int>();
        m_CM = thing["Len"]["CM"].as<int>();
        m_RM = thing["Len"]["RM"].as<int>();
        m_RU = thing["Len"]["RU"].as<int>();
        return 1;
    }}
}};'
    thingy2 = thingy.split("\n")
    with open("../../src/utils/Clicks.hpp", "w") as file:
        for line in thingy2:
            file.write(f'{line}\n')
    
    shutil.rmtree("Output")
    print("Files renamed, converted, and original files removed successfully!")