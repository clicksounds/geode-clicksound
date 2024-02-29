#include "./utils/Clicks.hpp"

const char* Clicks::getReleaseSprite(int clickSliderValue) {
    const char* releaseSoundInUse;
    switch(clickSliderValue) {
      // Other
      case 0: releaseSoundInUse = "__USECUSTOM__"; break;

      // Useful sounds
      case 1: releaseSoundInUse = "clickrelease-1-release.ogg"_spr; break;
      case 2: releaseSoundInUse = "clickrelease-2-release.ogg"_spr; break;
      case 3: releaseSoundInUse = "clickrelease-3-release.ogg"_spr; break;
      case 4: releaseSoundInUse = "clickrelease-4-release.ogg"_spr; break;
      case 5: releaseSoundInUse = (rand() % 4 == 0) ? "faze-release-1.ogg"_spr : ((rand() % 4 == 1) ? "faze-release-2.ogg"_spr : ((rand() % 4 == 2) ? "faze-release-3.ogg"_spr : "faze-release-4.ogg"_spr)); break;
      case 6: releaseSoundInUse = (rand() % 5 == 0) ? "bloody-release-1.ogg"_spr : ((rand() % 5 == 1) ? "bloody-release-2.ogg"_spr : ((rand() % 5 == 2) ? "bloody-release-3.ogg"_spr : ((rand() % 5 == 3) ? "bloody-release-4.ogg"_spr : "bloody-release-5.ogg"_spr))); break;
      case 7: releaseSoundInUse = "npesta-release.ogg"_spr; break;
      case 8: releaseSoundInUse = "spacebar2-release.ogg"_spr; break;
      case 9: releaseSoundInUse = ((rand() % 3 == 0) ? "Viper_SpaceBar_release_1.ogg"_spr : ((rand() % 3 == 1) ? "Viper_SpaceBar_release_2.ogg"_spr : "Viper_SpaceBar_release_3.ogg"_spr)); break;
      case 10: releaseSoundInUse = ((rand() % 5 == 0) ? "logitech_mx_anywhere_2s_Releases_1.ogg"_spr : ((rand() % 5 == 1) ? "logitech_mx_anywhere_2s_Releases_2.ogg"_spr : ((rand() % 5 == 2) ? "logitech_mx_anywhere_2s_Releases_3.ogg"_spr : ((rand() % 5 == 3) ? "logitech_mx_anywhere_2s_Releases_4.ogg"_spr : "logitech_mx_anywhere_2s_Releases_5.ogg"_spr)))); break;
      case 11: releaseSoundInUse = "zoink-release.ogg"_spr; break;
      case 12: releaseSoundInUse = "neiro-release.ogg"_spr; break;
      case 13: releaseSoundInUse = ((rand() % 8 == 0) ? "sayo-release-1.ogg"_spr : ((rand() % 8 == 1) ? "sayo-release-2.ogg"_spr : ((rand() % 8 == 2) ? "sayo-release-3.ogg"_spr : ((rand() % 8 == 3) ? "sayo-release-4.ogg"_spr : ((rand() % 8 == 4) ? "sayo-release-5.ogg"_spr : ((rand() % 8 == 5) ? "sayo-release-6.ogg"_spr : ((rand() % 8 == 6) ? "sayo-release-7.ogg"_spr : "sayo-release-8.ogg"_spr))))))); break;

      // Meme sounds
      case -1: releaseSoundInUse = "tiktok-release.ogg"_spr; break;
    }
    return releaseSoundInUse;
};