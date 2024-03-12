#pragma once
#include <Geode/loader/SettingNode.hpp>
using namespace geode::prelude;
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
