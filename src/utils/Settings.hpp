#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/SettingNode.hpp>
#include "../utils/AnotherSettings.hpp"
using namespace geode::prelude;


CCNode* createCheckboxButtonTexture(std::string text, bool isSelect){
    auto label = CCLabelBMFont::create(text.c_str(), "chatFont.fnt");
    if (isSelect){label->setString(fmt::format("{} + on", text).c_str());}
    label->setScale(.33F);
    label->setPositionX(0);
    return static_cast<CCNode*>(label);
}


// TYSM JOUCA AND FIREE

// CLICK SOUNDS
class SettingClickValue;

class SettingClickValue : public SettingValue {
protected:
    std::string m_placeholder;
public:
    SettingClickValue(std::string const& key, std::string const& modID, std::string const& placeholder)
      : SettingValue(key, modID), m_placeholder(placeholder) {}

    bool load(matjson::Value const& json) override {
        return true;
    }
    bool save(matjson::Value& json) const override {
        return true;
    }
    SettingNode* createNode(float width) override;
};


class SettingClickNode : public SettingNode {
protected:
 bool init(SettingClickValue* value, float width) {
        if (!SettingNode::init(value))
            return false;
        this->setContentSize({ width, 35.f });
        auto menu = CCMenu::create();
        auto label = CCLabelBMFont::create("Preview Click Sound", "bigFont.fnt");
        label->setScale(.33F);
        label->setPositionX(-93);
        auto playSpr = CCSprite::createWithSpriteFrameName("GJ_playMusicBtn_001.png");
        playSpr->setScale(.650F);
        //auto infoBtn
        auto testBtn = CCMenuItemSpriteExtra::create(
            playSpr,
            this,
            menu_selector(SettingClickNode::onClickBtn)
        );
        testBtn->setPositionX(143);
        menu->setPosition(width / 2, 18.f);
        menu->addChild(label);
        menu->addChild(testBtn);
        this->addChild(menu);
        return true;
    }

public:
    // because for some reason SettingNode doesnt expose description, i have to do all of this, smh.
    void onClickBtn(CCObject*);

    void commit() override {
        // Let the UI know you have committed the value
        this->dispatchCommitted();
    }

    // Geode calls this to query if the setting value has been changed, 
    // and those changes haven't been committed
    bool hasUncommittedChanges() override {
        return false;
    }

    // Geode calls this to query if the setting has a value that is 
    // different from its default value
    bool hasNonDefaultValue() override {
        return true;
    }

