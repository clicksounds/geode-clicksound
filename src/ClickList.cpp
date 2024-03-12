#include "./utils/Clicks.hpp"
#include <string>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

static const char* convertToConstChar(const std::string& text) {
    static std::string str;
    str = text;
    return str.c_str();
}
static std::string getmodfix() {
  return ""_spr;
}

const char* Clicks::getClickSprite(int clickSliderValue) {
    const char* clickSoundInUse;
    switch(clickSliderValue) {
      // Other
      case 0: clickSoundInUse = "__USECUSTOM__"; break;


      // convertToConstChar(fmt::format("{}Name-{}.ogg",getmodfix(),std::to_string((rand() % HowMany) + 1 )));

      // Useful sounds
      case 1: clickSoundInUse = "osu-hit.ogg"_spr; break;
      case 2: clickSoundInUse = "metronome.ogg"_spr; break;
      case 3: clickSoundInUse = "click1.ogg"_spr; break;
      case 4: clickSoundInUse = "click2.ogg"_spr; break;
      case 5: clickSoundInUse = "clickrelease-1-click.ogg"_spr; break;
      case 6: clickSoundInUse = "clickrelease-2-click.ogg"_spr; break;
      case 7: clickSoundInUse = "clickrelease-3-click.ogg"_spr; break;
      case 8: clickSoundInUse = "uparrow1.ogg"_spr; break;
      case 9: clickSoundInUse = "spacebar1.ogg"_spr; break;
      case 10: clickSoundInUse = "uparrow2.ogg"_spr; break;
      case 11: clickSoundInUse = "clickrelease-4-click.ogg"_spr; break;
      case 12: clickSoundInUse = convertToConstChar(fmt::format("{}faze-click-{}.ogg",getmodfix(),std::to_string((rand() % 5) + 1 ))); break; 
      case 13: clickSoundInUse = "uparrow3.ogg"_spr; break; 
      case 14: clickSoundInUse = convertToConstChar(fmt::format("{}bloody-click-{}.ogg",getmodfix(),std::to_string((rand() % 5) + 1 ))); break;
      case 15: clickSoundInUse = "spaceuk.ogg"_spr; break;
      case 16: clickSoundInUse = "npesta-click.ogg"_spr; break;
      case 17: clickSoundInUse = "cursed.ogg"_spr; break;
      case 18: clickSoundInUse = "spacebar2-click.ogg"_spr; break;
      case 19: clickSoundInUse = "click3.ogg"_spr; break;  
      case 20: clickSoundInUse = convertToConstChar(fmt::format("{}Viper_Clicks_SpaceBar_{}.ogg",getmodfix(),std::to_string((rand() % 5) + 1 ))); break;
      case 21: clickSoundInUse = convertToConstChar(fmt::format("{}logitech_mx_anywhere_2s_Clicks_{}.ogg",getmodfix(),std::to_string((rand() % 5) + 1 ))); break;
      case 22: clickSoundInUse = "zoink-click.ogg"_spr; break;
      case 23: clickSoundInUse = "screen-tap-1.ogg"_spr; break;
      case 24: clickSoundInUse = "plate.ogg"_spr; break;
      case 25: clickSoundInUse = "neiro-click.ogg"_spr; break;
      case 26: clickSoundInUse = "doggie.ogg"_spr; break;
      case 27: clickSoundInUse = "trusta.ogg"_spr; break;
      case 28: clickSoundInUse = "gazonk.ogg"_spr; break; 
      case 29: clickSoundInUse = convertToConstChar(fmt::format("{}sayo-click-{}.ogg",getmodfix(),std::to_string((rand() % 8) + 1 ))); break;
      case 30: clickSoundInUse = convertToConstChar(fmt::format("{}viper-click-{}.ogg",getmodfix(),std::to_string((rand() % 9) + 1 ))); break;
      case 31: clickSoundInUse = convertToConstChar(fmt::format("{}Logitech_G502-{}-{}.ogg",getmodfix(),(rand() % 100 == 0) ? "soft" : ((rand() % 100 > 50) ? "hard" : "normal"),std::to_string((rand() % 10) + 1 ))); break;

      // Meme sounds
      case -1: clickSoundInUse = "fire-in-the-hole.ogg"_spr; break;
      case -2: clickSoundInUse = "i-love-gd-cologne.ogg"_spr; break;
      case -3: clickSoundInUse = "vine-boom.ogg"_spr; break;
      case -4: clickSoundInUse = "bwomp.ogg"_spr; break;
      case -5: clickSoundInUse = "metal-pipe.ogg"_spr; break;
      case -6: clickSoundInUse = "tiktok-click.ogg"_spr; break;
      case -7: clickSoundInUse = "plug.ogg"_spr; break;
      case -8: clickSoundInUse = "el-gato.ogg"_spr; break;
      case -9: clickSoundInUse = "bonk.ogg"_spr; break;
      case -10: clickSoundInUse = "discordping.ogg"_spr; break;
      case -11: clickSoundInUse = "hamburger.ogg"_spr; break;
      case -12: clickSoundInUse = "gd-death.ogg"_spr; break;
      case -13: clickSoundInUse = "desk-hit.ogg"_spr; break;
      case -14: clickSoundInUse = (rand() % 2 == 0) ? "win10usbconnect.ogg"_spr : "win10usbdisconnect.ogg"_spr; break;
      case -15: clickSoundInUse = "taco-bell.ogg"_spr; break;
      case -16: clickSoundInUse = "bad-to-the-bone.ogg"_spr; break;
      case -17: clickSoundInUse = "huh.ogg"_spr; break;
      case -18: clickSoundInUse = (rand() % 5 == 0) ? "dash-one.ogg"_spr : ((rand() % 5 == 1) ? "dash-two.ogg"_spr : ((rand() % 5 == 2) ? "dash-three.ogg"_spr : ((rand() % 5 == 3) ? "dash-dash.ogg"_spr : "dash-geometry.ogg"_spr))); break;
      case -19: clickSoundInUse = "aughhh.ogg"_spr; break;
      case -20: clickSoundInUse = "car-door-slam.ogg"_spr; break;
      case -21: clickSoundInUse = "spectre.ogg"_spr; break;
      case -22: clickSoundInUse = (rand() % 500 == 0) ? "congregation-jumpscare.ogg"_spr : "osu-hit.ogg"_spr; break;
      case -23: clickSoundInUse = "soda.ogg"_spr; break;
      case -24: clickSoundInUse = "water-on-the-hill.ogg"_spr; break;
      case -25: clickSoundInUse = "riot-quack.ogg"_spr; break;
      case -26: clickSoundInUse = "fuck.ogg"_spr; break;
      case -27: clickSoundInUse = "gravity-coil.ogg"_spr; break;
    }
    return clickSoundInUse;
};
