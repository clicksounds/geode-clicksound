#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/SettingNode.hpp>
#include "../utils/Clicks.hpp"
#include "ClickTypes.hpp"
#include <matjson.hpp>
#include <string>
using namespace geode::prelude;


// STOLE FROM VIPER'S BETTER MENU BUT CUSTOMIZED BY COOPER 
class ClickTypeNode : public SettingNode {
protected:
    int m_currentPos;
    int m_currentClick;
    int m_currentMemeClick;
    std::string m_kind;
    std::string m_currentCustomClick;
    CCMenuItemToggler* usefulBtn;
    CCMenuItemToggler* memeBtn;
    CCMenuItemToggler* customBtn;
    CCMenu* menu;
    CCMenu* menu1;
    CCMenu* menu2;
    CCLabelBMFont* label;
    CCLabelBMFont* anotherLabel;
    InputNode* inputNode1;
    InputNode* inputNode2;
    CCSprite* arrowRight;
    CCMenuItemSpriteExtra* buttonRight;
    CCSprite* arrowLeft;
    CCMenuItemSpriteExtra* buttonLeft;
    CCSprite* folderSpr;
    CCMenuItemSpriteExtra* folderBtn;

    int getActiveCornerTag(int corner) {
        switch (corner) {
            case 1: // useful
                return 2004;
            case 2: // meme
                return 2005;
            case 3: // custom
                return 2006;
            default:
                return 2007;
        }
    }
    int tagToCorner(int tag) {
        switch (tag) {
            case 2004: // useful
                return 1;
            case 2005: // meme
                return 2;
            case 2006: // custom
                return 3;
            default:
                return 4;
        }
    }
    template <typename T>
    bool init(T* value, float width, std::string prefixText) {
        if (!SettingNode::init(value))
            return false;

        m_kind = prefixText;
        m_currentPos = value->getTab();
        m_currentClick = value->getClick();
        m_currentMemeClick = value->getMemeClick();
        m_currentCustomClick = value->getCustomClick();
        this->setContentSize({ width, 70.f });

        menu = CCMenu::create();
        menu->setContentSize({ width/2, 20.f} );
        menu->setPosition(width*3/4 - 7.f, 50.f);
        menu->setLayout(RowLayout::create()
      			->setGap(4.f)
      			->setAxisAlignment(AxisAlignment::Center)
      			->setAutoScale(false)
      			->setCrossAxisOverflow(true)
    	);

        usefulBtn = CCMenuItemToggler::create(
            ButtonSprite::create("Useful", 40.f, true, "bigFont.fnt", "GJ_button_01.png", 20.f, 0.5f),
            ButtonSprite::create("Useful", 40.f, true, "bigFont.fnt", "GJ_button_04.png", 20.f, 0.5f),
            this,
            menu_selector(ClickTypeNode::onCornerClick)
        );
        memeBtn = CCMenuItemToggler::create(
            ButtonSprite::create("Meme", 40.f, true, "bigFont.fnt", "GJ_button_01.png", 20.f, 0.5f),
            ButtonSprite::create("Meme", 40.f, true, "bigFont.fnt", "GJ_button_04.png", 20.f, 0.5f),
            this,
            menu_selector(ClickTypeNode::onCornerClick)
        );
        customBtn = CCMenuItemToggler::create(
            ButtonSprite::create("Custom", 40.f, true, "bigFont.fnt", "GJ_button_01.png", 20.f, 0.5f),
            ButtonSprite::create("Custom", 40.f, true, "bigFont.fnt", "GJ_button_04.png", 20.f, 0.5f),
            this,
            menu_selector(ClickTypeNode::onCornerClick)
        );

        usefulBtn->setTag(getActiveCornerTag(1));
        memeBtn->setTag(getActiveCornerTag(2));
        customBtn->setTag(getActiveCornerTag(3));
        usefulBtn->setTag(getActiveCornerTag(1));
        memeBtn->setTag(getActiveCornerTag(2));
        customBtn->setTag(getActiveCornerTag(3));
        int currentCorner = m_currentPos;
        usefulBtn->toggle(!(usefulBtn->getTag() == getActiveCornerTag(currentCorner)));
        memeBtn->toggle(!(memeBtn->getTag() == getActiveCornerTag(currentCorner)));
        customBtn->toggle(!(customBtn->getTag() == getActiveCornerTag(currentCorner)));
        
        this->addChild(menu);
        menu->addChild(usefulBtn);
        menu->addChild(memeBtn);
        menu->addChild(customBtn);

        label = CCLabelBMFont::create(fmt::format("{} Type",prefixText).c_str(), "bigFont.fnt");
        label->setScale(.5F);
        label->setAnchorPoint({ 0.f, 0.5f });
        label->setPosition(20.f, 50.f);

        this->addChild(label);
        menu->updateLayout();

        menu1 = CCMenu::create();
        menu1->setPosition({ width - 20.f, 20.f });
        menu1->setVisible(false);
        menu1->setTag(5001);
        this->addChild(menu1);

        anotherLabel = CCLabelBMFont::create(fmt::format("{} Sound",prefixText).c_str(), "bigFont.fnt");
        anotherLabel->setScale(.5F);
        anotherLabel->setAnchorPoint({ 0.f, 0.5f });
        anotherLabel->setPosition(20.f, 20.f);
        this->addChild(anotherLabel);

        inputNode1 = InputNode::create(200.f, "...", "chatFont.fnt");
        inputNode1->setScale(0.65f);
        inputNode1->setPosition(-75.f, 0);
        inputNode1->setString("...");
        inputNode1->getInput()->setAllowedChars("");
        menu1->addChild(inputNode1);

        arrowRight = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
        arrowRight->setScale(0.3f);
        buttonRight = CCMenuItemSpriteExtra::create(arrowRight, this, menu_selector(ClickTypeNode::onNext));
        menu1->addChild(buttonRight);

        arrowLeft = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
        arrowLeft->setScale(0.3f);
        arrowLeft->setFlipX(true);
        buttonLeft = CCMenuItemSpriteExtra::create(arrowLeft, this, menu_selector(ClickTypeNode::onPrev));
        buttonLeft->setPosition(-150.f, 0);
        menu1->addChild(buttonLeft);

        menu2 = CCMenu::create();
        menu2->setPosition({ width - 20.f, 20.f });
        menu2->setVisible(false);
        menu2->setTag(5002);
        this->addChild(menu2);

        inputNode2 = InputNode::create(200.f, "...", "chatFont.fnt");
        inputNode2->setScale(0.65f);
        inputNode2->setPosition(-80.f, 0);
        inputNode2->setString(m_currentCustomClick.c_str());
        inputNode2->getInput()->setAllowedChars("");
        menu2->addChild(inputNode2);

        folderSpr = CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png");
        folderSpr->setScale(0.5f);
        folderBtn = CCMenuItemSpriteExtra::create(
            folderSpr,
            this,
            menu_selector(ClickTypeNode::onFolder)
        );
        menu2->addChild(folderBtn);

        if (currentCorner == 1) {inputNode1->setString(std::to_string(m_currentClick).c_str());} else if (currentCorner == 2) {inputNode1->setString(std::to_string(m_currentMemeClick).c_str());}

        if (currentCorner == 1 || currentCorner == 2) {
            menu1->setVisible(true);
        } else if (currentCorner == 3) {
            menu2->setVisible(true);
        }
        return true;
    }
    