    // Geode calls this to reset the setting's value back to default
    void resetToDefault() override {

    }
    static SettingClickNode* create(SettingClickValue* value, float width) {
        auto ret = new SettingClickNode;
        if (ret && ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

class SettingReleaseValue;

class SettingReleaseValue : public SettingValue {
protected:
    std::string m_placeholder;
public:
    SettingReleaseValue(std::string const& key, std::string const& modID, std::string const& placeholder)
      : SettingValue(key, modID), m_placeholder(placeholder) {}

    bool load(matjson::Value const& json) override {
        return true;
    }
    bool save(matjson::Value& json) const override {
        return true;
    }
    SettingNode* createNode(float width) override;
};

// RELEASE SOUNDS
class SettingReleaseNode : public SettingNode {
protected:
 bool init(SettingReleaseValue* value, float width) {
        if (!SettingNode::init(value))
            return false;
        this->setContentSize({ width, 35.f });
        auto menu = CCMenu::create();
        auto label = CCLabelBMFont::create("Preview Release Sound", "bigFont.fnt");
        label->setScale(.3F);
        label->setPositionX(-93);
        auto playSpr = CCSprite::createWithSpriteFrameName("GJ_playMusicBtn_001.png");
        playSpr->setScale(.650F);
        //auto infoBtn
        auto testBtn = CCMenuItemSpriteExtra::create(
            playSpr,
            this,
            menu_selector(SettingReleaseNode::onReleaseBtn)
        );
        testBtn->setPositionX(143);
        menu->setPosition(width / 2, 18.f);
        menu->addChild(label);
        menu->addChild(testBtn);
        this->addChild(menu);
        return true;
    }

public:
    // because for some reason SettingNode doesnt expose description, i have to do all of this, smh.
    void onReleaseBtn(CCObject*);

    void commit() override {
        // Let the UI know you have committed the value
        this->dispatchCommitted();
    }

    // Geode calls this to query if the setting value has been changed, 
    // and those changes haven't been committed
    bool hasUncommittedChanges() override {
        return false;
    }

    // Geode calls this to query if the setting has a value that is 
    // different from its default value
    bool hasNonDefaultValue() override {
        return true;
    }

    // Geode calls this to reset the setting's value back to default
    void resetToDefault() override {

    }
    static SettingReleaseNode* create(SettingReleaseValue* value, float width) {
        auto ret = new SettingReleaseNode;
        if (ret && ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// SECTIONS
// Copied from Coop's new Hide+ mod (get it now!)

class SectionSettingValue;

class SectionSettingValue : public SettingValue {
protected:
    std::string m_placeholder;
public:
    // lines 5, 8, 11, and 12 are copied from GDUtils
    SectionSettingValue(std::string const& key, std::string const& modID, std::string const& placeholder)
      : SettingValue(key, modID), m_placeholder(placeholder) {}
    bool load(matjson::Value const& json) override {return true;}
    bool save(matjson::Value& json) const override {return true;}
    SettingNode* createNode(float width) override;
};

class SectionSettingNode : public SettingNode {
protected:
    bool init(SectionSettingValue* value, float width) {
        if (!SettingNode::init(value))
            return false;
        this->setContentSize({ width, 40.f });
        std::string name = Mod::get()->getSettingDefinition(value->getKey())->get<CustomSetting>()->json->get<std::string>("name");

        auto theMenu = CCMenu::create();
        auto theLabel = CCLabelBMFont::create(name.c_str(),"bigFont.fnt");

        theLabel->setScale(.5);
        theLabel->setPositionX(0);
        theMenu->addChild(theLabel);
        theMenu->setPosition(width / 2, 20.f);
        
        this->addChild(theMenu);



        return true;
    }

public:
    void commit() override {
        this->dispatchCommitted();
    }
    bool hasUncommittedChanges() override {
        return false;
    }
    bool hasNonDefaultValue() override {
        return true;
    }
    void resetToDefault() override {}

    static SectionSettingNode* create(SectionSettingValue* value, float width) {
        auto ret = new SectionSettingNode();
        if (ret && ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};




// STOLE FROM VIPER'S BETTER MENU
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
    bool init(T* value, float width, std::string prefixText = "Unknown [Mod Developer didn't put anything here]") {
        if (!SettingNode::init(value))
            return false;

       
        m_currentPos = value->getType();
        this->setContentSize({ width, 70.f });
        auto menu = CCMenu::create();
        CCSprite* toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
        CCSprite* toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
        toggleOn->setScale(.7F);
        toggleOff->setScale(.7F);
        menu->setPosition(290, 23.f);
        usefulBtn = CCMenuItemToggler::create(
            toggleOn,
            toggleOff,
            this,
            menu_selector(ClickTypeNode::onCornerClick)
        );
        memeBtn = CCMenuItemToggler::create(
            toggleOn,
            toggleOff,
            this,
            menu_selector(ClickTypeNode::onCornerClick)
        );
        customBtn = CCMenuItemToggler::create(
            toggleOn,
            toggleOff,
            this,
            menu_selector(ClickTypeNode::onCornerClick)
        );
        usefulBtn->setPosition({ -32, 15 });
        memeBtn->setPosition({ 32, 15 });
        customBtn->setPosition({ 0, 35 });

        usefulBtn->setTag(getActiveCornerTag(1));
        memeBtn->setTag(getActiveCornerTag(2));
        customBtn->setTag(getActiveCornerTag(3));
        int currentCorner = m_currentPos;
        usefulBtn->toggle(!(usefulBtn->getTag() == getActiveCornerTag(currentCorner)));
        memeBtn->toggle(!(memeBtn->getTag() == getActiveCornerTag(currentCorner)));
        customBtn->toggle(!(customBtn->getTag() == getActiveCornerTag(currentCorner)));
        
        menu->addChild(usefulBtn);
        menu->addChild(memeBtn);
        menu->addChild(customBtn);
        menu->setPositionX(293);
        this->addChild(menu);
        auto label = CCLabelBMFont::create(fmt::format("{} Type",prefixText).c_str(), "bigFont.fnt");
        label->setScale(0.750);
        label->setPositionX(94);
        label->setPositionY(36);
        this->addChild(label);
        //auto infoBtn
        
        
        label->setScale(.6F);
        this->addChild(label);
        this->addChild(menu);
        return true;
    }
    void onCornerClick(CCObject* sender) {
        usefulBtn->toggle(true);
        memeBtn->toggle(true);
        customBtn->toggle(true);
        m_currentPos = tagToCorner(sender->getTag());
        this->dispatchChanged();
    };

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
