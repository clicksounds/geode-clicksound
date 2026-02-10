#pragma once
#include <Geode/Geode.hpp>
#include "../jsonReader/Json.hpp"
#include "popup_nodes/CSIndexPackNode.hpp"
#include "SelectionEnum.hpp"
#include "../ButtonSprites/Sprite.hpp"
#include <regex>
using namespace geode::prelude;

extern void SendRequestAPI(bool forceDownload);

class Select : public geode::Popup {
protected:
    bool m_theme = false;
    double m_minsize = 0.0;
    std::function<void(std::string)> m_settings;
    ScrollLayer* scroll = nullptr;
    TextInput* searchBar = nullptr;
    bool m_featuredOnlyToggled = false;

    CSIndexPackNode* Item(auto send, auto modid, bool meme) {
        return CSIndexPackNode::create(send, [=]() {
            m_settings(modid);
            this->onClose(nullptr);
        }, m_theme);
    }

    void closebtntheme(bool theme) {
        if (!theme) return;
        setCloseButtonSpr(
            CircleButtonSprite::createWithSpriteFrameName(
                "geode.loader/close.png",
                .85f,
                CircleBaseColor::DarkPurple
            )
        );
    }

    bool init(
        bool meme,
        bool clicksound,
        std::function<void(std::string)> setting,
        bool theme
    ) {
        m_theme = theme;
        m_settings = setting;

        if (!Popup::init(
                420.f,
                270.f,
                SpritePicker::get("GJ_square01.png", theme)
            ))
            return false;

        closebtntheme(theme);
        setup();
        CreateWithArgs(meme, clicksound, setting);
        setID("SoundSelector"_spr);
        return true;
    }

    bool setup() {
        scroll = ScrollLayer::create(ccp(
            m_mainLayer->getContentSize().width - 30,
            m_mainLayer->getContentSize().height - 70
        ));
        scroll->setAnchorPoint(ccp(0, 0));
        scroll->setPosition(ccp(15, 15));
        m_mainLayer->addChild(scroll);

        addSearchBar();
        addDownloadBtn();
        addFeatureFilterBtn();
        return true;
    }

    void addSearchBar() {
        auto winSize = m_mainLayer->getContentSize();

        auto searchBarBg = CCLayerColor::create(ccc4(0, 0, 0, 100), 210.f, 35.f);
        searchBarBg->setAnchorPoint(ccp(0.5f, 1));
        searchBarBg->setPosition(ccp(winSize.width / 2, winSize.height - 50));
        m_mainLayer->addChild(searchBarBg, -1);

        searchBar = TextInput::create(
            winSize.width / 2,
            "Search...",
            "bigFont.fnt"
        );

        searchBar->setCallback([this](std::string input) {
            m_featuredOnlyToggled = false;
            auto btn = static_cast<CCMenuItemSpriteExtra*>(m_mainLayer->getChildByID("feature-filter-menu"_spr)->getChildByID("feature-filter-btn"_spr));
            btn->setOpacity(90);
            filterItems(input);
        });

        searchBar->setAnchorPoint(ccp(0.5f, 1));
        searchBar->setPosition(ccp(winSize.width / 2, winSize.height - 15));
        m_mainLayer->addChild(searchBar);

        auto divider = CCLayerColor::create(
            ccc4(200, 200, 200, 100),
            winSize.width - 30,
            2
        );
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

        double visibleCount = 0;
        std::regex pattern(query, std::regex::icase);

        for (auto* obj : objects) {
            if (auto* cell = typeinfo_cast<CSIndexPackNode*>(obj)) {
                bool matches = std::regex_search(cell->getName(), pattern);
                obj->setVisible(matches);
            }
            if (obj->isVisible()) visibleCount++;
        }

        float height = std::max<float>(m_minsize, 40.f * visibleCount);

        int i = -1;
        for (auto* obj : objects) {
            if (obj->isVisible()) {
                i++;
                obj->setPositionY(height - (40.f * i));
            } else {
                obj->setPositionY(-10000.f);
            }
        }

        NodeScroller->setContentSize(
            ccp(NodeScroller->getContentSize().width, height)
        );
        scroll->moveToTop();
    }

