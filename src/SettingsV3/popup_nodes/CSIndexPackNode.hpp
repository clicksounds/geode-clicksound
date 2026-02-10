#pragma once

#include "../../ButtonSprites/Sprite.hpp"
#include "../../jsonReader/Json.hpp"
#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/web.hpp>
#include <fstream>
#include <iostream>
using namespace geode::prelude;

inline float clampf_f(float value, float min, float max) {
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
class CSIndexPackNode : public CCLayerColor {
  public:
	CategoryData Information;
	CCLabelBMFont *Text;
	CCLabelBMFont *Author;
	CCMenu *_Apply_Menu;
	std::string m_name = "";
	std::function<void()> selectionobject;
	CCMenu *DEVS;
	std::string authorsListWhole = "";
	std::string packID = "";
	std::string packDescription = "";
	bool isFeatured;
	void OnDevelopers(auto sender) {
		MDPopup::create(
		    "More Info",
		    "Author(s): " + authorsListWhole + packID + packDescription,
		    "OK", nullptr,
			[](bool) {})
		    ->show();
	};
	std::string getName() {
		return m_name;
	};
	bool isFeaturedPack() {
		return isFeatured;
	}
	void selected(CCObject *) {
		if (selectionobject) {
			// todo update setting
			selectionobject();
		}
	}
	void favorited(CCObject *) {
		FLAlertLayer::create("Click Sounds", "Implementing now", "Close")->show();
	}
	void getlistfull() {
		if (!Information.jsonpath.empty() && std::filesystem::exists(Information.jsonpath)) {
			std::filesystem::path fs = std::filesystem::path(Information.jsonpath);
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

				if (jsonObject2.contains("id") && jsonObject2["id"].isString()) {
					std::string id = jsonObject2["id"].asString().unwrap();
					if (!id.empty()) {
						packID += "\n\nPack ID: " + id;
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
	bool init(CategoryData x, std::function<void()> Objectt, bool theme) {
		if (!CCLayerColor::init())
			return false;
		selectionobject = Objectt;
		Information = x;

		this->setContentSize(ccp(390, 35));
		this->setAnchorPoint(ccp(0, 1));
		this->setPositionY(207);
		this->setOpacity(100);
		// Text = CCLabelBMFont::create("ITEM NODE", "goldFont.fnt");
		Text = AutoScaleCCLabelBMFont::create("ITEM NODE", "bigFont.fnt", 200, 50);
		Text->setID("name-label");
		Text->setLayoutOptions(AxisLayoutOptions::create()->setScalePriority(1));
		if (!Information.jsonpath.empty() && std::filesystem::exists(Information.jsonpath)) {
			std::filesystem::path fs = std::filesystem::path(Information.jsonpath);
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

				// check if featured
				if (jsonObject.contains("id") && jsonObject["id"].isString()) {
					std::string id = jsonObject["id"].asString().unwrap();
					auto featuredPath = Mod::get()->getConfigDir() / "Clicks" / "clicks-main" / "featured_list.json";
					if (std::filesystem::exists(featuredPath)) {
						auto featuredJsonData = utils::file::readJson(featuredPath).unwrapOr(-2);
						if (featuredJsonData == -2) log::debug("couldnt unwrap featured_list.json");
						if (featuredJsonData.isArray()) {
							auto& array = featuredJsonData.asArray().unwrap();
							for (size_t i = 0; i < array.size(); i++) {
								const auto& jsonElement = array[i];
								if (jsonElement.isString() && jsonElement.asString().unwrap() == id) {
									isFeatured = true;
									break;
								}
							}
						}

					}
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
		if (!Information.jsonpath.empty() && std::filesystem::exists(Information.jsonpath)) {
			std::filesystem::path fs = std::filesystem::path(Information.jsonpath);
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
					DEVS = CCMenu::create(); // MEN(DEVS)
					DEVS->setID("developers");
					DEVS->ignoreAnchorPointForPosition(false);
					// Author->updateAnchoredPosition(Anchor::Bottom, ccp(0, -10), ccp(.5f, .5f));
					auto developersBtn = CCMenuItemSpriteExtra::create(
					    Author, this, menu_selector(CSIndexPackNode::OnDevelopers));
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

		// gradient
		CCLayerGradient *gradient = CCLayerGradient::create(ccc4(0, 0, 0, 100), ccc4(0, 0, 0, 100));
		gradient->setContentSize(this->getContentSize());
		gradient->setZOrder(-3);
		gradient->setVector(ccp(90, 0));
		this->addChild(gradient);
		this->setOpacity(0);

		// featured glow
		CCScale9Sprite *FeaturedGlow = CCScale9Sprite::create("featuredglow.png"_spr);
		FeaturedGlow->setPreferredSize(CCSize(this->getContentSize().width, this->getContentSize().height + 5.f));
		FeaturedGlow->setID("FeaturedGlow");
		FeaturedGlow->setZOrder(-1);
		FeaturedGlow->setAnchorPoint(CCPoint(0.f, 0.1f));
		FeaturedGlow->setOpacity(155);

		if (isFeatured) this->addChild(FeaturedGlow);

		// 'set' button
		auto ConfirmSprite = CCMenuItemSpriteExtra::create(ButtonSprite::create("Set", 40.f, true, SpritePicker::get("bigFont.fnt", theme), SpritePicker::get("GJ_button_01.png", theme), 20.f, 1.0f), this, menu_selector(CSIndexPackNode::selected));
		ConfirmSprite->m_scaleMultiplier = 0.9;
		_Apply_Menu = CCMenu::create(); // MEN(_Apply_Menu)
		_Apply_Menu->setID("apply");
		_Apply_Menu->ignoreAnchorPointForPosition(false);
		_Apply_Menu->setLayout(RowLayout::create()
		                           ->setAxisAlignment(AxisAlignment::Start)
		                           ->setCrossAxisLineAlignment(AxisAlignment::Start)
		                           ->setCrossAxisAlignment(AxisAlignment::Start));
		_Apply_Menu->setContentSize(ConfirmSprite->getContentSize());
		_Apply_Menu->setPosition(ccp(this->getContentSize().width - ConfirmSprite->getContentSize().width / 1.5, this->getContentSize().height / 2));
		if (isFeatured) {
			CCSprite* featuredSpr = CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png");
			featuredSpr->setScale(0.75f);
			featuredSpr->setID("featured-star");
			//_Apply_Menu->setPosition(CCPoint(_Apply_Menu->getPosition().x - 10.f, _Apply_Menu->getPosition().y));
			featuredSpr->setPosition(CCPoint(_Apply_Menu->getPosition().x - 45.f, _Apply_Menu->getPosition().y));
			this->addChild(featuredSpr);
		}
		_Apply_Menu->addChild(ConfirmSprite);	
		_Apply_Menu->updateLayout();
		this->addChild(_Apply_Menu);
		_Apply_Menu->setAnchorPoint({0.5, 0.5});
		return true;
	}
	static CSIndexPackNode *create(CategoryData x, std::function<void()> Objectt, bool theme) {
		CSIndexPackNode *pRet = new CSIndexPackNode();
		if (pRet && pRet->init(x, Objectt, theme)) {
			pRet->autorelease();
			return pRet;
		} else {
			delete pRet;
			return nullptr;
		}
	};
};
