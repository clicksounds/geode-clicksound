#pragma once
#include <Geode/Geode.hpp>
#include <matjson.hpp>
#include "getJSON.hpp"
using namespace geode::prelude;
class Clicks {
protected:
    static matjson::Value* m_ClickList;
    static matjson::Value* m_ReleaseList;
    static matjson::Value* m_BackClickList;
    static matjson::Value* m_BackReleaseList;
public:
    static const char* getClickSprite(int);
    static const char* getReleaseSprite(int);
    static const matjson::Value* getClickList() {return m_ClickList;}
    static const matjson::Value* getReleaseList() {return m_ReleaseList;}
    static const matjson::Value* getBackClickList() {return m_BackClickList;}
    static const matjson::Value* getBackReleaseList() {return m_BackReleaseList;}
    static const void initJson() {auto thing = getJson();m_ClickList = thing["Reg"]["Clicks"];m_ReleaseList = thing["Reg"]["Releases"];m_BackClickList = thing["Back"]["Clicks"];m_BackReleaseList = thing["Back"]["Releases"];}
};