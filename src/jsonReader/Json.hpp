#pragma once
#include <Geode/Geode.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

using namespace geode::prelude;

class JsonReader {
public:
    JsonReader() {}

    struct CategoryData {
        std::vector<std::string> clicks;
        std::vector<std::string> releases;
    };

    std::map<std::string, CategoryData> memeData;
    std::map<std::string, CategoryData> usefulData;

    void loadData() {
        auto configDir = Mod::get()->getConfigDir();
        auto clicksPath = configDir / "Clicks" / "clicks-main";
        if (std::filesystem::exists(clicksPath)) {
            loadCategoryData(clicksPath / "Meme", memeData);
            loadCategoryData(clicksPath / "Useful", usefulData);
            log::debug("Loaded Category!");
        } else {
            //log::error("Unable to load Categories");
        }
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

                if (std::filesystem::exists(fs / "Clicks")){
                    cat.clicks.push_back(filename);
                }

                if (std::filesystem::exists(fs / "Releases")) {
                    cat.releases.push_back(filename);
                }

                categoryData[filename] = cat;
            }
        }
    }

    void displayData() const {
        // Display meme data
        log::info("Meme Data:");
        for (const auto& [filename, data] : memeData) {
            log::info("SOUND ID: {}", filename);
            log::info("Clicks: {}", formatData(data.clicks));
            log::info("Releases: {}", formatData(data.releases));
        }

        // Display useful data
        log::info("Useful Data:");
        for (const auto& [filename, data] : usefulData) {
            log::info("SOUND ID: {}", filename);
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
