#pragma once
#include "jsonReader/Getsettingsinfo.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

static FMOD::ChannelGroup* SFX_Group = nullptr;

struct downloadedzipStruc {
    bool Finished = false;
    bool Failed = false;
    bool StartedDownloading = false;
};

static downloadedzipStruc indexzip;

static bool Custom_OnClick = false;
static bool Custom_OnLetGo = false;

class SoundCache {
public:
    std::string m_soundFile = "";
    FMOD::Sound* m_sound = nullptr;
    std::string Volume;
    std::string custom;

    SoundCache(std::string vol, std::string cust)
        : Volume(vol), custom(cust) {
        if (!SFX_Group) {
            FMODAudioEngine::sharedEngine()->m_system->createChannelGroup("SFX_Group", &SFX_Group);
            // attach to master, but never to music
            FMOD::ChannelGroup* master = nullptr;
            FMODAudioEngine::sharedEngine()->m_system->getMasterChannelGroup(&master);
            if (master) {
                master->addGroup(SFX_Group);
            }
        }
    }

    void Setsound(const std::string& soundFile) {
        if (!soundFile.empty()) {
            if (FMODAudioEngine::sharedEngine()->m_system->createSound(
                soundFile.c_str(), FMOD_DEFAULT, nullptr, &m_sound) == FMOD_OK) {
                m_sound->setMode(FMOD_LOOP_OFF);
                m_soundFile = soundFile;
            }
        }
    }

    void Play(bool TestButton = false) {
        std::string Paths = GetSettingJsonRead(custom).Custom_Sound_Path;
        if (m_soundFile != Paths) {
            Setsound(Paths);
        }
        if (!m_sound) return;
        PlayModded(TestButton);
    }


	void PlayModded(bool TestButton = false) {
		float GetVolume = Mod::get()->getSettingValue<int64_t>(Volume);
		if (GetVolume <= 0 && TestButton) GetVolume = 1;
		if (!m_sound) return;

		FMOD::Channel* channel = nullptr;

		const int MAX_SFX = 100;
		int active = 0;
		if (SFX_Group) SFX_Group->getNumChannels(&active);
		if (active >= MAX_SFX) {
			int idx = rand() % active;
			FMOD::Channel* old = nullptr;
			SFX_Group->getChannel(idx, &old);
			if (old) old->stop();
			active--;
		}

		FMODAudioEngine::sharedEngine()->m_system->playSound(m_sound, SFX_Group, false, &channel);
		if (!channel) return;

		float scale = 1.0f / std::sqrt(static_cast<float>(active+1)); 
		float randScale = 0.85f + static_cast<float>(rand()) / RAND_MAX * 0.3f;
		channel->setVolume((GetVolume / 50.f) * scale * randScale);

		FMOD::DSP* localPitchDSP = nullptr;
		FMODAudioEngine::sharedEngine()->m_system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &localPitchDSP);

		double semitone = static_cast<double>(Mod::get()->getSettingValue<int64_t>("sfx-semitone")) / 12.0;
		if (semitone < 0) semitone = std::pow(2.0, semitone);
		else semitone += 1.0;

		if (localPitchDSP) {
			localPitchDSP->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, static_cast<float>(semitone));
			channel->addDSP(0, localPitchDSP);
			localPitchDSP->release();
		}
	}



    ~SoundCache() {
        if (m_sound) {
            m_sound->release();
        }
    }
};

class MultiSoundCache {
public:
    std::vector<SoundCache*> m_sounds;

    MultiSoundCache() {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    void SetSounds(const std::vector<std::string>& soundFiles, const std::string& volume, const std::string& custom) {
        for (auto& sound : m_sounds) {
            delete sound;
        }
        m_sounds.clear();
        for (const auto& soundFile : soundFiles) {
            SoundCache* sound = new SoundCache(volume, custom);
            sound->Setsound(soundFile);
            m_sounds.push_back(sound);
        }
    }

    void PlayRandom() {
        if (!m_sounds.empty()) {
            int randomIndex = std::rand() % m_sounds.size();
            m_sounds[randomIndex]->PlayModded();
        }
    }

    ~MultiSoundCache() {
        for (auto& sound : m_sounds) {
            delete sound;
        }
    }
};

static MultiSoundCache* ClickSoundIndex = new MultiSoundCache();
static MultiSoundCache* ReleaseSoundIndex = new MultiSoundCache();
static SoundCache* ClickSound = new SoundCache("click-volume", "selection-clicks");
static SoundCache* ReleaseSound = new SoundCache("release-volume", "selection-release");
