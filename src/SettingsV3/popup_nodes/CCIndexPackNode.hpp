#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/ui/TextInput.hpp>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <Geode/utils/file.hpp>
#include "../popup.hpp"
#include "../../jsonReader/Json.hpp"
#include "../../ButtonSprites/Sprite.hpp"
using namespace geode::prelude;

float clampf_f(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

class AutoScaleCCLabelBMFont : public cocos2d::CCLabelBMFont {
public:
    float startsize = .5f;

    static AutoScaleCCLabelBMFont* create(const char* str, const char* fntFile, float width, float height) {
        AutoScaleCCLabelBMFont* label = new AutoScaleCCLabelBMFont();
        if (label && label->initWithString(str, fntFile, width, kCCTextAlignmentLeft, cocos2d::CCPointZero)) {
            label->autorelease();
            label->maxHeight = height;
            return label;
        }
        CC_SAFE_DELETE(label);
        return nullptr;
    }

    virtual void setString(const char* labelText) override {
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
    CCLabelBMFont* Text;
    CCLabelBMFont* Author;
    CCMenu* _Apply_Menu;
    std::function<void()> selectionobject;
    CCMenu* DEVS;
    std::string authorsListWhole = "";
    void OnDevelopers(auto sender) {
        FLAlertLayer::create( 
            nullptr,
            "Developers",
            "The Developers for the sound are "+authorsListWhole,  
            "OK",nullptr, 420.f,true,210.f,1.f
            )->show();
    };
    void selected(CCObject*) {
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
                        for (const auto& author : jsonObject2["authors"].asArray().unwrap()) {
                            if (author.contains("name") && author["name"].isString()) {
                               
        
                                if (!author["name"].asString().unwrap().empty() && add_sill) {
                                    authorsListWhole += ", ";
                                } else {
                                    if (!author["name"].asString().unwrap().empty()) {
                                        add_sill = true;  
                                    }
                                }

                                authorsListWhole += author["name"].asString().unwrap();
                                
                            }
                        }
                        

                        
                    }
                }
            }
    }
     bool init(CategoryData x, std::function<void()> Objectt, bool theme) {
            if (!CCLayerColor::init())
                return false;
            selectionobject = Objectt;
            Infomation = x;
            
            this->setContentSize(ccp(250, 35));
            this->setAnchorPoint(ccp(0, 1));
            this->setPositionY(207);
            this->setOpacity(100);
            //Text = CCLabelBMFont::create("ITEM NODE", "goldFont.fnt");
            float boxWidth = 210.f;  
            float boxHeight = 32.f;
            float scaleFactor = 0.5f;
            Text = AutoScaleCCLabelBMFont::create("ITEM NODE", "bigFont.fnt", 150, 50); 
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
                        Text->updateAnchoredPosition(Anchor::Top, ccp(0, -10), ccp(.5f, .5f));
                        //limitNodeWidth(Text, this->getContentSize() - CCSize(this->getContentSize().width, 0), .8f, .1f);
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
                        for (const auto& author : jsonObject2["authors"].asArray().unwrap()) {
                            if (author.contains("name") && author["name"].isString()) {
                                old = authorsList;
                                if (countonly) {
                                    if (!author["name"].asString().unwrap().empty()) {
                                        Number+=1; 
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
                           authorsList+=" + "+std::to_string(Number);
                           authorsList+=" more";
                        }

                        Author->setScale(0.5f);
                        Author->setString(authorsList.c_str());
                        Author->updateLabel();
                        MEN(DEVS)
                        DEVS->setID("developers");
                        DEVS->ignoreAnchorPointForPosition(false);
                        //Author->updateAnchoredPosition(Anchor::Bottom, ccp(0, -10), ccp(.5f, .5f));
                        auto developersBtn = CCMenuItemSpriteExtra::create(
                            Author, this, menu_selector(CCIndexPackNode::OnDevelopers)
                        );
                        developersBtn->m_scaleMultiplier = 1.1;
                        developersBtn->setID("developers-button");
                        developersBtn->setAnchorPoint({0,0});
                        DEVS->addChild(developersBtn);
                        DEVS->updateAnchoredPosition(Anchor::Bottom, ccp(0, 0), ccp(0, 0));
                        DEVS->updateLayout();
                    }
                }
            }
            this->addChildAtPosition(DEVS, Anchor::BottomLeft, ccp(3, 0), ccp(0, 0));

        CCLayerGradient* gradient = CCLayerGradient::create(ccc4(0, 0, 0, 100), ccc4(0, 0, 0, 100));
        gradient->setContentSize(this->getContentSize() + ccp(30,0));
        gradient->setZOrder(-3);
        gradient->setVector(ccp(90, 0));
        this->addChild(gradient);
        this->setOpacity(0); 
            // GJ_button_06
        auto ConfirmSprite = CCMenuItemSpriteExtra::create(ButtonSprite::create("Set", 40.f, true, SpritePicker::get("bigFont.fnt",theme),SpritePicker::get("GJ_button_01.png",theme), 20.f, 1.0f), this, menu_selector(CCIndexPackNode::selected));
        ConfirmSprite->m_scaleMultiplier = 0.9;
        MEN(_Apply_Menu)
        _Apply_Menu->setID("apply");
        _Apply_Menu->ignoreAnchorPointForPosition(false);
        _Apply_Menu->addChild(ConfirmSprite);
        _Apply_Menu->setLayout(RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(AxisAlignment::Start)
        );
        _Apply_Menu->setContentSize(ConfirmSprite->getContentSize());
        _Apply_Menu->setPosition(ccp(this->getContentSize().width,this->getContentSize().height / 2));
        _Apply_Menu->updateLayout();
        this->addChild(_Apply_Menu);
        _Apply_Menu->setAnchorPoint({0.5,0.5});
        return true;
     }
     static CCIndexPackNode* create(CategoryData x, std::function<void()> Objectt, bool theme) {
            CCIndexPackNode* pRet = new CCIndexPackNode();
            if (pRet && pRet->init(x,Objectt,theme)) {
                pRet->autorelease();
                return pRet;
            } else {
                delete pRet;
                return nullptr;
            }
        };
};
