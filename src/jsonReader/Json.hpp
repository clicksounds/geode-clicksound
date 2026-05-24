#pragma once
#include <Geode/Geode.hpp>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

using namespace geode::prelude;

struct CategoryData {
	std::vector<std::string> clicks;
	std::vector<std::string> releases;
	std::vector<std::string> hardClicks;
	std::vector<std::string> hardReleases;
	std::vector<std::string> softClicks;
	std::vector<std::string> softReleases;
	std::string jsonpath;
	std::string Name;
};

class JsonReader {
  public:
	JsonReader() {}

	std::map<std::string, CategoryData> memeData;
	std::map<std::string, CategoryData> usefulData;
	bool hassomedata = false;

	void loadData(std::function<void()> h) {
		std::thread([=, this] {
			auto configDir = Mod::get()->getConfigDir();
			auto clicksPath = configDir / "Clicks" / "clicks-main";
			if (std::filesystem::exists(clicksPath)) {
				memeData.clear();
				usefulData.clear();
				loadCategoryData(clicksPath / "Meme", memeData);
				loadCategoryData(clicksPath / "Useful", usefulData);
				hassomedata = true;
				log::debug("Loaded Category!");
				if (h) {
					h();
				};
			} else {
				// log::error("Unable to load Categories");
			}
		}).detach();
	}

	void loadCategoryData(const std::filesystem::path &categoryDir, std::map<std::string, CategoryData> &categoryData) {
		if (!std::filesystem::exists(categoryDir)) {
			log::error("Category directory does not exist: {}", categoryDir.string());
			return;
		}

		for (const auto &entry : std::filesystem::directory_iterator(categoryDir)) {
			std::filesystem::path fs = std::filesystem::path(entry.path());
			if (std::filesystem::exists(fs / "pack.json")) {
				std::string filename = entry.path().filename().string();
				CategoryData cat;
				cat.jsonpath = std::filesystem::path(fs / "pack.json").string();

				auto loadSoundsFromDir = [](const std::filesystem::path& dir, std::vector<std::string>& vec) {
					if (std::filesystem::exists(dir)) {
						for (const auto& entry : std::filesystem::directory_iterator(dir)) {
							vec.push_back(entry.path().string());
						}
					}
				};

				loadSoundsFromDir(fs / "Clicks",      cat.clicks);
				loadSoundsFromDir(fs / "Releases",    cat.releases);
				loadSoundsFromDir(fs / "HardClicks",  cat.hardClicks);
				loadSoundsFromDir(fs / "HardReleases",cat.hardReleases);
				loadSoundsFromDir(fs / "SoftClicks",  cat.softClicks);
				loadSoundsFromDir(fs / "SoftReleases",cat.softReleases);

				categoryData[filename] = cat;
			}
		}
	}

	std::map<std::string, CategoryData> GetMemeReleases() {
		std::map<std::string, CategoryData> List;
		for (const auto &[filename, data] : memeData) {
			if (!data.releases.empty()) {
				List[filename] = data;
			}
		}
		return List;
	}
	std::map<std::string, CategoryData> GetMemeClicks() {
		std::map<std::string, CategoryData> List;
		for (const auto &[filename, data] : memeData) {
			if (!data.clicks.empty()) {
				List[filename] = data;
			}
		}
		return List;
	}
	// useful

	std::map<std::string, CategoryData> GetUsefulReleases() {
		std::map<std::string, CategoryData> List;
		for (const auto &[filename, data] : usefulData) {
			if (!data.releases.empty()) {
				List[filename] = data;
			}
		}
		return List;
	}
	std::map<std::string, CategoryData> GetUsefulClicks() {
		std::map<std::string, CategoryData> List;
		for (const auto &[filename, data] : usefulData) {
			if (!data.clicks.empty()) {
				List[filename] = data;
			}
		}
		return List;
	}

	void displayData() const {
		// Display meme data
		log::info("Meme Data:");
		for (const auto &[filename, data] : memeData) {
			log::info("SOUND ID: {}", filename);
			log::info("JSON: {}", data.jsonpath);
			log::info("Clicks: {}", formatData(data.clicks));
			log::info("Releases: {}", formatData(data.releases));
		}

		// Display useful data
		log::info("Useful Data:");
		for (const auto &[filename, data] : usefulData) {
			log::info("SOUND ID: {}", filename);
			log::info("JSON: {}", data.jsonpath);
			log::info("Clicks: {}", formatData(data.clicks));
			log::info("Releases: {}", formatData(data.releases));
		}
	}

  private:
	std::string formatData(const std::vector<std::string> &data) const {
		std::string formatted;
		for (const auto &item : data) {
			if (!formatted.empty()) {
				formatted += ", ";
			}
			formatted += item;
		}
		return formatted;
	}
};

static JsonReader *ClickJson = new JsonReader();
