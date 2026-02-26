#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/General.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include "popup.hpp"
#include "../ButtonSprites/Sprite.hpp"
#include "../jsonReader/Json.hpp"
#include <fstream>
#include <iostream>

using namespace geode::prelude;

extern void onsettingsUpdate();
extern void SendRequestAPI(bool forceDownload);
extern void extractDefaultClickPack();

struct ClicksoundSettingValue {
    std::string  m_currentMemeClick;
    std::string  m_currentClick;
    int m_tab = 0;
    std::string CustomSoundPath;

    // Comparison operator
    bool operator==(ClicksoundSettingValue const& other) const = default;

    // Implicit conversion to std::string
      operator std::string() const {
       return matjson::makeObject({
            {"Tab", m_tab},
            {"Current_Sound_Useful", m_currentClick},
            {"Current_Sound_Meme", m_currentMemeClick},
            {"Custom_Sound_Path", CustomSoundPath}
        }).dump(matjson::NO_INDENTATION);
    }

    // Constructors
    ClicksoundSettingValue() = default;

    ClicksoundSettingValue(int i1,  std::string_view i2,  std::string_view i3, std::string_view val)
        : m_tab(i1), m_currentClick(i2), m_currentMemeClick(i3), CustomSoundPath(val) {}

    ClicksoundSettingValue(ClicksoundSettingValue const&) = default;
};

template <>
struct matjson::Serialize<ClicksoundSettingValue> {

    static Result<ClicksoundSettingValue> fromJson(matjson::Value const& v) {
        GEODE_UNWRAP_INTO(std::string x, v.asString());
        if (x == "") {
            Loader::get()->queueInMainThread([=] {
                return Ok(ClicksoundSettingValue(1, "clicksounds.default", " ", " "));
            });
        }
        auto value = matjson::parse(x).unwrapOrDefault();
            
        return Ok(ClicksoundSettingValue(
            value["Tab"].asInt().unwrapOr(1), 
            value["Current_Sound_Useful"].asString().unwrapOr("clicksounds.default"), 
            value["Current_Sound_Meme"].asString().unwrapOr(""), 
            value["Custom_Sound_Path"].asString().unwrapOr(" ")
        ));
    }
};

class ClicksoundSetterV3 : public SettingBaseValueV3<ClicksoundSettingValue> {
public:
    bool clicksound = false;
    static Result<std::shared_ptr<SettingV3>> parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
        auto res = std::make_shared<ClicksoundSetterV3>();
        auto root = checkJson(json, "selectionclicks");
        res->parseBaseProperties(key, modID, root);
        root.has("clicksound").into(res->clicksound);
        root.checkUnknownKeys();
        return root.ok(std::static_pointer_cast<SettingV3>(res));
    }

    SettingNodeV3* createNode(float width) override;
};

template <>
struct geode::SettingTypeForValueType<ClicksoundSettingValue> {
    using SettingType = ClicksoundSetterV3;
};

class ModSettingsPopup : public CCNode {};

inline bool GeodeLoader_Theme(CCSprite* sprite) {
     std::string texturePath = "";
     if (auto textureProtocol = typeinfo_cast<CCTextureProtocol*>(sprite)) {
        if (auto texture = textureProtocol->getTexture()) {
            auto* cachedTextures = CCTextureCache::sharedTextureCache()->m_pTextures;
            for (auto [key, obj] : CCDictionaryExt<std::string, CCTexture2D*>(cachedTextures)) {
                if (obj == texture) {
                    texturePath= key.c_str();
                    break;
                }
            }
        }
     }
    return texturePath.find("geode.loader") != std::string::npos;
};

inline bool parentcheck(CCNode* node) {
    while (node != nullptr) {
            if (auto x = typeinfo_cast<ModSettingsPopup*>(node)) {
                return GeodeLoader_Theme(x->getChildByType<CCLayer>(0)->getChildByType<ListBorders>(0)->getChildByType<CCSprite>(0));
            }
            node = node->getParent();
        }
        return false;
 };

