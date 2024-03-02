#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/ModMetadata.hpp>
#include <matjson.hpp>

using namespace geode::prelude;

Mod* newIndexToMod(std::string url) {
    auto mod;
    web::AsyncWebRequest()
        .fetch(url)
	    .json()
        .then([&](auto const& webRes){
            auto res = webRes["payload"];
            auto metadata = ModMetadata::create(matjson::parse("{}")).unwrap();
            metadata->setVersion(res["versions"][0]["name"]);
            metadata->setName(res["versions"][0]["version"]);
            metadata->setDevelopers({res["developers"][0]["display_name"]});
            metadata->setDescription(res["versions"][0]["description"]);
            mod = Mod(metadata);
	    });
    return &mod;
}