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
#include "../../jsonReader/Json.hpp"
using namespace geode::prelude;
#define MEN(class) class = CCMenu::create(); 
class CCIndexPackNode : public CCLayerColor {
public:
    CategoryData Infomation;
    CCLabelBMFont* Text;
    CCLabelBMFont* Author;
    CCMenu* _Apply_Menu;
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

    void getlistfull() {
         if (!Infomation.jsonpath.empty() && std::filesystem::exists(Infomation.jsonpath)) {
            std::filesystem::path fs = std::filesystem::path(Infomation.jsonpath);
            std::ifstream file(fs, std::ios::in | std::ios::binary);
                if (file.is_open()) {
                    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                    file.close();
                    try {
                        matjson::Value jsonObject2 = matjson::parse(content).unwrap();

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
                        

                        
                    } else {
                        std::cerr << "\"authors\" key is missing or not an array in the JSON." << std::endl;
                    }

                    } catch (const std::exception& e) {
                        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                    }
                } else {
                    std::cerr << "Failed to open the file: " << fs << std::endl;
                }
            }
    }
     bool init(CategoryData x) {
            if (!CCLayerColor::init())
                return false;
            Infomation = x;
            
            this->setContentSize(ccp(315, 35));
            this->setAnchorPoint(ccp(0, 1));
            this->setPositionY(207);
            this->setOpacity(100);
            //Text = CCLabelBMFont::create("ITEM NODE", "goldFont.fnt");
            Text = CCLabelBMFont::create("ITEM NODE", "bigFont.fnt");
            Text->setID("name-label");
            Text->setLayoutOptions(AxisLayoutOptions::create()->setScalePriority(1));
            if (!Infomation.jsonpath.empty() && std::filesystem::exists(Infomation.jsonpath)) {
            std::filesystem::path fs = std::filesystem::path(Infomation.jsonpath);
            std::ifstream file(fs, std::ios::in | std::ios::binary);
                if (file.is_open()) {
                    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                    file.close();
                    try {
                        matjson::Value jsonObject = matjson::parse(content).unwrap();

                        if (jsonObject.contains("name")) {
                            std::string name = jsonObject["name"].asString().unwrap();
                            Text->setString(name.c_str());
                            Text->updateAnchoredPosition(Anchor::Top, ccp(0, -10), ccp(.5f, .5f));
                            //limitNodeWidth(Text, this->getContentSize() - CCSize(this->getContentSize().width, 0), .8f, .1f);
                            Text->setScale(clamp((this->getContentSize().width / 3) / Text->getContentSize().width, 0.1f, 0.5f));
                            Text->updateLayout();
                        } else {
                            std::cerr << "\"name\" key not found in the JSON." << std::endl;
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                    }
                } else {
                    std::cerr << "Failed to open the file: " << fs << std::endl;
                }
            }
            this->getlistfull();
            this->addChildAtPosition(Text, Anchor::TopLeft, ccp(3, 0), ccp(0, 1.0f));

            Author = CCLabelBMFont::create("ITEM NODE", "goldFont.fnt");
            Author->setID("Author-label");
            Author->setLayoutOptions(AxisLayoutOptions::create()->setScalePriority(1));
            Author->setScale(0.5);
            std::string authorsList = "by ";
            int Number = 0;
            if (!Infomation.jsonpath.empty() && std::filesystem::exists(Infomation.jsonpath)) {
            std::filesystem::path fs = std::filesystem::path(Infomation.jsonpath);
            std::ifstream file(fs, std::ios::in | std::ios::binary);
                if (file.is_open()) {
                    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                    file.close();
                    try {
                        matjson::Value jsonObject2 = matjson::parse(content).unwrap();

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
                                        add_sill = true;  
                                        authorsList += author["name"].asString().unwrap();
                                        if (authorsList.length() > 11) {
                                            Author->setScale(clampf( (11  / authorsList.length()), 0.2,0.5));
                                        }
                                        continue;
                                    }
                                }

                                authorsList += author["name"].asString().unwrap();
                                if (authorsList.length() > 15 && add_sill) {
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

                        Author->setString(authorsList.c_str());
                        MEN(DEVS)
                        DEVS->setID("developers");
                        DEVS->ignoreAnchorPointForPosition(false);
                        //Author->updateAnchoredPosition(Anchor::Bottom, ccp(0, -10), ccp(.5f, .5f));
                        auto developersBtn = CCMenuItemSpriteExtra::create(
                            Author, this, menu_selector(CCIndexPackNode::OnDevelopers)
                        );
                        developersBtn->setID("developers-button");
                        developersBtn->setAnchorPoint({0,0});
                        DEVS->addChild(developersBtn);
                        DEVS->updateAnchoredPosition(Anchor::Bottom, ccp(0, 0), ccp(0, 0));
                        DEVS->updateLayout();
                    } else {
                        std::cerr << "\"authors\" key is missing or not an array in the JSON." << std::endl;
                    }

                    } catch (const std::exception& e) {
                        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                    }
                } else {
                    std::cerr << "Failed to open the file: " << fs << std::endl;
                }
            }
            this->addChildAtPosition(DEVS, Anchor::BottomLeft, ccp(3, 0), ccp(0, 0));

        CCLayerGradient* gradient = CCLayerGradient::create(ccc4(0, 0, 0, 100), ccc4(0, 0, 0, 100));
        gradient->setContentSize(this->getContentSize());
        gradient->setZOrder(-3);
        gradient->setVector(ccp(90, 0));
        this->addChild(gradient);
        this->setOpacity(0); 
            // GJ_button_06
        auto ConfirmSprite = ButtonSprite::create("Set", 40.f, true, "bigFont.fnt", "GJ_button_01.png", 20.f, 1.0f);
        MEN(_Apply_Menu)
        _Apply_Menu->setID("apply");
        _Apply_Menu->ignoreAnchorPointForPosition(false);
        _Apply_Menu->addChild(ConfirmSprite);
        _Apply_Menu->updateLayout();
        this->addChildAtPosition(_Apply_Menu, Anchor::BottomRight, ccp(3, 0), ccp(0, 0));
        _Apply_Menu->setAnchorPoint({0.250,-0.05});
        return true;
     }
     static CCIndexPackNode* create(CategoryData x) {
            CCIndexPackNode* pRet = new CCIndexPackNode();
            if (pRet && pRet->init(x)) {
                pRet->autorelease();
                return pRet;
            } else {
                delete pRet;
                return nullptr;
            }
        };
};
