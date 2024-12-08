#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/loader/SettingV3.hpp>
#include "SettingsV3/PlaySound.hpp"
#include "SettingsV3/SelectionEnum.hpp"
#include "StaticClasses.hpp"

using namespace geode::prelude;


// the check to see if you should play the sound or not
bool integrityCheck(PlayerObject* object, PlayerButton Pressed) {
    // play sounds when "only play on jump" settings is enabled and the player input is a jump, left movement, or right movement.
    if (Mod::get()->getSettingValue<bool>("only-on-jump")) {
        if (Pressed != PlayerButton::Jump) {
            return false;
        }
    }
    GJGameLevel* Level;
     if (!PlayLayer::get()) {
        if (!LevelEditorLayer::get()) {
            return false;
        }
        Level = LevelEditorLayer::get()->m_level;
     } else {
        PlayLayer* Pl = PlayLayer::get();
        // Mac fix
        if (Pl->m_isPaused) {
            return false;
        }
        Level = Pl->m_level;
     };
     if (object->m_isSecondPlayer && Level->m_twoPlayerMode || !object->m_isSecondPlayer) {
        return true;
     } else {
        return false;
     }
}

class $modify(PlayerObject) {
public:
        // add it to fields to access later but stored in the object (m_fields->Var)
    struct Fields {
         bool directionUp = false;
         bool directionRight = false;
         bool directionLeft = false;
    };
    
    // For setting bools for setting dir
    void SetupNewDirections(PlayerButton p0, bool Set) { 
        switch (p0) { 
            case PlayerButton::Jump:   m_fields->directionUp = Set; break; 
            case PlayerButton::Right:  m_fields->directionRight = Set; break; 
            case PlayerButton::Left:  m_fields->directionLeft = Set; break; 
            default:break; 
        } 
    }
    // getting the bools in a orderly way
    bool GetNewDirections(PlayerButton p0) { 
        switch (p0) { 
            case PlayerButton::Jump:   return m_fields->directionUp; break; 
            case PlayerButton::Right:  return m_fields->directionRight; break; 
            case PlayerButton::Left:  return m_fields->directionLeft; break; 
            default:break; 
        } 
        return false;
    }
    // click sounds
    bool pushButton(PlayerButton p0) {
        bool ret = PlayerObject::pushButton(p0);
        // check if you can and or check if it is correct
        if (!integrityCheck(this,p0)) {
            return ret;
        };

        auto clickSoundFile = Mod::get()->getSettingValue<std::filesystem::path>("custom-presssound").string();
        auto isClickEnabled = Mod::get()->getSettingValue<bool>("enable-clicksounds");
        auto click_vol = Mod::get()->getSettingValue<int64_t>("click-volume");
        // set the direction bool to true
        SetupNewDirections(p0,true);
        
        // is it enabled or is volume < 0
        if (click_vol <= 0 || !isClickEnabled) return ret;
        
        // Sound Player
        ClickSound->Play();

        return ret;
    }

    // release sounds
    bool releaseButton(PlayerButton p0) {
        bool ret = PlayerObject::releaseButton(p0);
        // Did you click? check
        if (!GetNewDirections(p0)) {
            return ret;
        };
        // check if you can and or check if it is correct
         if (!integrityCheck(this,p0)) {
            return ret;
        };

        auto releaseSoundFile = Mod::get()->getSettingValue<std::filesystem::path>("custom-releasesound").string();
        auto isReleaseEnabled = Mod::get()->getSettingValue<bool>("enable-releasesounds");
        auto release_vol = Mod::get()->getSettingValue<int64_t>("release-volume");
        // set the direction bool to false
        SetupNewDirections(p0,false);
        // is it enabled or is volume < 0
        if (release_vol <= 0 || !isReleaseEnabled) return ret;

        // Sound Player
        ReleaseSound->Play();

        return ret;
    }
};


// on the mod loading
$execute {
    // Does the release-sound path setting change?
    listenForSettingChanges("custom-releasesound", [](std::filesystem::path releaseSoundFile) {
        ReleaseSound->Setsound(releaseSoundFile.string());
    });
    // Does the click-sound path setting change?
     listenForSettingChanges("custom-presssound", [](std::filesystem::path PressSoundSoundFile) {
        ClickSound->Setsound(PressSoundSoundFile.string());
    });
    // on boot set Sound Caches
    std::string releaseSoundFile = Mod::get()->getSettingValue<std::filesystem::path>("custom-releasesound").string();
    ReleaseSound->Setsound(releaseSoundFile);
    std::string clickSoundFile = Mod::get()->getSettingValue<std::filesystem::path>("custom-presssound").string();
    ClickSound->Setsound(clickSoundFile);
}

