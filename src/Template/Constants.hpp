//
// Created by MINH NHUT on 6/27/2023.
//

#ifndef DATAVISUALIZATION2_CONSTANTS_HPP
#define DATAVISUALIZATION2_CONSTANTS_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

#include <string>
#include <vector>

namespace Constants {
// Color
sf::Color const mBlack(66, 66, 66);
sf::Color const BlueDark(13, 71, 161);
sf::Color const WhiteDisplay(255, 255, 255);
sf::Color const WhiteUI(245, 245, 245);
sf::Color const GrayBorder(189, 189, 189);
sf::Color const YellowLight(255, 245, 157);
sf::Color const OrangeLight(255, 213, 79);
sf::Color const RedDark(183, 28, 28);
sf::Color const DimLightBorder(189, 189, 189, 50);

sf::Color const BlackUI(69, 90, 100);
sf::Color const BlackDisplay(55, 71, 79);
sf::Color const RedLight(229, 115, 115);
sf::Color const BlueLight(100, 181, 246);
sf::Color const TealDark(0, 96, 100);
sf::Color const TealLight(0, 137, 123);
sf::Color const BlueGrayBorder(81, 105, 117);
sf::Color const DimDarkBorder(81, 105, 117, 50);

// Resource links
std::vector<std::string> const imageNames = {

};

#ifdef SFML_DEBUG
const std::string DATA_PREFIX = "../../";
#else
const std::string DATA_PREFIX = "";
#endif
}  // namespace Constants

#endif  //DATAVISUALIZATION2_CONSTANTS_HPP
