#pragma once
#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

static struct ClicksoundSettingValue {
    int m_currentMemeClick = 0;
    int m_currentClick = 0;
    int m_tab = 0;
    std::string CustomSoundPath;

    // Comparison operator
    bool operator==(ClicksoundSettingValue const& other) const = default;

    // Implicit conversion to std::string
      operator std::string() const {
       return matjson::makeObject({
            {"Tab", m_tab},
            {"Current_Sound_Useful", m_currentClick},
            {"Current_Sound_Meme", m_currentMemeClick},
            {"Custom_Sound_Path", CustomSoundPath}
        }).dump(matjson::NO_INDENTATION);
    }

    // Constructors
    ClicksoundSettingValue() = default;

    ClicksoundSettingValue(int i1, int i2, int i3, std::string_view val)
        : m_tab(i1), m_currentClick(i2), m_currentMemeClick(i3), CustomSoundPath(val) {}

    ClicksoundSettingValue(ClicksoundSettingValue const&) = default;
};

template <>
struct matjson::Serialize<ClicksoundSettingValue> {

    static Result<ClicksoundSettingValue> fromJson(matjson::Value const& v) {
        GEODE_UNWRAP_INTO(std::string x, v.asString());
        if (x == "") {
           return Ok(ClicksoundSettingValue(0, 0, 0, " ")); 
        }
       try {
        auto value = matjson::parse(x).unwrap();
        return Ok(ClicksoundSettingValue(
            value["Tab"].asInt().unwrap(), 
            value["Current_Sound_Useful"].asInt().unwrap(), 
            value["Current_Sound_Meme"].asInt().unwrap(), 
            value["Custom_Sound_Path"].asString().unwrap()
        ));
        } catch (const std::exception& e) {
            return Ok(ClicksoundSettingValue(0, 0, 0, " "));
        }
    }

    static bool is_json(matjson::Value const& json) {
        return json.isString();
    }
};




class MyCustomSettingV3 : public SettingBaseValueV3<ClicksoundSettingValue> {
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
struct geode::SettingTypeForValueType<ClicksoundSettingValue> {
    using SettingType = MyCustomSettingV3;
};

class MyCustomSettingNodeV3 : public SettingValueNodeV3<MyCustomSettingV3> {
protected:
    std::vector<CCMenuItemToggler*> m_toggles;
    CCMenuItemSpriteExtra* m_folderBtn;
    CCMenu* m_menufolder;
    CCLabelBMFont* m_nameLabel;
    bool init(std::shared_ptr<MyCustomSettingV3> setting, float width) {
        if (!SettingValueNodeV3::init(setting, width))
            return false;
        
        this->setContentSize({ width, 70.f });
        CCSprite* folderSpr = CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png");
        folderSpr->setScale(1.0f);
        m_folderBtn = CCMenuItemSpriteExtra::create(
            folderSpr,
            this,
            menu_selector(MyCustomSettingNodeV3::onFolder)
        );
        m_nameLabel = CCLabelBMFont::create("", "bigFont.fnt");
        this->removeChild(this->getNameMenu(),false);
         this->removeChild(this->getButtonMenu(),false);
        this->getNameMenu()->setLayout(
            RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(AxisAlignment::Start)
        );
        
        this->addChildAtPosition(this->getNameMenu(), Anchor::TopLeft, ccp(10, 0), ccp(0, 1.0f));
        this->addChildAtPosition(this->getButtonMenu(), Anchor::TopRight, ccp(-10, 0), ccp(1.0f, 1.0f));
        this->getStatusLabel()->setPosition(this->getNameMenu()->getPosition() - ccp(0,this->getNameMenu()->getContentHeight() + 2));
        this->getStatusLabel()->setScale(0.175);
        this->getStatusLabel()->setAnchorPoint({0,1});
        this->getButtonMenu()->setScale(1.2);
        this->getNameMenu()->setScale(1.2);
        m_menufolder = CCMenu::create();
        m_menufolder->addChild(m_folderBtn);
        m_menufolder->setScale(1);
        m_menufolder->setLayout(RowLayout::create());
        m_menufolder->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
        this->addChild(m_menufolder);
        m_nameLabel->setPosition(m_menufolder->getPosition() - ccp(0,m_menufolder->getContentSize().height));
        m_nameLabel->setScale(0.5);
        m_nameLabel->setAnchorPoint({0.5,0});
        this->addChild(m_nameLabel);

        
        int count = 0;
        for (auto value : {
            std::make_pair(0, "Meme"),
            std::make_pair(1, "Useful"),
            std::make_pair(2, "Custom")
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
        this->getButtonMenu()->setLayout(RowLayout::create());
        this->getButtonMenu()->updateLayout();
        this->updateState(nullptr);
        
        return true;
    }
    
    void updateState(CCNode* invoker) override {
        SettingValueNodeV3::updateState(invoker);
        float shouldEnable = this->getSetting()->shouldEnable();
        
        m_menufolder->setVisible(static_cast<int>(this->getValue().m_tab) == 2);
        m_nameLabel->setVisible(static_cast<int>(this->getValue().m_tab) == 2);
        std::error_code ec;
        auto Custompa = this->getValue().CustomSoundPath;
        if (Custompa.empty() || Custompa == " " || !std::filesystem::is_regular_file(Custompa, ec)) {
            m_nameLabel->setColor(ccGRAY);
            m_nameLabel->setOpacity(155);
            m_nameLabel->setString("");
        } else {
            std::filesystem::path filePath(Custompa);
            m_nameLabel->setString(filePath.filename().string().c_str());
            m_nameLabel->setColor(ccWHITE);
            m_nameLabel->setOpacity(255);
        }
        m_folderBtn->setEnabled(shouldEnable);
        if (!shouldEnable) {
           m_nameLabel->setColor(ccGRAY);
        }
        for (auto toggle : m_toggles) {
            if (shouldEnable) {
                toggle->toggle(toggle->getTag() == static_cast<int>(this->getValue().m_tab));
                toggle->setVisible(true);
            } else {
              (toggle->toggle(false)); 
              (toggle->setVisible(false));
            }
            toggle->setEnabled(shouldEnable);
            auto children = toggle->getChildren();
            for (auto children : CCArrayExt<CCNode*>(children)) {
                children->setScale(1);
            }
        }
    }
    void onToggle(CCObject* sender) {
        ClicksoundSettingValue Changes = this->getValue();
        Changes.m_tab = sender->getTag();
        this->setValue(Changes, static_cast<CCNode*>(sender));
    }
     void onFolder(CCObject* sender) {
        file::FilePickOptions::Filter textFilter;
        file::FilePickOptions fileOptions;
        textFilter.description = "Sound effect";
        textFilter.files = {"*.ogg", "*.mp3", "*.wav"};
        fileOptions.filters.push_back(textFilter);

        file::pick(file::PickMode::OpenFile, { Mod::get()->getResourcesDir(), { textFilter } }).listen([this,sender](Result<std::filesystem::path>* res) {
            if (res->isOk()) {
                    std::filesystem::path path = res->unwrap();
                    ClicksoundSettingValue Changes = this->getValue();
                    Changes.CustomSoundPath = path.string().c_str();
                    this->setValue(Changes, static_cast<CCNode*>(sender));
                }
            });
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