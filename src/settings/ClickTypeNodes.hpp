#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/SettingNode.hpp>
#include "ClickTypes.hpp"
using namespace geode::prelude;

CCNode* createCheckboxButtonTexture(std::string text, float width, bool isSelect){
    auto label = CCLabelBMFont::create(text.c_str(), "chatFont.fnt");
    label->setScale(.33F);
    label->setPosition(28.833f, 16);
    auto bgSelector = cocos2d::extension::CCScale9Sprite::create(
            "square02b_small.png", { 0.0f, 0.0f, 40.0f, 40.0f }
        );
    bgSelector->setColor({ 255, 255, 255 });
    if (isSelect) {bgSelector->setOpacity(190);} else {bgSelector->setOpacity(0);}
    bgSelector->setContentSize({ 52.667f, 27.f });
    bgSelector->setPosition(28.833f, 16);
    auto layer = CCLayer::create();
    layer->ignoreAnchorPointForPosition(false);
    layer->addChild(label);
    layer->addChild(bgSelector);
    layer->setContentSize({ 57.667f, 32.f });
    layer->setPositionY(0);
    return static_cast<CCNode*>(layer);
}


// STOLE FROM VIPER'S BETTER MENU BUT CUSTOMIZED BY COOPER 
class ClickTypeNode : public SettingNode {
protected:
    int m_currentPos;
    CCMenuItemToggler* usefulBtn;
    CCMenuItemToggler* memeBtn;
    CCMenuItemToggler* customBtn;

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

       
        m_currentPos = value->getType();
        this->setContentSize({ width, 80.f });

        auto menu = CCMenu::create();
        menu->setContentSize({ width/2, 20.f} );
        menu->setPosition(width*3/4 - 7.f, 60.f);
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

        auto label = CCLabelBMFont::create(fmt::format("{} Type",prefixText).c_str(), "bigFont.fnt");
        label->setScale(.5F);
        label->setAnchorPoint({ 0.f, 0.5f });
        label->setPosition(20.f, 60.f);

        this->addChild(label);
        menu->updateLayout();

        auto menu1 = CCMenu::create();
        menu1->setPosition({ width - 20.f, 20.f });
        menu1->setVisible(false);
        menu1->setTag(5001);
        this->addChild(menu1);

        auto anotherLabel = CCLabelBMFont::create(fmt::format("{} Sound",prefixText).c_str(), "bigFont.fnt");
        anotherLabel->setScale(.5F);
        anotherLabel->setAnchorPoint({ 0.f, 0.5f });
        anotherLabel->setPosition(20.f, 20.f);
        this->addChild(anotherLabel);

        auto inputNode1 = InputNode::create(103.f, "...", "chatFont.fnt");
        inputNode1->setScale(0.65f);    
        inputNode1->setPosition(-51.5f, 0);
        inputNode1->setString("...");
        inputNode1->getInput()->setAllowedChars("");
        menu1->addChild(inputNode1);

        auto arrowRight = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
        arrowRight->setScale(0.3f);
        auto buttonRight = CCMenuItemSpriteExtra::create(arrowRight, this, menu_selector(ClickTypeNode::onNext));
        buttonRight->setPosition(-10.f, 0);
        menu1->addChild(buttonRight);

        auto arrowLeft = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
        arrowLeft->setScale(0.3f);
        arrowLeft->setFlipX(true);
        auto buttonLeft = CCMenuItemSpriteExtra::create(arrowLeft, this, menu_selector(ClickTypeNode::onPrev));
        buttonLeft->setPosition(-93.f, 0);
        menu1->addChild(buttonLeft);

        auto menu2 = CCMenu::create();
        menu2->setPosition({ width - 20.f, 20.f });
        menu2->setVisible(false);
        menu2->setTag(5002);
        this->addChild(menu2);

        auto inputNode2 = InputNode::create(143.f, "...", "chatFont.fnt");
        inputNode2->setScale(0.65f);
        inputNode2->setPosition(-61.5f, 0);
        inputNode2->setString("...");
        inputNode2->getInput()->setAllowedChars("");
        menu2->addChild(inputNode2);

        auto folderSpr = CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png");
        folderSpr->setScale(0.5f);
        auto folderBtn = CCMenuItemSpriteExtra::create(
            folderSpr,
            this,
            menu_selector(ClickTypeNode::onFolder)
        );
        menu2->addChild(folderBtn);


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
                log::error("{}", path.string());
            }
        );
        this->dispatchChanged();
    }

public:
    void commit() override {
        static_cast<ClickTypeValue*>(m_value)->setType(m_currentPos);
        this->dispatchCommitted();
    }
    bool hasUncommittedChanges() override {
        return m_currentPos != static_cast<ClickTypeValue*>(m_value)->getType();
    }
    bool hasNonDefaultValue() override {
        return m_currentPos != 1;
    }

    // Geode calls this to reset the setting's value back to default
    void resetToDefault() override {
        usefulBtn->toggle(false);
        memeBtn->toggle(true);
        customBtn->toggle(true);
        m_currentPos = 1;
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
