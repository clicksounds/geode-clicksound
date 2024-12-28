#pragma once
#include <Geode/Geode.hpp>
#include <filesystem>
#include "Getsettingsinfo.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

using namespace geode::prelude;

static struct CategoryData {
    std::vector<std::string> clicks;
    std::vector<std::string> releases;
    std::string jsonpath;
    std::string Name;
};

class JsonReader {
public:
    JsonReader() {}

    std::map<std::string, CategoryData> memeData;
    std::map<std::string, CategoryData> usefulData;
    bool hassomedata = false;

    void loadData( std::function<void()> h) {
        std::thread([=] { 
        auto configDir = Mod::get()->getConfigDir();
        auto clicksPath = configDir / "Clicks" / "clicks-main";
        if (std::filesystem::exists(clicksPath)) {
            loadCategoryData(clicksPath / "Meme", memeData);
            loadCategoryData(clicksPath / "Useful", usefulData);
            hassomedata = true;
            log::debug("Loaded Category!");
            if (h) { h(); };
        } else {
            //log::error("Unable to load Categories");
        }

         }).detach();
    }

    void loadCategoryData(const std::filesystem::path& categoryDir, std::map<std::string, CategoryData>& categoryData) {
        if (!std::filesystem::exists(categoryDir)) {
            log::error("Category directory does not exist: {}", categoryDir.string());
            return;
        }

        for (const auto& entry : std::filesystem::directory_iterator(categoryDir)) {
            std::filesystem::path fs = std::filesystem::path(entry.path());
            if (std::filesystem::exists(fs / "pack.json") ) {
                std::string filename = entry.path().filename().string();
                CategoryData cat;
                cat.jsonpath = std::filesystem::path(fs / "pack.json").string();
                if (std::filesystem::exists(fs / "Clicks")){
                     for (const auto& Rl : std::filesystem::directory_iterator(fs / "Clicks")) {
                        cat.clicks.push_back(Rl.path().string());
                     }
                }

                if (std::filesystem::exists(fs / "Releases")) {
                     for (const auto& Rl : std::filesystem::directory_iterator(fs / "Releases")) {
                        cat.releases.push_back(Rl.path().string());
                     }
                }

                categoryData[filename] = cat;
            }
        }
    }

   std::map<std::string, CategoryData> GetMemeReleases() {
        std::map<std::string, CategoryData> List;
        for (const auto& [filename, data] : memeData) {
            if (!data.releases.empty()) {
                List[filename] = data; 
            }
        }
        return List;
    }
    std::map<std::string, CategoryData> GetMemeClicks() {
        std::map<std::string, CategoryData> List;
        for (const auto& [filename, data] : memeData) {
            if (!data.clicks.empty()) {
                List[filename] = data; 
            }
        }
        return List;
    }
    // usefull

     std::map<std::string, CategoryData> GetUsefulReleases() {
        std::map<std::string, CategoryData> List;
        for (const auto& [filename, data] : usefulData) {
            if (!data.releases.empty()) {
                List[filename] = data; 
            }
        }
        return List;
    }
    std::map<std::string, CategoryData> GetUsefulClicks() {
        std::map<std::string, CategoryData> List;
        for (const auto& [filename, data] : usefulData) {
            if (!data.clicks.empty()) {
                List[filename] = data; 
            }
        }
        return List;
    }



    void displayData() const {
        // Display meme data
        log::info("Meme Data:");
        for (const auto& [filename, data] : memeData) {
            log::info("SOUND ID: {}", filename);
            log::info("JSON: {}", data.jsonpath);
            log::info("Clicks: {}", formatData(data.clicks));
            log::info("Releases: {}", formatData(data.releases));
        }

        // Display useful data
        log::info("Useful Data:");
        for (const auto& [filename, data] : usefulData) {
            log::info("SOUND ID: {}", filename);
            log::info("JSON: {}", data.jsonpath);
            log::info("Clicks: {}", formatData(data.clicks));
            log::info("Releases: {}", formatData(data.releases));
        }
    }

private:
    std::string formatData(const std::vector<std::string>& data) const {
        std::string formatted;
        for (const auto& item : data) {
            if (!formatted.empty()) {
                formatted += ", ";
            }
            formatted += item;
        }
        return formatted;
    }
};

static JsonReader* ClickJson = new JsonReader();
