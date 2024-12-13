#pragma once
#include <Geode/Geode.hpp>
#include <matjson.hpp>
#include "../SettingsV3/SelectionEnum.hpp"
using namespace geode::prelude;

static struct ReadingSettings
{
    int M_Tab;
    std::string Current_Sound_Useful;
    std::string Current_Sound_Meme;
    std::string Custom_Sound_Path;
    std::string SettingName;
};


static ReadingSettings GetSettingJsonRead(std::string name) {
    ClicksoundSettingValue x = Mod::get()->getSettingValue<ClicksoundSettingValue>(name);
    ReadingSettings Setup;
    Setup.M_Tab = x.m_tab; 
    Setup.Current_Sound_Useful = x.m_currentClick;
    Setup.Current_Sound_Meme = x.m_currentMemeClick;
    Setup.Custom_Sound_Path = x.CustomSoundPath;
    Setup.SettingName = name;
    return Setup;
}