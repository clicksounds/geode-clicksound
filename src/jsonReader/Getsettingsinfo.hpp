#pragma once
#include <Geode/Geode.hpp>
#include <matjson.hpp>
#include "../SettingsV3/SelectionEnum.hpp"
#include "../StaticClasses.hpp"
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

static void onsettingsUpdate() {
    auto selection_release = GetSettingJsonRead("selection-release");
    ReleaseSound->Setsound(selection_release.Custom_Sound_Path);

    auto selection_clicks = GetSettingJsonRead("selection-clicks");
    ClickSound->Setsound(selection_clicks.Custom_Sound_Path);
    if (ClickJson->hassomedata) {

        Custom_OnClick = selection_clicks.M_Tab == 2;
        if (selection_clicks.M_Tab == 0) {
            auto list = ClickJson->GetMemeClicks();
            auto sound = list.find(selection_clicks.Current_Sound_Meme);
            if (sound != list.end()) {
                ClickSoundIndex->SetSounds(sound->second.clicks, "click-volume","selection-clicks");
            }
        } else {
            auto list = ClickJson->GetUsefulClicks();
            auto sound = list.find(selection_clicks.Current_Sound_Useful);
            if (sound != list.end()) {
                ClickSoundIndex->SetSounds(sound->second.clicks, "click-volume","selection-clicks");
            }
        }

        Custom_OnLetGo = selection_release.M_Tab == 2;
         if (selection_release.M_Tab == 0) {
            auto list = ClickJson->GetMemeReleases();
            auto sound = list.find(selection_release.Current_Sound_Meme);
            if (sound != list.end()) {
                ReleaseSoundIndex->SetSounds(sound->second.releases, "release-volume","selection-release");
            }
        } else {
            auto list = ClickJson->GetUsefulReleases();
            auto sound = list.find(selection_release.Current_Sound_Useful);
            if (sound != list.end()) {
                ReleaseSoundIndex->SetSounds(sound->second.releases, "release-volume","selection-release");
            }
        }

    }
}