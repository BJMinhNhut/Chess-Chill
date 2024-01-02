//
// Created by MINH NHUT on 1/2/2024.
//

#ifndef CHESS_CHILL_HISTORYPANEL_HPP
#define CHESS_CHILL_HISTORYPANEL_HPP

#include "Button.hpp"

#include <SFML/Graphics/Text.hpp>

namespace GUI {
class HistoryPanel : public Button {
   public:
	typedef std::shared_ptr<HistoryPanel> Ptr;

   public:
	HistoryPanel(const FontHolder& fonts, const TextureHolder& textures);

	void setTime(const std::string& time);
	void setMode(const std::string& mode);
	void setType(const std::string& type);
	void setResult(const std::string& result);
	void setDate(const std::string& date);

   private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

   private:
	sf::Text mTime, mDate, mMode, mType, mResult;
};
}  // namespace GUI

#endif  //CHESS_CHILL_HISTORYPANEL_HPP
