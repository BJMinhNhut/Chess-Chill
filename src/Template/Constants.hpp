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
sf::Color const mWhite(255, 252, 249);
sf::Color const mBlack(41, 41, 41);
sf::Color const mBlueDark(77, 88, 106);
sf::Color const mRed(215, 38, 56);
sf::Color const mCyan(177, 237, 232);

#ifdef SFML_DEBUG
const std::string DATA_PREFIX = "../../";
#else
const std::string DATA_PREFIX = "";
#endif
}  // namespace Constants

#endif  //DATAVISUALIZATION2_CONSTANTS_HPP
