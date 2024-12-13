#pragma once
#include <Geode/Geode.hpp>
#include "jsonReader/Getsettingsinfo.hpp"

using namespace geode::prelude;
// Custom class for Caching sounds (Make it less laggy for mobile platforms and such)
class SoundCache {
    public:
        std::string m_soundFile = "";
        FMOD::Sound* m_sound;
        std::string Volume;
        std::string custom;
        FMOD::Channel* Soundchannel;
        
        SoundCache(std::string x,std::string x2) {
            Volume = x;
            custom = x2;
        };
        void Setsound(std::string soundFile) {
                if (soundFile.c_str()) {
                    if (FMODAudioEngine::sharedEngine()->m_system->createSound(soundFile.c_str(), FMOD_DEFAULT, nullptr, &m_sound) == FMOD_OK) {
                       m_sound->setMode(FMOD_LOOP_OFF);
                       m_soundFile = soundFile;
                    }
                } 
        }
        void Play() { 
            std::string  Paths =  GetSettingJsonRead(custom).Custom_Sound_Path;
            if (m_soundFile != Paths) {
                Setsound(Paths);
            }
            if (!m_sound) {
                return;
            }
            float GetVolume = Mod::get()->getSettingValue<int64_t>(Volume);
            if (GetVolume <= 0) {
                GetVolume = 1;
            }
            FMODAudioEngine::sharedEngine()->m_system->playSound(m_sound, nullptr, false, &Soundchannel);
            Soundchannel->setVolume(GetVolume / 50.f);
        };

        ~SoundCache() {
            if (m_sound) {
                m_sound->release();
            }
        };

};



// Create the classes for Caching
static SoundCache* ClickSound = new SoundCache("click-volume","selection-clicks");
static SoundCache* ReleaseSound = new SoundCache("release-volume","selection-release");