    void onCornerClick(CCObject* sender) {
        usefulBtn->toggle(true);
        memeBtn->toggle(true);
        customBtn->toggle(true);
        m_currentPos = tagToCorner(sender->getTag());

        if (m_currentPos == 1 || m_currentPos == 2) {
            if(m_currentPos == 1) {
                if(m_kind == "Click"){
                    inputNode1->setString(Clicks::get()->getClickList()["Useful"][m_currentClick]["name"].as_string().c_str());
                } else {
                    inputNode1->setString(Clicks::get()->getReleaseList()["Useful"][m_currentClick]["name"].as_string().c_str());
                }
            } else {
                if(m_kind == "Click"){
                    inputNode1->setString(Clicks::get()->getClickList()["Meme"][m_currentMemeClick]["name"].as_string().c_str());
                } else {
                    inputNode1->setString(Clicks::get()->getReleaseList()["Meme"][m_currentMemeClick]["name"].as_string().c_str());
                }
            } 
            this->getChildByTag(5001)->setVisible(true);
            this->getChildByTag(5002)->setVisible(false);
        } else if (m_currentPos == 3) {
            this->getChildByTag(5001)->setVisible(false);
            this->getChildByTag(5002)->setVisible(true);
        }

        this->dispatchChanged();
    };

    void onNext(CCObject* sender) {
        log::debug("next");
        this->dispatchChanged();
    }

    void onPrev(CCObject* sender) {
        log::debug("prev");
        this->dispatchChanged();
    }

    void onFolder(CCObject* sender) {
        file::pickFile(
            file::PickMode::OpenFile,
            {
                dirs::getGameDir()
            },
            [&](auto path) {
                m_currentCustomClick = path.string();
                inputNode2->setString(path.string().c_str());
            }
        );
        this->dispatchChanged();
    }

public:
    void commit() override {
        static_cast<ClickTypeValue*>(m_value)->setTab(m_currentPos);
        static_cast<ClickTypeValue*>(m_value)->setClick(m_currentClick);
        static_cast<ClickTypeValue*>(m_value)->setMemeClick(m_currentMemeClick);
        static_cast<ClickTypeValue*>(m_value)->setCustomClick(m_currentCustomClick);
        this->dispatchCommitted();
    }
    bool hasUncommittedChanges() override {
        return m_currentPos != static_cast<ClickTypeValue*>(m_value)->getTab() || m_currentClick != static_cast<ClickTypeValue*>(m_value)->getClick() || m_currentMemeClick != static_cast<ClickTypeValue*>(m_value)->getMemeClick() || m_currentCustomClick != static_cast<ClickTypeValue*>(m_value)->getCustomClick();
    }
    bool hasNonDefaultValue() override {
        return m_currentPos != 1 || m_currentClick != 1 || m_currentMemeClick != 1 || m_currentCustomClick != "...";
    }

    // Geode calls this to reset the setting's value back to default
    void resetToDefault() override {
        usefulBtn->toggle(false);
        memeBtn->toggle(true);
        customBtn->toggle(true);
        m_currentPos = 1;
        m_currentClick = 1;
        m_currentMemeClick = 1;
        m_currentCustomClick = "...";
        static_cast<ClickTypeValue*>(m_value)->setTab(m_currentPos);
        static_cast<ClickTypeValue*>(m_value)->setClick(m_currentClick);
        static_cast<ClickTypeValue*>(m_value)->setMemeClick(m_currentMemeClick);
        static_cast<ClickTypeValue*>(m_value)->setCustomClick(m_currentCustomClick);
    }
    template <typename T>
    static ClickTypeNode* create(T* value, float width, std::string prefixText = "?") {
        auto ret = new ClickTypeNode;
        if (ret && ret->init(value, width, prefixText)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