class ClicksoundSetterNodeV3 : public SettingValueNodeV3<ClicksoundSetterV3> {
protected:
    std::vector<CCMenuItemToggler*> m_toggles;
    std::vector<std::pair<CCMenuItemToggler*,const char* >> m_togglerItems;
    CCMenuItemSpriteExtra* m_folderBtn;
    CCMenuItemSpriteExtra* m_popup;
    CCMenuItemSpriteExtra* m_downloadBtn;
    CCMenuItemSpriteExtra* m_cspiBtn;
    CCMenuItemSpriteExtra* m_clearBtn;
    CCMenu* m_menufolder;
    CCMenu* m_selectionpopup;
    bool m_ThemeGeode = false;
    CCLabelBMFont* m_nameLabel;
    bool cs = false;
    bool m_cspiFilePickerOpen = false;

    void Popup(CCObject*) {
        Mod* mod = Mod::get();
        if (mod->getSavedValue<bool>("CSINDEXDOWNLOADING")) {
            FLAlertLayer::create("Click Sounds Index", "Unable to load while downloading. Please wait until the download completes, then try again.", "Close")->show();
            return;
        } else if (mod->getSavedValue<bool>("CSINDEXCLEARING")) {
            FLAlertLayer::create("Click Sounds Index", "Unable to load while clearing. Please wait a few seconds for the index to finish clearing, then try again.", "Close")->show();
            return;
        } else if (mod->getSavedValue<bool>("CSINDEXRELOAD")) {
            mod->setSavedValue<bool>("CSINDEXRELOAD", false);
            ClickJson->loadData([=, this]() {
                queueInMainThread([=, this]() {
                    onsettingsUpdate();
                    // this is scuffed as hell but without it, the selection menu needs to be opened twice to reload
                        auto popup = Select::create(static_cast<int>(this->getValue().m_tab) == 0, cs, [=, this](std::string modid) {
                            ClicksoundSettingValue Changes = this->getValue();
                            if (static_cast<int>(this->getValue().m_tab) == 0) {
                                Changes.m_currentMemeClick = modid;
                            } else {
                                Changes.m_currentClick = modid;
                            }
                            this->setValue(Changes, nullptr);
                            this->onCommit();
                        }, m_ThemeGeode);
                        popup->m_noElasticity = false;
                        popup->show();
                });
            });
            return;
        }
        auto popup = Select::create(static_cast<int>(this->getValue().m_tab) == 0,cs,[=, this](std::string modid) {
                ClicksoundSettingValue Changes = this->getValue();
                if (static_cast<int>(this->getValue().m_tab) == 0) {
                    Changes.m_currentMemeClick = modid;
                } else {
                    Changes.m_currentClick = modid;
                }
                this->setValue(Changes, nullptr);
                this->onCommit();
        },m_ThemeGeode);
        popup->m_noElasticity = false;
        popup->show();
    };
    
