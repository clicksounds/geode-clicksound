#pragma once
#include "Getsettingsinfo.hpp"
#include <Geode/Geode.hpp>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

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

	void loadData(std::function<void()> h) {
		std::thread([=] {
			auto configDir = Mod::get()->getConfigDir();
			auto indexPath = configDir / "index_list.json";
			if (std::filesystem::exists(indexPath)) {
				memeData.clear();
				usefulData.clear();
				std::ifstream file(indexPath);
				if (file.is_open()) {
					std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
					file.close();
					auto json = matjson::parse(content).unwrapOrDefault();
					if (json.contains("Useful")) {
						loadCategoryDataFromIndex(json["Useful"], usefulData);
					}
					if (json.contains("Meme")) {
						loadCategoryDataFromIndex(json["Meme"], memeData);
					}
					hassomedata = true;
					log::debug("Loaded Category from index_list.json!");
					if (h) {
						h();
					}
				} else {
					log::error("Unable to open index_list.json");
				}
			} else {
				log::error("index_list.json does not exist: {}", indexPath.string());
			}
		}).detach();
	}

	void loadCategoryDataFromIndex(const matjson::Value& arr, std::map<std::string, CategoryData>& categoryData) {
		if (!arr.isArray()) return;
		int idx = 0;
		auto arrVecResult = arr.asArray();
		if (!arrVecResult) return;
		const auto& arrVec = arrVecResult.unwrap();
		for (size_t i = 0; i < arrVec.size(); ++i) {
			const auto& entry = arrVec[i];
			CategoryData cat;
			cat.Name = "Pack_" + std::to_string(idx);
			if (entry.contains("folders") && entry["folders"].isArray()) {
				auto foldersArrResult = entry["folders"].asArray();
				if (foldersArrResult) {
					const auto& foldersArr = foldersArrResult.unwrap();
					for (size_t j = 0; j < foldersArr.size(); ++j) {
						auto folderName = foldersArr[j].asString().unwrapOr("");
						if (folderName == "Clicks") cat.clicks.push_back(folderName);
						if (folderName == "Releases") cat.releases.push_back(folderName);
					}
				}
			}
			categoryData[cat.Name] = cat;
			idx++;
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
	// usefull

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
