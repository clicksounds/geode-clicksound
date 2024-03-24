#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/SettingNode.hpp>
using namespace geode::prelude;

class ClickTypeValue : public SettingValue {
protected:
    matjson::Value m_type;
public:
    ClickTypeValue(std::string const& key, std::string const& modID, int type)
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
};

class ReleaseTypeValue : public SettingValue {
protected:
    matjson::Value m_type;
public:
    ReleaseTypeValue(std::string const& key, std::string const& modID, int type)
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
};