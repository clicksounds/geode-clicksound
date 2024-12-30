#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;
namespace SpritePicker {
    bool secret = false;

    static const char* get(const std::string& Name, bool geode_theme) {   

        if (Name == "GJ_square01.png") {
            return secret
                ? "geode.loader/GE_square03.png" 
                : (geode_theme ? "geode.loader/GE_square01.png" : "GJ_square01.png"); 
        }

        if (Name == "bigFont.fnt") {
             return secret
                ? "goldFont.fnt" 
                : (geode_theme ? "goldFont.fnt" : "bigFont.fnt"); 
        };

        if (Name == "GJ_button_01.png") {
            return secret
                ? "geode.loader/GE_button_04.png" 
                : (geode_theme ? "geode.loader/GE_button_05.png" : "GJ_button_01.png"); 
        };


            return ""; // Default return if no match
        }
}