    bool init(std::shared_ptr<ClicksoundSetterV3> setting, float width) {
        if (!SettingValueNodeV3::init(setting, width))
            return false;

        queueInMainThread([=, this] {
            m_ThemeGeode = parentcheck(this->getNameMenu());
            if (m_ThemeGeode) {
                for (auto& value : m_togglerItems) {
                    auto toggle = value.first;
                    toggle->m_onButton->setSprite(ButtonSprite::create(value.second, 40.f, true, SpritePicker::get("bigFont.fnt", m_ThemeGeode), SpritePicker::get("GJ_button_01.png", m_ThemeGeode), 20.f, 1.0f));
                    toggle->m_offButton->setSprite(ButtonSprite::create(value.second, 40.f, true, SpritePicker::get("bigFont.fnt", m_ThemeGeode), SpritePicker::get("GJ_button_04.png", m_ThemeGeode), 20.f, 1.0f));
                }
            }
        });
    
        cs = setting->clicksound;
        this->setContentSize({ width, 70.f });
        CCSprite* folderSpr = CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png");
        folderSpr->setScale(1.0f);
        m_folderBtn = CCMenuItemSpriteExtra::create(
            folderSpr,
            this,
            menu_selector(ClicksoundSetterNodeV3::onFolder)
        );
        m_nameLabel = CCLabelBMFont::create("", "bigFont.fnt");
        this->removeChild(this->getNameMenu(), false);
        this->removeChild(this->getButtonMenu(), false);
        this->getNameMenu()->setLayout(
            RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(AxisAlignment::Start)
        );
        
        this->addChildAtPosition(this->getNameMenu(), Anchor::TopLeft, ccp(10, 0), ccp(0, 1.0f));
        this->addChildAtPosition(this->getButtonMenu(), Anchor::TopRight, ccp(-10, 0), ccp(1.0f, 1.0f));
        this->getStatusLabel()->setPosition(this->getNameMenu()->getPosition() - ccp(0, this->getNameMenu()->getContentHeight() + 2));
        this->getStatusLabel()->setScale(0.175);
        this->getStatusLabel()->setAnchorPoint({0, 1});
        this->getButtonMenu()->setScale(0.9);
        this->getNameMenu()->setScale(1.2);
        m_menufolder = CCMenu::create();
        m_menufolder->addChild(m_folderBtn);
        m_menufolder->setScale(1);
        m_menufolder->setLayout(RowLayout::create());
        m_menufolder->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
        this->addChild(m_menufolder);
    
        m_selectionpopup = CCMenu::create();
        auto btnspr = CCSprite::create("csindexlogo.png"_spr);
        btnspr->setScale(0.5);
        this->m_popup = CCMenuItemSpriteExtra::create(
            btnspr,
            this,
            menu_selector(ClicksoundSetterNodeV3::Popup)
        );

        auto downloadSpr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
        downloadSpr->setScale(0.75);
        this->m_downloadBtn = CCMenuItemSpriteExtra::create(
            downloadSpr,
            this,
            menu_selector(ClicksoundSetterNodeV3::onDownloadBtn)
        );

        auto cspiSpr = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        cspiSpr->setScale(0.55);
        this->m_cspiBtn = CCMenuItemSpriteExtra::create(
            cspiSpr,
            this,
            menu_selector(ClicksoundSetterNodeV3::onCspiBtn)
        );

        auto clearSpr = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
        clearSpr->setScale(0.6);
        this->m_clearBtn = CCMenuItemSpriteExtra::create(
            clearSpr,
            this,
            menu_selector(ClicksoundSetterNodeV3::onClearBtn)
        );

        m_selectionpopup->addChild(this->m_cspiBtn);
        m_selectionpopup->addChild(this->m_downloadBtn);
        m_selectionpopup->addChild(this->m_popup);
        m_selectionpopup->addChild(this->m_clearBtn);
        auto m_selectionpopuplayout = RowLayout::create();
        m_selectionpopuplayout->setGap(15.f);
        m_selectionpopup->setLayout(m_selectionpopuplayout);
        m_selectionpopup->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
        m_selectionpopup->setAnchorPoint({0.5, 0.5});
        this->addChild(m_selectionpopup);
        
        m_nameLabel->setPosition(m_selectionpopup->getPosition() - ccp(0, m_selectionpopup->getContentSize().height));
        m_nameLabel->setScale(0.5);
        m_nameLabel->setAnchorPoint({0.5, 0});
        this->addChild(m_nameLabel);
    
        int count = 0;
        for (auto value : {
            std::make_pair(0, "Meme"),
            std::make_pair(1, "Useful")//,
            //std::make_pair(2, "Custom")
        }) {
            count += 40;
            auto offSpr = ButtonSprite::create(value.second, 40.f, false, "bigFont.fnt", "GJ_button_04.png", 20.f, 1.0f);
            auto onSpr = ButtonSprite::create(value.second, 40.f, false, "bigFont.fnt", "GJ_button_01.png", 20.f, 1.0f);
            offSpr->setOpacity(90);
            auto toggle = CCMenuItemToggler::create(
                offSpr, onSpr, this, menu_selector(ClicksoundSetterNodeV3::onToggle)
            );
            toggle->m_notClickable = true;
            toggle->setTag(static_cast<int>(value.first));
            m_toggles.push_back(toggle);
            m_togglerItems.emplace_back(toggle, value.second);
            this->getButtonMenu()->addChild(toggle);
        }
        this->getButtonMenu()->setLayout(RowLayout::create());
        this->getButtonMenu()->updateLayout();

        this->updateState(nullptr);
        return true;
    }

