#pragma once
#include <Geode/Geode.hpp>
#include "../jsonReader/Json.hpp"
#include "popup_nodes/CCIndexPackNode.hpp"
#include "SelectionEnum.hpp"
#include "../ButtonSprites/Sprite.hpp"
#include <regex>
using namespace geode::prelude;

extern void SendRequestAPI(bool forceDownload);

class Select : public geode::Popup<> {
protected:
    bool m_theme = false;
    double m_minsize;
    std::function<void(std::string)> m_settings;
    ScrollLayer* scroll;
    TextInput* searchBar;
    

    CCIndexPackNode* Item(auto send, auto modid, bool meme) {
        return CCIndexPackNode::create(send, [=]() {
            m_settings(modid);
            this->onClose(nullptr);
        }, m_theme);
    }

    void closebtntheme(bool theme) {
        m_theme = theme;
        if (theme) {
            this->setCloseButtonSpr(
                CircleButtonSprite::createWithSpriteFrameName(
                    "geode.loader/close.png", .85f,
                    CircleBaseColor::DarkPurple
                )
            );
        }
    }

    bool setup() {
        auto winSize = CCDirector::get()->getWinSize();

        scroll = ScrollLayer::create(ccp(
            m_mainLayer->getContentSize().width - 30,
            m_mainLayer->getContentSize().height - 70
        ));
        scroll->setAnchorPoint(ccp(0, 0));
        scroll->setPosition(ccp(15, 15));
        m_mainLayer->addChild(scroll);

        addSearchBar();
        addDownloadBtn();
        return true;
    }
    void addSearchBar() {
        auto winSize = m_mainLayer->getContentSize();

        auto searchBarBg = CCLayerColor::create(ccc4(0, 0, 0, 100), 210.f, 35.f);
        searchBarBg->setAnchorPoint(ccp(0.5f, 1));
        searchBarBg->setPosition(ccp(winSize.width / 2, winSize.height - 50));
        m_mainLayer->addChild(searchBarBg, -1);
        searchBar = TextInput::create(
            winSize.width / 2, "Search...", "bigFont.fnt"
        );

        searchBar->setCallback([=](std::string input){
            filterItems(input);
        });

        searchBar->setAnchorPoint(ccp(0.5f, 1));
        searchBar->setPosition(ccp(winSize.width / 2, winSize.height - 15));
        m_mainLayer->addChild(searchBar);

        auto divider = CCLayerColor::create(ccc4(200, 200, 200, 100), winSize.width - 30, 2);
        divider->setAnchorPoint(ccp(0, 1));
        divider->setPosition(ccp(15, winSize.height - 50));
        m_mainLayer->addChild(divider);

        auto indexLabel = CCLabelBMFont::create("Index", "bigFont.fnt");
        indexLabel->setAnchorPoint(ccp(0.5f, 1));
        indexLabel->setPosition(ccp(winSize.width / 7, winSize.height - 20));
        indexLabel->setScale(0.7f);
        indexLabel->setID("index-label"_spr);
        m_mainLayer->addChild(indexLabel);

    }

    void filterItems(const std::string& query) {
        auto NodeScroller = scroll->m_contentLayer;
        CCArrayExt<CCNode*> objects = NodeScroller->getChildren();
        double maxcount =0 ;
        for (auto* obj : objects) {
            if (CCIndexPackNode* Cell = typeinfo_cast<CCIndexPackNode*>(obj)) {
                std::regex pattern(query, std::regex::icase);
                bool matches =  std::regex_search(Cell->getName(), pattern);
                obj->setVisible(matches);
            }
             if (obj->isVisible()) maxcount+=1 ;
        }
        int basePosY = 180;
        int i = -1;
        float height = std::max<float>(m_minsize, 40 * maxcount);
        for (auto* obj : objects) {
            if (obj->isVisible()) {
                i++;
                obj->setPositionY(height - (40 * i));
            } else {obj->setPositionY(-10000);}
        }
        NodeScroller->setContentSize(ccp(NodeScroller->getContentSize().width, height));
        scroll->moveToTop();
    }


    bool CreateWithArgs(bool meme, bool clicksound, std::function<void(std::string)> setting) {
        auto NodeScroller = scroll->m_contentLayer;
        m_settings = setting;
        int basePosY = 180;
    
        auto json = (meme) ? ClickJson->memeData : ClickJson->usefulData;
        bool hasPacks = false;
    
        for (const auto& [filename, data] : json) {
            if (data.clicks.empty() && clicksound) {
                continue;
            }
            if (data.releases.empty() && !clicksound) {
                continue;
            }
    
            hasPacks = true;
            auto Object = Item(data, filename, clicksound);
            Object->setPositionY(basePosY);
            NodeScroller->addChild(Object);
        }
    
        if (!hasPacks) {
            std::string message = "No click packs were found. \nPlease download the index\nfrom the upper right corner\nor install a .packgen.zip pack.";
            auto noPacksLabel = CCLabelBMFont::create(message.c_str(), "bigFont.fnt");
            noPacksLabel->setAnchorPoint(ccp(0.5f, 0.5f));
            noPacksLabel->setScale(0.5f);
            noPacksLabel->setAlignment(kCCTextAlignmentCenter);
            noPacksLabel->setPosition(ccp(scroll->getContentSize().width / 2, scroll->getContentSize().height / 1.75));
            NodeScroller->addChild(noPacksLabel);
        }
    
        m_minsize = scroll->getContentSize().height;
        float height = std::max<float>(m_minsize, 40 * NodeScroller->getChildrenCount());
        NodeScroller->setContentSize(ccp(NodeScroller->getContentSize().width, height));
        if (hasPacks) {
            CCArrayExt<CCNode*> objects = NodeScroller->getChildren();
            int i = -1;
            for (auto* obj : objects) {
                i++;
                obj->setPositionY(height - (40 * i));
            }
        }
        scroll->moveToTop();
    
        return true;
    }

    void addDownloadBtn() {
        auto downloadSpr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
        //downloadSpr->setScale(1);
        auto downloadBtn = CCMenuItemSpriteExtra::create(
            downloadSpr,
            this,
            menu_selector(Select::onDownloadBtn)
        );
        auto downloadMenu = CCMenu::create();
        downloadMenu->setID("download-index-menu");
        downloadBtn->setID("download-index-button");
        downloadMenu->setPosition(ccp(m_mainLayer->getContentSize().width, m_mainLayer->getContentSize().height));
        downloadMenu->addChild(downloadBtn);
        m_mainLayer->addChild(downloadMenu);
    }

    void onDownloadBtn(CCObject* sender) {
        geode::createQuickPopup(
            "Warning",
            "The Click Sounds Index is over <cj>50mb+</c> in size. Are you sure you want to redownload it?",
            "Cancel", "Download", 
            [](auto, bool btn2) {
                if (btn2) {
                    SendRequestAPI(true);
                }
            });
    };

public:
    static Select* create(bool meme = false, bool clicksound = true, std::function<void(std::string)> setting = [](std::string x) {}, bool theme = false) {
        auto ret = new Select;

        if (ret && ret->initAnchored(420.f, 270.f, SpritePicker::get("GJ_square01.png", theme))) {
            ret->autorelease();
            ret->closebtntheme(theme);
            ret->CreateWithArgs(meme, clicksound, setting);
            ret->setID("SoundSelector"_spr);
            return ret;
        }

        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
