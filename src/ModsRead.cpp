#include <Geode/binding/GJGameState.hpp>
#include <Geode/Geode.hpp>
using namespace geode::prelude;

$execute {
    listenForSettingChanges("clicksound-currentsound", +[](int64_t value) {
        // do something with the value
      GJGameState->CurentClicksound = value;
    });
  GJGameState->CurentClicksound = 0;
}
