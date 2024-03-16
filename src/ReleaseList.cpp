#include "./utils/Clicks.hpp"
static const char* convertToConstChar(const std::string& text) {
    static std::string str;
    str = text;
    return str.c_str();
}
static std::string getmodfix() {
  return ""_spr;
}

const char* Clicks::getReleaseSprite(int clickSliderValue) {
    const char* releaseSoundInUse;
    switch(clickSliderValue) {
      // Other
      case 0: releaseSoundInUse = "__USECUSTOM__"; break;
      
      // convertToConstChar(fmt::format("{}Name-{}.ogg",getmodfix(),std::to_string((rand() % HowMany) + 1 )));

      // Useful sounds
      case 1: releaseSoundInUse = "clickrelease-1-release.ogg"_spr; break;
      case 2: releaseSoundInUse = "clickrelease-2-release.ogg"_spr; break;
      case 3: releaseSoundInUse = "clickrelease-3-release.ogg"_spr; break;
      case 4: releaseSoundInUse = "clickrelease-4-release.ogg"_spr; break; 
      case 5: releaseSoundInUse = convertToConstChar(fmt::format("{}faze-release-{}.ogg",getmodfix(),std::to_string((rand() % 4) + 1 ))); break; 
      case 6: releaseSoundInUse = convertToConstChar(fmt::format("{}bloody-release-{}.ogg",getmodfix(),std::to_string((rand() % 5) + 1 ))); break; 
      case 7: releaseSoundInUse = "npesta-release.ogg"_spr; break;
      case 8: releaseSoundInUse = "spacebar2-release.ogg"_spr; break;
      case 9: releaseSoundInUse = convertToConstChar(fmt::format("{}Viper_SpaceBar_release_{}.ogg",getmodfix(),std::to_string((rand() % 3) + 1 ))); break; 
      case 10: releaseSoundInUse = convertToConstChar(fmt::format("{}logitech_mx_anywhere_2s_Releases_{}.ogg",getmodfix(),std::to_string((rand() % 5) + 1 ))); break; 
      case 11: releaseSoundInUse = "zoink-release.ogg"_spr; break;
      case 12: releaseSoundInUse = "neiro-release.ogg"_spr; break;
      case 13: releaseSoundInUse = convertToConstChar(fmt::format("{}sayo-release-{}.ogg",getmodfix(),std::to_string((rand() % 20) + 1 ))); break; 
      case 14: releaseSoundInUse = convertToConstChar(fmt::format("{}viper-release-{}.ogg",getmodfix(),std::to_string((rand() % 20) + 1 ))); break; 
      case 15: releaseSoundInUse = "Logitech_G502-release-1.ogg"_spr; break;
      case 16: releaseSoundInUse = convertToConstChar(fmt::format("{}up-arrow-4-release-{}.ogg",getmodfix(),std::to_string((rand() % 10) + 1 ))); break; 
      case 17: releaseSoundInUse = convertToConstChar(fmt::format("{}mouse-clicks-6-Release-{}.ogg",getmodfix(),std::to_string((rand() % 10) + 1 ))); break;
      case 18: releaseSoundInUse = convertToConstChar(fmt::format("{}0PacitysClicksV2releases{}.ogg",getmodfix(),std::to_string((rand() % 4) + 1 ))); break;

      // Meme sounds
      case -1: releaseSoundInUse = "tiktok-release.ogg"_spr; break;
    }
    return releaseSoundInUse;
};
