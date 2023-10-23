//
// Created by MINH NHUT on 7/4/2023.
//

#include "Settings.hpp"

#include <fstream>
#include <iostream>

Settings::Settings() : theme(Themes::Light) {}

void Settings::print() {
    std::cerr << "==Settings==\n";
    std::cerr << "Theme: ";
    if (theme == Themes::Light)
        std::cerr << "light\n";
    else
        std::cerr << "dark\n";

    std::cerr << "==End==\n";
}

bool Settings::operator==(const Settings &settings) const {
    if (theme != settings.theme)
        return false;
    return true;
}

bool Settings::operator!=(const Settings &settings) const {
    return !((*this) == settings);
}

Settings getSettings() {
    std::ifstream settingsFile("settings.bin",
                               std::ios::in | std::ios::binary);
    Settings settings;
    settingsFile.seekg(0);

    if (!settingsFile.read((char *) &settings, sizeof(settings))) {
        std::cerr << "Settings file doesn't have correct format. "
                     "Init default settings.";
        updateSettings(settings);
    }
    settingsFile.close();
    settings.print();
    return settings;
}

void updateSettings(const Settings &settings) {
    std::ofstream settingsFile("settings.bin",
                               std::ios::out | std::ios::binary);
    settingsFile.seekp(0);
    settingsFile.write((char *) &settings, sizeof(settings));
    settingsFile.close();
}