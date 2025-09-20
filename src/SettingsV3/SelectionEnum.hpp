#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/General.hpp>
#include <Geode/utils/web.hpp>
#include "popup.hpp"
#include "../ButtonSprites/Sprite.hpp"
#include "../jsonReader/Json.hpp"
#include <cctype>
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace geode::prelude;

extern void onsettingsUpdate();
extern void SendRequestAPI(bool forceDownload);

static struct ClicksoundSettingValue {
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
                return Ok(ClicksoundSettingValue(1, "beat.default", " ", " "));
            });
        }
        auto value = matjson::parse(x).unwrapOrDefault();
            
        return Ok(ClicksoundSettingValue(
            value["Tab"].asInt().unwrapOr(1), 
            value["Current_Sound_Useful"].asString().unwrapOr("beat.default"), 
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

bool GeodeLoader_Theme(CCSprite* sprite) {
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

bool parentcheck(CCNode* node) {
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
    CCMenu* m_menufolder;
    CCMenu* m_selectionpopup;
    bool m_ThemeGeode = false;
    CCLabelBMFont* m_nameLabel;
    bool cs = false;
    void Popup(CCObject*) {
        if (Mod::get()->getSavedValue<bool>("CSINDEXDOWNLOADING")) {
            FLAlertLayer::create("Click Sounds Index", "Unable to load while downloading. Please wait until the download completes, then try again.", "Close")->show();
            return;
        } else if (Mod::get()->getSavedValue<bool>("CSINDEXRELOAD")) {
            Mod::get()->setSavedValue<bool>("CSINDEXRELOAD", false);
            ClickJson->loadData([=]() {
                onsettingsUpdate();
                // this is scuffed as hell but without it, the selection menu needs to be opened twice to reload
                queueInMainThread([=]() {
                    auto popup = Select::create(static_cast<int>(this->getValue().m_tab) == 0, cs, [=](std::string modid) {
                        ClicksoundSettingValue Changes = this->getValue();
                        if (static_cast<int>(this->getValue().m_tab) == 0) {
                            Changes.m_currentMemeClick = modid;
                        } else {
                            Changes.m_currentClick = modid;
                        }
                        this->setValue(Changes, nullptr);
                    }, m_ThemeGeode);
                    popup->m_noElasticity = false;
                    popup->show();
                });
            });
            return;
        }
        auto popup = Select::create(static_cast<int>(this->getValue().m_tab) == 0,cs,[=](std::string modid) {
                ClicksoundSettingValue Changes = this->getValue();
                if (static_cast<int>(this->getValue().m_tab) == 0) {
                    Changes.m_currentMemeClick = modid;
                } else {
                    Changes.m_currentClick = modid;
                }
                this->setValue(Changes, nullptr);
        },m_ThemeGeode);
        popup->m_noElasticity = false;
        popup->show();
    };
    
    bool init(std::shared_ptr<ClicksoundSetterV3> setting, float width) {
        if (!SettingValueNodeV3::init(setting, width))
            return false;
        
        queueInMainThread([=] {
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
        this->getButtonMenu()->setScale(1.2);
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
        auto downloadBtn = CCMenuItemSpriteExtra::create(
            downloadSpr,
            this,
            menu_selector(ClicksoundSetterNodeV3::onDownloadBtn)
        );

        auto cspiSpr = CCSprite::create("cspiicon.png"_spr);
        cspiSpr->setScale(0.75);
        auto cspiBtn = CCMenuItemSpriteExtra::create(
            cspiSpr,
            this,
            menu_selector(ClicksoundSetterNodeV3::onCspiBtn)
        );

        m_selectionpopup->addChild(cspiBtn);
        m_selectionpopup->addChild(downloadBtn);
        m_selectionpopup->addChild(this->m_popup);
        auto m_selectionpopuplayout = RowLayout::create();
        m_selectionpopuplayout->setGap(15.f);
        m_selectionpopup->setLayout(m_selectionpopuplayout);
        m_selectionpopup->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
        m_selectionpopup->setAnchorPoint({0.5, 0.5});
        this->addChild(m_selectionpopup);
    
        m_nameLabel->setPosition(m_menufolder->getPosition() - ccp(0, m_menufolder->getContentSize().height));
        m_nameLabel->setScale(0.5);
        m_nameLabel->setAnchorPoint({0.5, 0});
        this->addChild(m_nameLabel);
    
        int count = 0;
        for (auto value : {
            std::make_pair(0, "Meme"),
            std::make_pair(1, "Useful"),
            std::make_pair(2, "Custom")
        }) {
            count += 40;
            auto offSpr = ButtonSprite::create(value.second, 40.f, true, "bigFont.fnt", "GJ_button_04.png", 20.f, 1.0f);
            offSpr->setOpacity(90);
            auto onSpr = ButtonSprite::create(value.second, 40.f, true, "bigFont.fnt", "GJ_button_01.png", 20.f, 1.0f);
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
            "The Click Sounds Index is over <cj>30mb+</c> in size. Are you sure you want to redownload it?",
            "Cancel", "Download", 
            [](auto, bool btn2) {
                if (btn2) {
                    SendRequestAPI(true);
                }
            });
    };

    // cspi code start

    void onCspiBtn(CCObject* sender) {
        geode::createQuickPopup(
            "CSPI Title",
            "CSPI Text",
            "Close"
        );
    };

    std::string GetJsonName(CategoryData Infomation) {
        if (!Infomation.jsonpath.empty() && std::filesystem::exists(Infomation.jsonpath)) {
            std::filesystem::path fs = std::filesystem::path(Infomation.jsonpath);
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
                } else {
                    m_nameLabel->setString(GetJsonName(ClickJson->usefulData.at(Custompa)).c_str());
                    m_nameLabel->setColor(ccWHITE);
                    m_nameLabel->setOpacity(255);
                }
        }
        if (this->getValue().m_tab == 0) {
            // meme
            auto Custompa = this->getValue().m_currentMemeClick;
            if (Custompa.empty() || Custompa == " " || !ClickJson->hassomedata || ClickJson->memeData.find(Custompa) ==  ClickJson->memeData.end()) {
                    m_nameLabel->setColor(ccGRAY);
                    m_nameLabel->setOpacity(155);
                    m_nameLabel->setString("");
                } else {
                    m_nameLabel->setString(GetJsonName(ClickJson->memeData.at(Custompa)).c_str());
                    m_nameLabel->setColor(ccWHITE);
                    m_nameLabel->setOpacity(255);
                }
        }
        m_folderBtn->setEnabled(shouldEnable);
        m_popup->setEnabled(shouldEnable);
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
    }
     void onFolder(CCObject* sender) {
        file::FilePickOptions::Filter textFilter;
        file::FilePickOptions fileOptions;
        textFilter.description = "Sound effect";
        textFilter.files = {"*.ogg", "*.mp3", "*.wav"};
        fileOptions.filters.push_back(textFilter);

        file::pick(file::PickMode::OpenFile, { Mod::get()->getResourcesDir(), { textFilter } }).listen([this,sender](Result<std::filesystem::path>* res) {
            if (res->isOk()) {
                    std::filesystem::path path = res->unwrap();
                    ClicksoundSettingValue Changes = this->getValue();
                    Changes.CustomSoundPath = path.string().c_str();
                    this->setValue(Changes, static_cast<CCNode*>(sender));
                }
            });
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

SettingNodeV3* ClicksoundSetterV3::createNode(float width) {
    return ClicksoundSetterNodeV3::create(std::static_pointer_cast<ClicksoundSetterV3>(shared_from_this()), width);
}

$execute {
    log::debug("Loading");
        auto ret = Mod::get()->registerCustomSettingType("selectionclicks", &ClicksoundSetterV3::parse);
        if (!ret) {
            log::error("Unable to register setting type: {}", ret.unwrapErr());
        }
}