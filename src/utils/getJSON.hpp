#pragma once
#include <Geode/Geode.hpp>
#include <matjson.hpp>
using namespace geode::prelude;



matjson::Value getJson() {
return matjson::parse(jsonString)
}