    bool CreateWithArgs(
        bool meme,
        bool clicksound,
        std::function<void(std::string)> setting
    ) {
        auto NodeScroller = scroll->m_contentLayer;

        auto json = meme
            ? ClickJson->memeData
            : ClickJson->usefulData;

        bool hasPacks = false;

        for (const auto& [filename, data] : json) {
            if (data.clicks.empty() && clicksound) continue;
            if (data.releases.empty() && !clicksound) continue;

            hasPacks = true;
            auto obj = Item(data, filename, clicksound);
            NodeScroller->addChild(obj);
        }

        if (!hasPacks) {
            auto label = CCLabelBMFont::create(
                "No click packs were found.\n"
                "Please download the index\n"
                "from the upper right corner\n"
                "or install a .packgen.zip pack.",
                "bigFont.fnt"
            );
            label->setScale(0.5f);
            label->setAlignment(kCCTextAlignmentCenter);
            label->setPosition(ccp(
                scroll->getContentSize().width / 2,
                scroll->getContentSize().height / 1.75f
            ));
            NodeScroller->addChild(label);
        }

        m_minsize = scroll->getContentSize().height;
        float height = std::max<float>(
            m_minsize,
            40.f * NodeScroller->getChildrenCount()
        );

        NodeScroller->setContentSize(
            ccp(NodeScroller->getContentSize().width, height)
        );

        if (hasPacks) {
            CCArrayExt<CCNode*> objects = NodeScroller->getChildren();
            int i = -1;
            for (auto* obj : objects) {
                i++;
                obj->setPositionY(height - (40.f * i));
            }
        }

        scroll->moveToTop();
        return true;
    }

    void addDownloadBtn() {
        auto spr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
        auto btn = CCMenuItemSpriteExtra::create(
            spr,
            this,
            menu_selector(Select::onDownloadBtn)
        );

        auto menu = CCMenu::create();
        menu->setPosition(ccp(
            m_mainLayer->getContentSize().width,
            m_mainLayer->getContentSize().height
        ));
        menu->addChild(btn);
        m_mainLayer->addChild(menu);
    }

    void onDownloadBtn(CCObject*) {
        geode::createQuickPopup(
            "Warning",
            "The Click Sounds Index is over <cj>50mb+</c> in size. "
            "Are you sure you want to redownload it?",
            "Cancel",
            "Download",
            [](auto, bool yes) {
                if (yes) SendRequestAPI(true);
            }
        );
    }

    void addFeatureFilterBtn() {
        auto spr = CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png");
        auto btn = CCMenuItemSpriteExtra::create(
            spr,
            this,
            menu_selector(Select::onFeatureFilterBtn)
        );
        btn->setOpacity(90);
        btn->setID("feature-filter-btn"_spr);

        auto menu = CCMenu::create();
        menu->setPosition(ccp(
            m_mainLayer->getContentSize().width - 55,
            m_mainLayer->getContentSize().height - 30
        ));
        menu->addChild(btn);
        menu->setID("feature-filter-menu"_spr);
        m_mainLayer->addChild(menu);
    }

    void onFeatureFilterBtn(CCObject*) {
        auto btn = static_cast<CCMenuItemSpriteExtra*>(m_mainLayer->getChildByID("feature-filter-menu"_spr)->getChildByID("feature-filter-btn"_spr));
        m_featuredOnlyToggled = !m_featuredOnlyToggled;
        searchBar->setString("");

        if (!m_featuredOnlyToggled) {
            filterItems(searchBar->getString());
            btn->setOpacity(90);
            return;
        } else {
            btn->setOpacity(255);
            auto NodeScroller = scroll->m_contentLayer;
            CCArrayExt<CCNode*> objects = NodeScroller->getChildren();

            double visibleCount = 0;

            for (auto* obj : objects) {
                if (auto* cell = typeinfo_cast<CSIndexPackNode*>(obj)) {
                    bool isFeatured = cell->isFeaturedPack();
                    obj->setVisible(isFeatured);
                }
                if (obj->isVisible()) visibleCount++;
            }

            float height = std::max<float>(m_minsize, 40.f * visibleCount);

            int i = -1;
            for (auto* obj : objects) {
                if (obj->isVisible()) {
                    i++;
                    obj->setPositionY(height - (40.f * i));
                } else {
                    obj->setPositionY(-10000.f);
                }
            }

            NodeScroller->setContentSize(
                ccp(NodeScroller->getContentSize().width, height)
            );
            scroll->moveToTop();
        }
    }

public:
    static Select* create(
        bool meme = false,
        bool clicksound = true,
        std::function<void(std::string)> setting = [](std::string) {},
        bool theme = false
    ) {
        auto ret = new Select;
        if (ret && ret->init(meme, clicksound, setting, theme)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};