#pragma once
#include <Geode/Geode.hpp>
#include <matjson.hpp>
#include "getJSON.hpp"
using namespace geode::prelude;
class Clicks {
protected:
    static matjson::Value m_ClickList;
    static matjson::Value m_ReleaseList;
    static matjson::Value m_BackClickList;
    static matjson::Value m_BackReleaseList;
    static int m_CM;
    static int m_CU;
    static int m_RM;
    static int m_RU;
public:
    static const char* getClickSprite(int);
    static const char* getReleaseSprite(int);
    static const int getMemeClickLen() {return m_CM;}
    static const int getClickLen() {return m_CU;}
    static const int getMemeReleaseLen() {return m_RM;}
    static const int getReleaseLen() {return m_RU;}
    static const matjson::Value getClickList() {return m_ClickList;}
    static const matjson::Value getReleaseList() {return m_ReleaseList;}
    static const matjson::Value getBackClickList() {return m_BackClickList;}
    static const matjson::Value getBackReleaseList() {return m_BackReleaseList;}
    static const void initJson() {
        auto thing = getJson();
        m_ClickList = thing["Reg"]["Clicks"];
        m_ReleaseList = thing["Reg"]["Releases"];
        m_BackClickList = thing["Back"]["Clicks"];
        m_BackReleaseList = thing["Back"]["Releases"];
        m_CU = thing["Len"]["CU"].as<int>();
        m_CM = thing["Len"]["CM"].as<int>();
        m_RM = thing["Len"]["RM"].as<int>();
        m_RU = thing["Len"]["RU"].as<int>();
    }
};