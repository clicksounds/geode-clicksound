// THIS IS UNUSED I DUNNO HOW TO MATJSON :Skull:
#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

enum class MyCustomEnum {
    Meme,
    Useful,
    Custom
};

template <>
struct matjson::Serialize<MyCustomEnum> {
    static matjson::Value toJson(MyCustomEnum const& value) {
        switch (value) {
            default:
            case MyCustomEnum::Meme: return "Meme";
            case MyCustomEnum::Useful: return "Useful";
            case MyCustomEnum::Custom: return "Custom";
        }
        
    }
    static Result<MyCustomEnum> fromJson(matjson::Value const& value) {
        GEODE_UNWRAP_INTO(auto str, value.asString());
        switch (hash(str)) {
            case hash("Meme"): return Ok(MyCustomEnum::Meme);
            case hash("Useful"): return Ok(MyCustomEnum::Useful);
            case hash("Custom"): return Ok(MyCustomEnum::Custom);
            default: return Ok(MyCustomEnum::Useful);
        }
    }
};

class MyCustomSettingV3 : public SettingBaseValueV3<MyCustomEnum> {
public:
    static Result<std::shared_ptr<SettingV3>> parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
        auto res = std::make_shared<MyCustomSettingV3>();
        auto root = checkJson(json, "selectionclicks");
        res->parseBaseProperties(key, modID, root);
        root.checkUnknownKeys();
        return root.ok(std::static_pointer_cast<SettingV3>(res));
    }
    
    SettingNodeV3* createNode(float width) override;
};

template <>
struct geode::SettingTypeForValueType<MyCustomEnum> {
    using SettingType = MyCustomSettingV3;
};

class MyCustomSettingNodeV3 : public SettingValueNodeV3<MyCustomSettingV3> {
protected:
    std::vector<CCMenuItemToggler*> m_toggles;

    bool init(std::shared_ptr<MyCustomSettingV3> setting, float width) {
        if (!SettingValueNodeV3::init(setting, width))
            return false;
        
        int count = 0;
        for (auto value : {
            std::make_pair(MyCustomEnum::Meme, "Meme"),
            std::make_pair(MyCustomEnum::Useful, "Useful"),
            std::make_pair(MyCustomEnum::Custom, "Custom")
        }) {
            count+=40;
            auto offSpr = ButtonSprite::create(value.second, 40.f, true, "bigFont.fnt", "GJ_button_04.png", 20.f, 1.0f);
            offSpr->setOpacity(90);
            auto onSpr = ButtonSprite::create(value.second, 40.f, true, "bigFont.fnt", "GJ_button_01.png", 20.f, 1.0f);
            auto toggle = CCMenuItemToggler::create(
                offSpr, onSpr, this, menu_selector(MyCustomSettingNodeV3::onToggle)
            );
            toggle->m_notClickable = true;
            toggle->setTag(static_cast<int>(value.first));
            m_toggles.push_back(toggle);
            this->getButtonMenu()->addChild(toggle);
        }
        this->getButtonMenu()->setContentWidth(count);
        this->getButtonMenu()->setLayout(RowLayout::create());

        this->updateState(nullptr);
        
        return true;
    }
    
    void updateState(CCNode* invoker) override {
        SettingValueNodeV3::updateState(invoker);
        auto shouldEnable = this->getSetting()->shouldEnable();
        for (auto toggle : m_toggles) {
            toggle->toggle(toggle->getTag() == static_cast<int>(this->getValue()));
            toggle->setEnabled(shouldEnable);
            auto children = toggle->getChildren();
            for (auto children : CCArrayExt<CCNode*>(children)) {
                children->setScale(1);
            }
        }
    }
    void onToggle(CCObject* sender) {
        this->setValue(static_cast<MyCustomEnum>(sender->getTag()), static_cast<CCNode*>(sender));
    }

public:
    static MyCustomSettingNodeV3* create(std::shared_ptr<MyCustomSettingV3> setting, float width) {
        auto ret = new MyCustomSettingNodeV3();
        if (ret && ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

SettingNodeV3* MyCustomSettingV3::createNode(float width) {
    return MyCustomSettingNodeV3::create(std::static_pointer_cast<MyCustomSettingV3>(shared_from_this()), width);
}

$execute {
    auto ret = Mod::get()->registerCustomSettingType("selectionclicks", &MyCustomSettingV3::parse);
    if (!ret) {
        log::error("Unable to register setting type: {}", ret.unwrapErr());
    }
}