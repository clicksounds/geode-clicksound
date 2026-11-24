#include <Geode/Geode.hpp>

using namespace geode::prelude;

$on_mod(Loaded) {
	std::string message = R"(
	Click Sounds is created and maintained by BeatACVR.
		
	Co-developers:
	Cosmella, August.

	Contributors: 
	Uproxide, FireeDev, RayDeeUx.

	Helpers: 
	AkuLunical.

	Index Moderators: 
	Jaid, Andarian, KinobuNine, yesimapotato, whiteisgrey.

	Domain Supporters: 
	ubtertallcats, mxgaming01.

	Click Sounds Pack Installer Owners: 
	zxsetj, Karim768, TheRealNywal, Andarian, commandblockaar, bionsaaisme, destryshereens, GDMineraI, Hwao, Mihele9, SkyeOuO, Podfour.
	)";
	ButtonSettingPressedEventV3(Mod::get(), "buttons").listen([message] (auto buttonKey) {
		if (buttonKey == "credits-button") {
			MDPopup::create("Credits", message, "OK", nullptr, [](bool) {})->show();
		}
	}).leak();
};