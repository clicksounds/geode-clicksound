#pragma once

#include "../../ButtonSprites/Sprite.hpp"
#include "../../jsonReader/Json.hpp"
#include "../popup.hpp"
#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/web.hpp>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
using namespace geode::prelude;

extern void onsettingsUpdate();

float clampf_f(float value, float min, float max) {
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

class AutoScaleCCLabelBMFont : public cocos2d::CCLabelBMFont {
  public:
	float startsize = .5f;

	static AutoScaleCCLabelBMFont *create(const char *str, const char *fntFile, float width, float height) {
		AutoScaleCCLabelBMFont *label = new AutoScaleCCLabelBMFont();
		if (label && label->initWithString(str, fntFile, width, kCCTextAlignmentLeft, cocos2d::CCPointZero)) {
			label->autorelease();
			label->maxHeight = height;
			return label;
		}
		CC_SAFE_DELETE(label);
		return nullptr;
	}

	virtual void setString(const char *labelText) override {
		CCLabelBMFont::setString(labelText);
		float scaleY = maxHeight / this->getContentHeight();
		if (scaleY < 1) {
			this->setScale(scaleY * startsize);
		} else {
			this->setScale(startsize);
		}

		scaleY = maxHeight / this->getContentHeight();
		if (scaleY < 1) {
			this->setScale(scaleY * startsize);
		}
	}

  protected:
	float maxHeight;
};

#define MEN(class) class = CCMenu::create();
class CCIndexPackNode : public CCLayerColor {
  public:
	CategoryData Infomation;
	CCLabelBMFont *Text;
	CCLabelBMFont *Author;
	CCMenu *_Apply_Menu;
	std::string m_name = "";
	std::function<void()> selectionobject;
	CCMenu *DEVS;
	std::string authorsListWhole = "";
	std::string packDescription;
	bool isClickSection = true; // Add a flag to indicate if this node is for clicks or releases
	void OnDevelopers(auto sender) {
		MDPopup::create(
		    "More Info",
		    "Author(s): " + authorsListWhole + packDescription,
		    "OK", nullptr,
			[](bool) {})
		    ->show();
	};
	std::string getName() {
		return m_name;
	};
	void selected(CCObject *) {
		if (selectionobject) {
			// todo update setting
			selectionobject();
		}
	}
	void getlistfull() {
		if (!Infomation.jsonpath.empty() && std::filesystem::exists(Infomation.jsonpath)) {
			std::filesystem::path fs = std::filesystem::path(Infomation.jsonpath);
			std::ifstream file(fs, std::ios::in | std::ios::binary);
			if (file.is_open()) {
				std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				file.close();
				matjson::Value jsonObject2 = matjson::parse(content).unwrapOrDefault();
	
				if (jsonObject2.contains("authors") && jsonObject2["authors"].isArray()) {
					bool add_sill = false;
					for (const auto &author : jsonObject2["authors"].asArray().unwrap()) {
						if (author.contains("name") && author["name"].isString()) {
							std::string name = author["name"].asString().unwrap();
							if (!name.empty()) {
								if (add_sill) {
									authorsListWhole += ", ";
								} else {
									add_sill = true;
								}
								if (author.contains("gdAccountID") && author["gdAccountID"].isNumber()) {
									authorsListWhole += "[" + name + "](user:" + std::to_string(author["gdAccountID"].asInt().unwrap()) + ")";
								} else {
									authorsListWhole += name;
								}
							}
						}
					}
				}
	
				if (jsonObject2.contains("description") && jsonObject2["description"].isString()) {
					std::string description = jsonObject2["description"].asString().unwrap();
					if (!description.empty()) {
						packDescription += "\n\nPack Description:\n" + description;
					}
				}
			}
		}
	}
	bool init(CategoryData x, std::function<void()> Objectt, bool theme, bool isClick = true) {
		if (!CCLayerColor::init())
			return false;
		selectionobject = Objectt;
		Infomation = x;
		isClickSection = isClick;

		this->setContentSize(ccp(390, 35));
		this->setAnchorPoint(ccp(0, 1));
		this->setPositionY(207);
		this->setOpacity(100);
		// Text = CCLabelBMFont::create("ITEM NODE", "goldFont.fnt");
		Text = AutoScaleCCLabelBMFont::create("ITEM NODE", "bigFont.fnt", 200, 50);
		Text->setID("name-label");
		Text->setLayoutOptions(AxisLayoutOptions::create()->setScalePriority(1));
		if (!Infomation.jsonpath.empty() && std::filesystem::exists(Infomation.jsonpath)) {
			std::filesystem::path fs = std::filesystem::path(Infomation.jsonpath);
			std::ifstream file(fs, std::ios::in | std::ios::binary);
			if (file.is_open()) {
				std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				file.close();
				matjson::Value jsonObject = matjson::parse(content).unwrapOrDefault();

				if (jsonObject.contains("name") && jsonObject["name"].isString()) {
					std::string name = jsonObject["name"].asString().unwrap();
					m_name = name;
					Text->updateAnchoredPosition(Anchor::Top, ccp(0, -10), ccp(.5f, .5f));
					// limitNodeWidth(Text, this->getContentSize() - CCSize(this->getContentSize().width, 0), .8f, .1f);
					Text->setScale(0.5f);
					Text->setString(name.c_str());
				}
			}
		}
		this->getlistfull();
		this->addChildAtPosition(Text, Anchor::TopLeft, ccp(3, 0), ccp(0, 1.0f));

		Author = CCLabelBMFont::create("ITEM NODE", "goldFont.fnt");
		Author->setID("Author-label");
		Author->setLayoutOptions(AxisLayoutOptions::create()->setScalePriority(1));
		std::string authorsList = "by ";
		int Number = 0;
		if (!Infomation.jsonpath.empty() && std::filesystem::exists(Infomation.jsonpath)) {
			std::filesystem::path fs = std::filesystem::path(Infomation.jsonpath);
			std::ifstream file(fs, std::ios::in | std::ios::binary);
			if (file.is_open()) {
				std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				file.close();
				matjson::Value jsonObject2 = matjson::parse(content).unwrapOrDefault();

				if (jsonObject2.contains("authors") && jsonObject2["authors"].isArray()) {
					bool add_sill = false;
					bool countonly = false;
					std::string old = authorsList;
					for (const auto &author : jsonObject2["authors"].asArray().unwrap()) {
						if (author.contains("name") && author["name"].isString()) {
							old = authorsList;
							if (countonly) {
								if (!author["name"].asString().unwrap().empty()) {
									Number += 1;
								};
								continue;
							}
							if (!author["name"].asString().unwrap().empty() && add_sill) {
								authorsList += ", ";
							} else {
								if (!author["name"].asString().unwrap().empty()) {
									std::string name = author["name"].asString().unwrap();
									add_sill = true;
									if (name.length() > 13) {
										name = name.substr(0, 13) + "...";
									}
									authorsList += name;
									continue;
								}
							}

							if (add_sill) {
								authorsList = old;
								Number += 1;
								countonly = true;
							}
						}
					}
					if (Number > 0) {
						authorsList += " + " + std::to_string(Number);
						authorsList += " more";
					}

					Author->setScale(0.5f);
					Author->setString(authorsList.c_str());
					Author->updateLabel();
					MEN(DEVS)
					DEVS->setID("developers");
					DEVS->ignoreAnchorPointForPosition(false);
					// Author->updateAnchoredPosition(Anchor::Bottom, ccp(0, -10), ccp(.5f, .5f));
					auto developersBtn = CCMenuItemSpriteExtra::create(
					    Author, this, menu_selector(CCIndexPackNode::OnDevelopers));
					developersBtn->m_scaleMultiplier = 1.1;
					developersBtn->setID("developers-button");
					developersBtn->setAnchorPoint({0, 0});
					DEVS->addChild(developersBtn);
					DEVS->updateAnchoredPosition(Anchor::Bottom, ccp(0, 0), ccp(0, 0));
					DEVS->updateLayout();
				}
			}
		}
		this->addChildAtPosition(DEVS, Anchor::BottomLeft, ccp(3, 0), ccp(0, 0));

		CCLayerGradient *gradient = CCLayerGradient::create(ccc4(0, 0, 0, 100), ccc4(0, 0, 0, 100));
		gradient->setContentSize(this->getContentSize());
		gradient->setZOrder(-3);
		gradient->setVector(ccp(90, 0));
		this->addChild(gradient);
		this->setOpacity(0);
		// GJ_button_06
		auto ConfirmSprite = CCMenuItemSpriteExtra::create(ButtonSprite::create("Set", 40.f, true, SpritePicker::get("bigFont.fnt", theme), SpritePicker::get("GJ_button_01.png", theme), 20.f, 1.0f), this, menu_selector(CCIndexPackNode::selected));
        ConfirmSprite->m_scaleMultiplier = 0.9;
        MEN(_Apply_Menu)
        _Apply_Menu->setID("apply");
        _Apply_Menu->ignoreAnchorPointForPosition(false);
        float spacing = 8.0f;
        float totalWidth = ConfirmSprite->getContentSize().width;
	//CCMenuItemSpriteExtra* BothSprite = nullptr;
	// Use parsed CategoryData vectors (populated by JsonReader) to determine availability
	if (!Infomation.clicks.empty() && !Infomation.releases.empty()) {
            /*BothSprite = CCMenuItemSpriteExtra::create(
                ButtonSprite::create("Both", 40.f, true, SpritePicker::get("bigFont.fnt", theme), SpritePicker::get("GJ_button_01.png", theme), 20.f, 1.0f),
                this, menu_selector(CCIndexPackNode::selectedBoth));
            BothSprite->m_scaleMultiplier = 0.9;
            totalWidth += BothSprite->getContentSize().width + spacing;*/
        }
        _Apply_Menu->setContentSize(CCSize(totalWidth, ConfirmSprite->getContentSize().height));
        ConfirmSprite->setAnchorPoint({1, 0.5f});
        ConfirmSprite->setPosition(totalWidth, ConfirmSprite->getContentSize().height / 2);
        _Apply_Menu->addChild(ConfirmSprite);
        /*if (BothSprite) {
            BothSprite->setAnchorPoint({1, 0.5f});
            BothSprite->setPosition(totalWidth - ConfirmSprite->getContentSize().width - spacing, BothSprite->getContentSize().height / 2);
            _Apply_Menu->addChild(BothSprite);
        }*/
        _Apply_Menu->setPosition(ccp(this->getContentSize().width, this->getContentSize().height / 2));
        _Apply_Menu->updateLayout();
        this->addChild(_Apply_Menu);
        _Apply_Menu->setAnchorPoint({1.0, 0.5});

		return true;
	}
	static CCIndexPackNode *create(CategoryData x, std::function<void()> Objectt, bool theme, bool isClick = true) {
		CCIndexPackNode *pRet = new CCIndexPackNode();
		if (pRet && pRet->init(x, Objectt, theme, isClick)) {
			pRet->autorelease();
			return pRet;
		} else {
			delete pRet;
			return nullptr;
		}
	};
	/*bool hasClickAndReleaseSounds(const std::string& folderPath) {
		std::filesystem::path dirPath = folderPath;
		// If a file (likely pack.json) was passed, use its parent folder
		if (std::filesystem::is_regular_file(dirPath)) {
			dirPath = dirPath.parent_path();
		}

		if (!std::filesystem::is_directory(dirPath)) {
			geode::log::info("hasClickAndReleaseSounds: path is not a directory: {}", dirPath.string());
			return false;
		}

		bool hasClick = false;
		bool hasRelease = false;

		// Look for Clicks and Releases subfolders (case-insensitive)
		for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
			if (!entry.is_directory()) continue;
			std::string folder = entry.path().filename().string();
			std::string lower = folder;
			std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
			if (lower == "clicks") {
				for (const auto& f : std::filesystem::directory_iterator(entry.path())) {
					if (f.is_regular_file()) {
						hasClick = true;
						break;
					}
				}
			} else if (lower == "releases") {
				for (const auto& f : std::filesystem::directory_iterator(entry.path())) {
					if (f.is_regular_file()) {
						hasRelease = true;
						break;
					}
				}
			}
			if (hasClick && hasRelease) {
				geode::log::info("hasClickAndReleaseSounds: both Clicks and Releases found in {}", dirPath.string());
				return true;
			}
		}

		if (!hasClick) geode::log::info("hasClickAndReleaseSounds: no Click files in {}", dirPath.string());
		if (!hasRelease) geode::log::info("hasClickAndReleaseSounds: no Release files in {}", dirPath.string());
		return false;
	}
	void selectedBoth(CCObject *) {
		try {
			geode::log::info("Both button pressed");
			bool isMeme = false;
			geode::log::info("isClickSection: {}", isClickSection);
			geode::log::info("Infomation.clicks.size(): {} Infomation.releases.size(): {}", Infomation.clicks.size(), Infomation.releases.size());
			if (!isClickSection && Infomation.clicks.empty() && !Infomation.releases.empty()) {
				isMeme = true;
			}
			geode::log::info("Determined isMeme: {}", isMeme);
			std::string packId = m_name;
			geode::log::info("PackId: {}", packId);
			for (const char* key : {"selection-clicks", "selection-release"}) {
				geode::log::info("Updating setting: {}", key);
				std::string valStr = Mod::get()->getSettingValue<std::string>(key);
				geode::log::info("Old value: {}", valStr);
				matjson::Value val = matjson::parse(valStr).unwrapOrDefault();
				if (isMeme) {
					val["Current_Sound_Meme"] = packId;
					val["Tab"] = 0;
					geode::log::info("Set Current_Sound_Meme to {} and Tab to 0", packId);
				} else {
					val["Current_Sound_Useful"] = packId;
					val["Tab"] = 1;
					geode::log::info("Set Current_Sound_Useful to {} and Tab to 1", packId);
				}
				std::string newVal = val.dump(matjson::NO_INDENTATION);
				geode::log::info("New value: {}", newVal);
				Mod::get()->setSettingValue(key, newVal);
				onsettingsUpdate();
			}
			if (selectionobject) {
				geode::log::info("Calling selectionobject() to close popup");
				selectionobject();
			}
			geode::log::info("Both button logic complete");
		} catch (...) {
			geode::log::error("Failed to set both click and release pack");
		}
	}*/
};