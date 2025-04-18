#pragma once
#include "jsonReader/Getsettingsinfo.hpp"
#include <Geode/Geode.hpp>

static bool Custom_OnClick = false;
static bool Custom_OnLetGo = false;

struct downloadedzipStruc {
	bool Finished = false;
	bool Failed = false;
	bool StartedDownloading = false;
};

static downloadedzipStruc indexzip;
FMOD::ChannelGroup *CS_Group;
FMOD::DSP *pitchShifterDSP;
using namespace geode::prelude;
// Custom class for Caching sounds (Make it less laggy for mobile platforms and such)
class SoundCache {
  public:
	std::string m_soundFile = "";
	FMOD::Sound *m_sound;
	std::string Volume;
	std::string custom;
	FMOD::Channel *Soundchannel;

	SoundCache(std::string x, std::string x2) : Volume(x), custom(x2), Soundchannel(nullptr) {
		if (!CS_Group) {
			FMODAudioEngine::sharedEngine()->m_system->createChannelGroup("CS_Group", &CS_Group);
		}

		if (!pitchShifterDSP) {
			FMODAudioEngine::sharedEngine()->m_system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pitchShifterDSP);
			CS_Group->addDSP(0, pitchShifterDSP);
		}
	}

	void Setsound(std::string soundFile) {
		if (soundFile.c_str()) {
			if (FMODAudioEngine::sharedEngine()->m_system->createSound(soundFile.c_str(), FMOD_DEFAULT, nullptr, &m_sound) == FMOD_OK) {
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
		if (!m_sound) {
			return;
		}
		PlayModded(TestButton);
	}

	void PlayModded(bool TestButton = false) {
		float GetVolume = Mod::get()->getSettingValue<int64_t>(Volume);
		if (GetVolume <= 0 && TestButton == true) {
			GetVolume = 1;
		}
		FMODAudioEngine::sharedEngine()->m_system->playSound(m_sound, CS_Group, false, &Soundchannel);
		Soundchannel->setVolume(GetVolume / 50.f);
		double semitone = static_cast<double>(Mod::get()->getSettingValue<int64_t>("sfx-semitone")) / 12;
		if (semitone < 0) {
			semitone = std::pow(2, semitone); // fix negtive octave
		} else {
			semitone += 1;
		}
		pitchShifterDSP->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, semitone); // semitone is half a octave
	}

	~SoundCache() {
		if (m_sound) {
			m_sound->release();
		}
	}
};

class MultiSoundCache {
  public:
	std::vector<SoundCache *> m_sounds;

	MultiSoundCache() {
		std::srand(static_cast<unsigned>(std::time(nullptr)));
	}

	void SetSounds(const std::vector<std::string> &soundFiles, const std::string &volume, const std::string &custom) {
		std::vector<SoundCache *> memsafe = m_sounds;
		m_sounds.clear();
		for (auto &sound : memsafe) {
			delete sound;
		}
		memsafe.clear();
		for (const auto &soundFile : soundFiles) {
			SoundCache *sound = new SoundCache(volume, custom);
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
		for (auto &sound : m_sounds) {
			delete sound;
		}
	}
};

static MultiSoundCache *ClickSoundIndex = new MultiSoundCache();
static MultiSoundCache *ReleaseSoundIndex = new MultiSoundCache();

// Create the classes for Caching
static SoundCache *ClickSound = new SoundCache("click-volume", "selection-clicks");
static SoundCache *ReleaseSound = new SoundCache("release-volume", "selection-release");