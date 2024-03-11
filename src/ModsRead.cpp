#include <Geode/binding/GJGameState.hpp>
#include <Geode/Geode.hpp>
using namespace geode::prelude;
class GJGameState
{
static int CurentClicksound = 0;
};

$on_mod(Loaded) {
listenForSettingChanges("clicksound-currentsound", +[](int value) {
        // do something with the value
      GJGameState::CurentClicksound = value;
    });
  GJGameState::CurentClicksound = 0;
}
