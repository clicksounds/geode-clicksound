#pragma once
#include "../StaticClasses.hpp"

void testSound(bool isClickSound) {
    if (isClickSound) {
        if (Custom_OnClick) {
            ClickSound->Play();
        } else {
            ClickSoundIndex->PlayRandom();
        }
    } else {
        if (Custom_OnLetGo) {
            ReleaseSound->Play();
        } else {
            ReleaseSoundIndex->PlayRandom();
        }
    }
}