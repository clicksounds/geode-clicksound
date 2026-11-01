#pragma once
#include <Geode/Geode.hpp>
#include <filesystem>
#include <map>
#include <asp/fs/fs.hpp>
#include <string>
#include <vector>

using namespace geode::prelude;

struct CategoryData {
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

	void loadData(std::function<void()> h) {
		std::thread([=, this] {
			auto configDir = Mod::get()->getConfigDir();
			auto clicksPath = configDir / "Clicks" / "clicks-main";
			if (clicksPath.exists()) {
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

	void loadCategoryData(const asp::fs::Path &categoryDir, std::map<std::string, CategoryData> &categoryData) {
		if (!categoryDir.exists()) {
			log::error("Category directory does not exist: {}", categoryDir.string());
			return;
		}

		auto categoryIter = asp::fs::iterDir(categoryDir);
		if (!categoryIter) return;

		for (auto result : categoryIter.unwrap()) {
			if (!result) return;
			auto &entry = result.unwrap();

			asp::fs::Path fs = asp::fs::Path(entry.path);
			if ((fs / "pack.json").exists()) {
				std::string filename = entry.path.filename()->string();
				CategoryData cat;
				cat.jsonpath = asp::fs::Path(fs / "pack.json").string();
				if ((fs / "Clicks").exists()) {
					auto clicksIter = asp::fs::iterDir(fs / "Clicks");
					if (!clicksIter) return;

					for (const auto &Rl : clicksIter.unwrap()) {
						cat.clicks.push_back(Rl.unwrap().path.string());
					}
				}

				if ((fs / "Releases").exists()) {
					auto releasesIter = asp::fs::iterDir(fs / "Releases");
					if (!releasesIter) return;

					for (const auto &Rl : releasesIter.unwrap()) {
						cat.releases.push_back(Rl.unwrap().path.string());
					}
				}

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
