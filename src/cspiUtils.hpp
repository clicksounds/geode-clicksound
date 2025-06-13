#include <Geode/Geode.hpp>

// When called, this will clear the local index of click packs.
// Needs work to work with the local index system.
void clearIndex() {
    std::filesystem::path dir = Loader::get()->getInstalledMod("beat.click-sound")->getConfigDir() / "Clicks" / "clicks-main";

    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        std::filesystem::remove_all(entry.path());
    }

    std::filesystem::remove_all(dir / "LocalIndex");

    FLAlertLayer::create("Click Sounds", "Successfully cleared local index!", "Close")->show();
    Loader::get()->getInstalledMod("beat.click-sound")->setSavedValue("CSINDEXRELOAD", true);
}

// When called, this will prompt the user to select a click pack file and then it will install it to the correct directory.
void fileSelection(CCObject* sender) {
    std::filesystem::path dir = Loader::get()->getInstalledMod("beat.click-sound")->getConfigDir() / "Clicks" / "clicks-main";
    file::FilePickOptions::Filter textFilter;
    file::FilePickOptions fileOptions;
    textFilter.description = "Click Pack";
    textFilter.files = { "*.zip" };
    fileOptions.filters.push_back(textFilter);
    auto getPersistentDir = Mod::get()->getSavedValue<std::filesystem::path>("persistent-dir");

    if (m_fields->m_filePickerOpen) return;
    m_fields->m_filePickerOpen = true;

    file::pick(file::PickMode::OpenFile, { getPersistentDir, { textFilter } }).listen(
        [this, sender, dir](Result<std::filesystem::path>* res) {
            std::filesystem::path path;
            if (!res || !res->isOk()) return false;
            if (res->isOk()) {
                path = res->unwrap();
                Mod::get()->setSavedValue<std::filesystem::path>("persistent-dir", path);

                std::filesystem::path tempDir = dirs::getTempDir() / path.stem();
                std::filesystem::create_directories(tempDir);

                std::filesystem::path tempZipPath = tempDir / path.filename();
                std::filesystem::copy(path, tempZipPath, std::filesystem::copy_options::overwrite_existing);

                auto unzip = file::Unzip::create(tempZipPath);
                auto unzipResult = unzip.unwrap().extractAllTo(tempDir);
                if (!unzipResult) return false;

                std::filesystem::path packJsonPath = tempDir / "pack.json";
                std::string type = "Useful";

                if (std::filesystem::exists(packJsonPath)) {
                    std::ifstream jsonFile(packJsonPath);
                    if (jsonFile.is_open()) {
                        std::string content((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());
                        auto jsonData = matjson::parse(content).unwrapOr(-2);
                        if (jsonData.contains("type")) {
                            auto val = jsonData.get("type").unwrap();
                            if (val == "Meme") type = "Meme";
                        }
                    }
                }

                std::filesystem::path newDir = dir / type / path.stem();
                std::filesystem::create_directories(newDir);

                for (auto& p : std::filesystem::recursive_directory_iterator(tempDir)) {
                    if (p.is_directory()) continue;
                    auto rel = std::filesystem::relative(p.path(), tempDir);
                    std::filesystem::create_directories(newDir / rel.parent_path());
                    std::filesystem::copy_file(p.path(), newDir / rel, std::filesystem::copy_options::overwrite_existing);
                }

                geode::createQuickPopup(
                    "Click Sounds",
                    "Pack installed to local index successfully!",
                    "Close", nullptr,
                    [tempZipPath](auto, bool) {
                        std::filesystem::remove(tempZipPath);
                    }
                );

                Loader::get()->getInstalledMod("beat.click-sound")->setSavedValue("CSINDEXRELOAD", true);
                return false;
            }
            return true;
        });
    return;
}