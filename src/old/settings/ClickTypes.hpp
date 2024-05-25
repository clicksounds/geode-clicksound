#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/SettingNode.hpp>
#include "../utils/Clicks.hpp"
#include <matjson.hpp>
#include <string>
using namespace geode::prelude;

struct CRTypeStruct {
    matjson::Value m_type;
};

class ClickTypeValue : public SettingValue {
protected:
    matjson::Value m_type;
public:
    ClickTypeValue(std::string const& key, std::string const& modID, matjson::Value const& type)
      : SettingValue(key, modID), m_type(type) {}

    bool load(matjson::Value const& json) override {
        if (!json["tab"].is<int>()) return false;
        m_type = json;
        return true;
    }

    bool save(matjson::Value& json) const override {
        json = m_type;
        return true;
    }

    SettingNode* createNode(float width) override;

    void setTab(int type) {
        m_type["tab"] = type;
    }

    int getTab() const {
        return m_type["tab"].as<int>();
    }

    void setClick(int type) {
        m_type["click"] = type;
    }

    int getClick() const {
        return m_type["click"].as<int>();
    }

    void setMemeClick(int type) {
        m_type["memeClick"] = type;
    }

    int getMemeClick() const {
        return m_type["memeClick"].as<int>();
    }

    void setCustomClick(std::string type) {
        m_type["customClick"] = type;
    }

    std::string getCustomClick() const {
        return m_type["customClick"].as<std::string>();
    }

    matjson::Value getJson() const {
        return m_type;
    }
};

class ReleaseTypeValue : public SettingValue {
protected:
    matjson::Value m_type;
public:
    ReleaseTypeValue(std::string const& key, std::string const& modID, matjson::Value const& type)
      : SettingValue(key, modID), m_type(type) {}

    bool load(matjson::Value const& json) override {
        if (!json["tab"].is<int>() || !json["click"].is<int>() || !json["memeClick"].is<int>() || !json["customClick"].is<std::string>()) {return false;}
        m_type = json;
        return true;
    }

    bool save(matjson::Value& json) const override {
        json = m_type;
        return true;
    }

    SettingNode* createNode(float width) override;

    void setTab(int type) {
        m_type["tab"] = type;
    }

    int getTab() const {
        return m_type["tab"].as<int>();
    }
    
    void setClick(int type) {
        m_type["click"] = type;
    }

    int getClick() const {
        return m_type["click"].as<int>();
    }

    void setMemeClick(int type) {
        m_type["memeClick"] = type;
    }

    int getMemeClick() const {
        return m_type["memeClick"].as<int>();
    }

    void setCustomClick(std::string type) {
        m_type["customClick"] = type;
    }

    std::string getCustomClick() const {
        return m_type["customClick"].as<std::string>();
    }

    matjson::Value getJson() const {
        return m_type;
    }
};

template<>
struct SettingValueSetter<CRTypeStruct> {
    static CRTypeStruct get(SettingValue* setting) {
        auto CRSetting = static_cast<ClickTypeValue*>(setting);
        struct CRTypeStruct defaultStruct = { CRSetting->getJson() };
        return defaultStruct;
    };
};