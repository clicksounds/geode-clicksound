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
	std::string packID;
	std::string packVersion;
	void OnDevelopers(auto sender) {
		MDPopup::create(
		    "More Info",
		    "Author(s): " + authorsListWhole + packID + packVersion + packDescription,
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

				if (jsonObject2.contains("id") && jsonObject2["id"].isString()) {
					std::string id = jsonObject2["id"].asString().unwrap();
					if (!id.empty()) {
						packID += "\n\nID: " + id;
					}
				}

				if (jsonObject2.contains("version") && jsonObject2["version"].isNumber()) {
					packVersion = "\n\nVersion: v" + std::to_string(jsonObject2["version"].asInt().unwrap());
				} else {
					packVersion = "\n\nVersion: v1 (No version specified)";
				}
			}
		}
	}
	bool init(CategoryData x, std::function<void()> Objectt, bool theme) {
		if (!CCLayerColor::init())
			return false;
		selectionobject = Objectt;
		Infomation = x;

		this->setContentSize(ccp(390, 35));
		this->setAnchorPoint(ccp(0, 1));
		this->setPositionY(207);
		this->setOpacity(100);
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
		if (Text) this->addChildAtPosition(Text, Anchor::TopLeft, ccp(3, 0), ccp(0, 1.0f));

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
		if (!DEVS) {
			MEN(DEVS)
		}
		if (DEVS) this->addChildAtPosition(DEVS, Anchor::BottomLeft, ccp(3, 0), ccp(0, 0));

		CCLayerGradient *gradient = CCLayerGradient::create(ccc4(0, 0, 0, 100), ccc4(0, 0, 0, 100));
		gradient->setContentSize(this->getContentSize());
		gradient->setZOrder(-3);
		gradient->setVector(ccp(90, 0));
		this->addChild(gradient);
		this->setOpacity(0);
		// GJ_button_06
		auto ConfirmSprite = CCMenuItemSpriteExtra::create(ButtonSprite::create("Set", 40.f, true, SpritePicker::get("bigFont.fnt", theme), SpritePicker::get("GJ_button_01.png", theme), 20.f, 1.0f), this, menu_selector(CCIndexPackNode::selected));
		ConfirmSprite->m_scaleMultiplier = 0.9;
		// Disable if not downloaded
		{
			// Try to extract pack id and type
			std::string packId = "";
			std::string packType = "Useful";
			// Try to get from packID string (set in getlistfull), fallback to m_name
			if (!packID.empty()) {
				// packID is like "\n\nID: beat.default" so extract after last ':'
				size_t pos = packID.rfind(":");
				if (pos != std::string::npos) packId = packID.substr(pos + 1);
				// Remove whitespace
				packId.erase(0, packId.find_first_not_of(" \n\t"));
				packId.erase(packId.find_last_not_of(" \n\t") + 1);
			}
			if (packId.empty() && !m_name.empty()) packId = m_name;
			// Try to guess type from Infomation (if memeData or usefulData)
			if (!Infomation.clicks.empty() || !Infomation.releases.empty()) {
				// If this node is for a meme pack, set type to Meme
				if (Infomation.Name.find("Meme") != std::string::npos) packType = "Meme";
			}
			// Check if directory exists
			if (!packId.empty()) {
				auto configDir = Mod::get()->getConfigDir();
				auto packDir = configDir / "Clicks" / "clicks-main" / packType / packId;
				if (!std::filesystem::exists(packDir)) {
					ConfirmSprite->setEnabled(false);
				}
			}
		}
		MEN(_Apply_Menu)
		_Apply_Menu->setID("apply");
		_Apply_Menu->ignoreAnchorPointForPosition(false);
		_Apply_Menu->addChild(ConfirmSprite);
		_Apply_Menu->setLayout(RowLayout::create()
		                           ->setAxisAlignment(AxisAlignment::Start)
		                           ->setCrossAxisLineAlignment(AxisAlignment::Start)
		                           ->setCrossAxisAlignment(AxisAlignment::Start));
		_Apply_Menu->setContentSize(ConfirmSprite->getContentSize());
		_Apply_Menu->setPosition(ccp(this->getContentSize().width - ConfirmSprite->getContentSize().width / 1.5, this->getContentSize().height / 2));
		_Apply_Menu->updateLayout();
		this->addChild(_Apply_Menu);
		_Apply_Menu->setAnchorPoint({0.5, 0.5});
		return true;
	}
	static CCIndexPackNode *create(CategoryData x, std::function<void()> Objectt, bool theme) {
		CCIndexPackNode *pRet = new CCIndexPackNode();
		if (pRet && pRet->init(x, Objectt, theme)) {
			pRet->autorelease();
			return pRet;
		} else {
			delete pRet;
			return nullptr;
		}
	};
};
