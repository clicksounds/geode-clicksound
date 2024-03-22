#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/SettingNode.hpp>
using namespace geode::prelude;

class ClickTypeValue : public SettingValue {
protected:
    int m_type;
public:
    ClickTypeValue(std::string const& key, std::string const& modID, int type)
      : SettingValue(key, modID), m_type(type) {}

    bool load(matjson::Value const& json) override {
        if (!json.is<int>()) return false;
        m_type = json.as<int>();
        return true;
    }

    bool save(matjson::Value& json) const override {
        json = m_type;
        return true;
    }

    SettingNode* createNode(float width) override;

    void setType(int type) {
        m_type = type;
    }

    int getType() const {
        return m_type;
    }
};

class ReleaseTypeValue : public SettingValue {
protected:
    int m_type;
public:
    ReleaseTypeValue(std::string const& key, std::string const& modID, int type)
      : SettingValue(key, modID), m_type(type) {}

    bool load(matjson::Value const& json) override {
        if (!json.is<int>()) return false;
        m_type = json.as<int>();
        return true;
    }

    bool save(matjson::Value& json) const override {
        json = m_type;
        return true;
    }

    SettingNode* createNode(float width) override;

    void setType(int type) {
        m_type = type;
    }

    int getType() const {
        return m_type;
    }
};