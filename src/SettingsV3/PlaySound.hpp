#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include "../StaticClasses.hpp"

using namespace geode::prelude;

class MyButtonSettingV3 : public SettingV3 {
public:
    bool clicksound = false;
    static Result<std::shared_ptr<SettingV3>> parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
        auto res = std::make_shared<MyButtonSettingV3>();
        auto root = checkJson(json, "ClickSoundEffect");
        res->init(key, modID, root);
        root.has("clicksound").into(res->clicksound);
        res->parseNameAndDescription(root);
        res->parseEnableIf(root);
        root.checkUnknownKeys();
        return root.ok(std::static_pointer_cast<SettingV3>(res));
    }

    bool load(matjson::Value const& json) override {
        return true;
    }
    bool save(matjson::Value& json) const override {
        return true;
    }

    bool isDefaultValue() const override {
        return true;
    }

    void reset() override {}

    SettingNodeV3* createNode(float width) override;
};

class MyButtonSettingNodeV3 : public SettingNodeV3 {
protected:
    CCSprite* m_buttonSprite;
    CCMenuItemSpriteExtra* m_button;
    std::shared_ptr<MyButtonSettingV3> m_setting;

    bool init(std::shared_ptr<MyButtonSettingV3> setting, float width) {
        if (!SettingNodeV3::init(setting, width))
            return false;
        
        m_buttonSprite = CCSprite::createWithSpriteFrameName("GJ_playMusicBtn_001.png");
        m_buttonSprite->setScale(.68f);
        m_button = CCMenuItemSpriteExtra::create(
            m_buttonSprite, this, menu_selector(MyButtonSettingNodeV3::onButton)
        );
        m_setting = setting;
        this->getButtonMenu()->addChildAtPosition(m_button, Anchor::Center);
        this->getButtonMenu()->setContentWidth(30);
        this->getButtonMenu()->updateLayout();

        this->updateState(nullptr);
        
        return true;
    }
    
    void updateState(CCNode* invoker) override {
        SettingNodeV3::updateState(invoker);

        auto shouldEnable = this->getSetting()->shouldEnable();
        m_button->setEnabled(shouldEnable);
        m_buttonSprite->setCascadeColorEnabled(true);
        m_buttonSprite->setCascadeOpacityEnabled(true);
        m_buttonSprite->setOpacity(shouldEnable ? 255 : 155);
        m_buttonSprite->setColor(shouldEnable ? ccWHITE : ccGRAY);
    }
    void onButton(CCObject*) {
        if (m_setting->clicksound) {
            if (Custom_OnClick) {
                ClickSound->Play();
            } else {
                ClickSoundIndex->PlayRandom();
            }
        } else {
        if (Custom_OnLetGo) {
            ReleaseSound->Play();
        } else {
            ReleaseSoundIndex->PlayRandom();
        }
        }
    }

    void onCommit() override {}
    void onResetToDefault() override {}

public:
    static MyButtonSettingNodeV3* create(std::shared_ptr<MyButtonSettingV3> setting, float width) {
        auto ret = new MyButtonSettingNodeV3();
        if (ret && ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool hasUncommittedChanges() const override {
        return false;
    }
    bool hasNonDefaultValue() const override {
        return false;
    }

    std::shared_ptr<MyButtonSettingV3> getSetting() const {
        return std::static_pointer_cast<MyButtonSettingV3>(SettingNodeV3::getSetting());
    }
};

SettingNodeV3* MyButtonSettingV3::createNode(float width) {
    return MyButtonSettingNodeV3::create(
        std::static_pointer_cast<MyButtonSettingV3>(shared_from_this()),
        width
    );
}

$execute {
   auto ret = Mod::get()->registerCustomSettingType("clicksoundeffect", &MyButtonSettingV3::parse);
    if (!ret) {
        log::error("Unable to register setting type: {}", ret.unwrapErr());
    }
}