    void onDownloadBtn(CCObject* sender) {
        geode::createQuickPopup(
            "Warning",
            "The Click Sounds Index is over <cj>50mb+</c> in size. Are you sure you want to redownload it?\n<cr>All existing click packs will be deleted and replaced with the downloaded index.</c>",
            "Cancel", "Download", 
            [](auto, bool btn2) {
                if (btn2) {
                    SendRequestAPI(true);
                }
            }
        );
    };

    void onClearBtn(CCObject* sender) {   
        if (Mod::get()->getSavedValue<bool>("CSINDEXDOWNLOADING")) {
            FLAlertLayer::create("Click Sounds Index", "Unable to clear index while downloading. Please wait until the download completes, then try again. \n\nIf the download takes too long, restarting Geometry Dash will stop the download.", "Close")->show();
            return;
        }

        geode::createQuickPopup(
            "Warning",
            "Are you sure you want to clear the Click Sounds Index? This will <cr>delete all downloaded click packs</c> and cannot be undone.\n\n<cg>.packgen.zip</c> click packs will need to be reinstalled.",
            "Cancel", "Clear", 
            [](auto, bool btn2) {
                if (btn2) {
                    std::thread([] {
                        Mod* mod = Mod::get();
                        mod->setSavedValue<bool>("CSINDEXCLEARING", true);
                        std::filesystem::path dir = mod->getConfigDir() / "Clicks" / "clicks-main";

                        if (std::filesystem::exists(dir / "Meme"))
                            std::filesystem::remove_all(dir / "Meme");
                        if (std::filesystem::exists(dir / "Useful"))
                            std::filesystem::remove_all(dir / "Useful");

                        std::filesystem::create_directory(dir / "Meme");
                        std::filesystem::create_directory(dir / "Useful");

                        extractDefaultClickPack();

                        Loader::get()->queueInMainThread([mod] {
                            FLAlertLayer::create("Click Sounds", "Successfully cleared index!", "Close")->show();
                            mod->setSavedValue<bool>("CSINDEXRELOAD", true);
                            mod->setSavedValue<bool>("CSINDEXCLEARING", false);
                        });
                    }).detach();
                }
            }
        );
    };

    // cspi code start

    void onCspiBtn() {
        Mod::get()->getSavedValue<bool>("read-warnings") ? cspiStartPopup() : cspiIntroPopup();
    };

    void cspiStartPopup() {
		auto cspiPopup = geode::createQuickPopup(
			"Click Sounds",
			"Press <cl>Continue</c> and select a <cg>.packgen.zip</c> click pack file to install it.\n\n\n",
			"Cancel", "Continue",
			[this](auto, bool btn2) {
				if (btn2) {
					cspiFileSelection();
				}
			}
		);
        auto packgenSpr = ButtonSprite::create("Create Click Pack");
        auto packgenBtn = CCMenuItemSpriteExtra::create(
            packgenSpr,
            this,
            menu_selector(ClicksoundSetterNodeV3::onCspiPackgenBtn)
        );
        packgenBtn->setID("packgen-button"_spr);
        packgenSpr->setScale(0.75f);
        packgenBtn->setPosition(0.f, cspiPopup->getContentSize().height / 8);
        cspiPopup->m_buttonMenu->addChild(packgenBtn);
	}

    void onCspiPackgenBtn(CCObject* sender) {
        cocos2d::CCApplication::sharedApplication()->openURL("https://clicksounds.github.io/clicks/packgen.html");
    }

    void cspiIntroPopup(int i = 0) {
		static const std::vector<std::pair<std::string, std::string>> msgContent = {
			{"<cr>Read these instructions carefully, as they will not be repeated.</c>", "Next"},
			{"To make a pack, use the pack generator on the Click Sounds website.", "Next"},
			{"Only <cg>.packgen.zip</c> click pack files can be installed. <cr>Packs from ZCB Live are not compatible.</c>", "Next"},
			{"To convert a <cr>ZCB Live click pack</c> to <cg>.packgen.zip</c>, use the pack generator on the Click Sounds website.", "Next"},
			{"A guide to installing custom click packs is available on the Click Sounds website.", "Close"}
		};
	
		if (i >= msgContent.size()) {
			Mod::get()->setSavedValue<bool>("read-warnings", true);
            cspiStartPopup();
			return;
		}
	
		geode::createQuickPopup("Click Sounds", msgContent[i].first.c_str(), msgContent[i].second.c_str(), nullptr, [this, i](auto, bool btn1) {
			if (!btn1) {
				cspiIntroPopup(i + 1);
			}
		});
	}

