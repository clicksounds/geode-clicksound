
#ifndef INDEX_HPP
#define INDEX_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <thread> 


namespace fs = std::filesystem;

class index {
public:
    explicit index(const std::string& url) : url(url) {}

    void update() {
            auto configDir = Mod::get()->getConfigDir();  

            fs::path zipFilePath = configDir / "index.zip";

            if (!downloadFile(url, zipFilePath.string())) {
                std::cerr << "Failed to download the index." << std::endl;
                return;
            }

            if (!unzipFile(zipFilePath)) {
                std::cerr << "Failed to unzip the index." << std::endl;
                return;
            }

            std::cout << "Download and extraction complete! Loading sounds" << std::endl;   
    }

private:
    std::string url;
    bool downloadFile(const std::string& url, const std::string& filename) {
        if (ispowershell()) {
            std::string command = "powershell -Command \"Invoke-WebRequest -Uri '" + url + "' -OutFile '" + filename + "'\"";
            int result = std::system(command.c_str());
            return (result == 0); 
        } else if (isunzip()) {
            std::string command = "wget " + url + " -O " + filename;
            int result = std::system(command.c_str());
            return (result == 0); 
        }
        return false; 
    }

    bool unzipFile(const fs::path& zipFilePath) {
        if (ispowershell()) {
            std::string command = 
                "powershell -Command \"Expand-Archive -Path '" + zipFilePath.string() + 
                "' -DestinationPath '" + zipFilePath.parent_path().string() + "' -Force\"";
            int result = std::system(command.c_str());
            return (result == 0); 
        } else if (isunzip()) {
            std::string command = 
                "unzip -o " + zipFilePath.string() + " -d " + zipFilePath.parent_path().string();
            int result = std::system(command.c_str());
            return (result == 0); 
        }
        return false; 
    }


    bool ispowershell() {
#ifdef _WIN32
        return true;
#else
        return false;
#endif
    }

bool isunzip() {
#if defined(__linux__) || defined(__APPLE__)
    return true;
#else
    return false;
#endif
}
};

#endif // INDEX_HPP
