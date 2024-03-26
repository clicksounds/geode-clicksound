#pragma once
#include <Geode/Geode.hpp>
#include <matjson.hpp>
using namespace geode::prelude;

auto jsonString = "";

matjson::Value getJson() {
    return matjson::parse(jsonString)
}