    void cspiFileSelection() {
		std::filesystem::path dir = Loader::get()->getInstalledMod("beat.click-sound")->getConfigDir() / "Clicks" / "clicks-main";
		file::FilePickOptions::Filter textFilter;
		file::FilePickOptions fileOptions;
		textFilter.description = "Click Pack";
		textFilter.files = { "*.packgen.zip" };
		fileOptions.filters.push_back(textFilter);
		auto getPersistentDir = Mod::get()->getSavedValue<std::filesystem::path>("cspi-persistent-dir");

		if (m_cspiFilePickerOpen) return;
		m_cspiFilePickerOpen = true;

        async::spawn(
            file::pick(file::PickMode::OpenFile, { getPersistentDir, { textFilter } }),
            [this, dir](Result<std::optional<std::filesystem::path>> res) {
                if (!res || !res.isOk()) return false;
                if (res.isOk()) {
                    auto opt = res.unwrap();
                    if (opt) {
                        auto path = opt.value();
                        if (path.extension() != ".zip") {
                            FLAlertLayer::create("Click Sounds", "Invalid click pack: Not in .packgen.zip format!", "Close")->show();
                            m_cspiFilePickerOpen = false;
                            return false;
                        }

                        Mod::get()->setSavedValue<std::filesystem::path>("cspi-persistent-dir", path);

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
                        } else {
                            FLAlertLayer::create("Click Sounds", "Invalid click pack: Not in .packgen.zip format!", "Close")->show();
                            m_cspiFilePickerOpen = false;
                            return false;
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
                            fmt::format("{} pack installed successfully!", type),
                            "Close", nullptr,
                            [tempZipPath, this](auto, bool) {
                                std::filesystem::remove(tempZipPath);
                                m_cspiFilePickerOpen = false;
                            }
                        );

                        Loader::get()->getInstalledMod("beat.click-sound")->setSavedValue("CSINDEXRELOAD", true);
                        return false;
                    } else {
                        m_cspiFilePickerOpen = false;
                        return false;
                    }

                    
                }
                m_cspiFilePickerOpen = false;
                return true;
            }
        );
	}

    std::string GetJsonName(CategoryData Information) {
        if (!Information.jsonpath.empty() && std::filesystem::exists(Information.jsonpath)) {
            std::filesystem::path fs = std::filesystem::path(Information.jsonpath);
            std::ifstream file(fs, std::ios::in | std::ios::binary);
                if (file.is_open()) {
                    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                    file.close();
                    matjson::Value jsonObject = matjson::parse(content).unwrapOrDefault();

                    if (jsonObject.contains("name") && jsonObject["name"].isString()) {
                        return jsonObject["name"].asString().unwrap();
                    }
                };
            };

        return "";
    };

    void updateState(CCNode* invoker) override {
        SettingValueNodeV3::updateState(invoker);
        float shouldEnable = this->getSetting()->shouldEnable();

        m_menufolder->setVisible(static_cast<int>(this->getValue().m_tab) == 2);
        m_selectionpopup->setVisible(static_cast<int>(this->getValue().m_tab) != 2);
        if (this->getValue().m_tab == 2) {
            std::error_code ec;
            auto Custompa = this->getValue().CustomSoundPath;
            if (Custompa.empty() || Custompa == " " || !std::filesystem::is_regular_file(Custompa, ec)) {
                m_nameLabel->setColor(ccGRAY);
                m_nameLabel->setOpacity(155);
                m_nameLabel->setString("");
            } else {
                std::filesystem::path filePath(Custompa);
                m_nameLabel->setString(filePath.filename().string().c_str());
                m_nameLabel->setColor(ccWHITE);
                m_nameLabel->setOpacity(255);
            }
        }
        if (this->getValue().m_tab == 1) {
            // current
            auto Custompa = this->getValue().m_currentClick;
            if (Custompa.empty() || Custompa == " " || !ClickJson->hassomedata || ClickJson->usefulData.find(Custompa) == ClickJson->usefulData.end()) {
                    m_nameLabel->setColor(ccGRAY);
                    m_nameLabel->setOpacity(155);
                    m_nameLabel->setString("");
                    m_selectionpopup->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height * 0.4f));
                } else {
                    m_nameLabel->setString(GetJsonName(ClickJson->usefulData.at(Custompa)).c_str());
                    m_nameLabel->setColor(ccWHITE);
                    m_nameLabel->setOpacity(255);
                    m_selectionpopup->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height * 0.5f));
                }
        }
        if (this->getValue().m_tab == 0) {
            // meme
            auto Custompa = this->getValue().m_currentMemeClick;
            if (Custompa.empty() || Custompa == " " || !ClickJson->hassomedata || ClickJson->memeData.find(Custompa) ==  ClickJson->memeData.end()) {
                    m_nameLabel->setColor(ccGRAY);
                    m_nameLabel->setOpacity(155);
                    m_nameLabel->setString("");
                    m_selectionpopup->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height * 0.4f));
                } else {
                    m_nameLabel->setString(GetJsonName(ClickJson->memeData.at(Custompa)).c_str());
                    m_nameLabel->setColor(ccWHITE);
                    m_nameLabel->setOpacity(255);
                    m_selectionpopup->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height * 0.5f));
                }
        }

        m_folderBtn->setEnabled(shouldEnable);
        m_popup->setEnabled(shouldEnable);
        m_downloadBtn->setEnabled(shouldEnable);
        m_cspiBtn->setEnabled(shouldEnable);
        m_clearBtn->setEnabled(shouldEnable);

        if (!shouldEnable) {
           m_nameLabel->setColor(ccGRAY);
        }
        for (auto toggle : m_toggles) {
            if (shouldEnable) {
                toggle->toggle(toggle->getTag() == static_cast<int>(this->getValue().m_tab));
                toggle->setVisible(true);
            } else {
              (toggle->toggle(false)); 
              (toggle->setVisible(false));
            }
            toggle->setEnabled(shouldEnable);
            auto children = toggle->getChildren();
            for (auto children : CCArrayExt<CCNode*>(children)) {
                children->setScale(1);
            }
        }
    }
    void onToggle(CCObject* sender) {
        ClicksoundSettingValue Changes = this->getValue();
        Changes.m_tab = sender->getTag();
        this->setValue(Changes, static_cast<CCNode*>(sender));
        this->onCommit();
    }
     void onFolder(CCObject* sender) {
        file::FilePickOptions::Filter textFilter;
        file::FilePickOptions fileOptions;
        textFilter.description = "Sound effect";
        textFilter.files = {"*.ogg", "*.mp3", "*.wav"};
        fileOptions.filters.push_back(textFilter);

        async::spawn(
            file::pick(file::PickMode::OpenFile, { Mod::get()->getResourcesDir(), { textFilter } }), 
            [this,sender](Result<std::optional<std::filesystem::path>> res) {
                if (res.isOk()) {
                    auto path = res.unwrap();
                    ClicksoundSettingValue Changes = this->getValue();
                    Changes.CustomSoundPath = path->string().c_str();
                    this->setValue(Changes, static_cast<CCNode*>(sender));
                    this->onCommit();
                }
            }
        );

    }

public:
    static ClicksoundSetterNodeV3* create(std::shared_ptr<ClicksoundSetterV3> setting, float width) {
        auto ret = new ClicksoundSetterNodeV3();
        if (ret && ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

inline SettingNodeV3* ClicksoundSetterV3::createNode(float width) {
    return ClicksoundSetterNodeV3::create(std::static_pointer_cast<ClicksoundSetterV3>(shared_from_this()), width);
}

$execute {
    log::debug("Loading");
        auto ret = Mod::get()->registerCustomSettingType("selectionclicks", &ClicksoundSetterV3::parse);
        if (!ret) {
            log::error("Unable to register setting type: {}", ret.unwrapErr());
        }
}