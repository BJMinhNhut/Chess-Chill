//
// Created by MINH NHUT on 7/4/2023.
//

#ifndef DATAVISUALIZATION2_SETTINGS_HPP
#define DATAVISUALIZATION2_SETTINGS_HPP

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

struct Settings {
    enum Themes {
        Light, Dark
    };
    Themes theme;

    Settings();

    void print();

    bool operator==(const Settings &settings) const;

    bool operator!=(const Settings &settings) const;
};

Settings getSettings();

void updateSettings(const Settings &settings);

#endif  // SETTINGS_HPP

#endif //DATAVISUALIZATION2_SETTINGS_HPP
