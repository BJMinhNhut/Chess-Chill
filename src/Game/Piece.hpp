//
// Created by MINH NHUT on 10/23/2023.
//

#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP

#include "Template/ResourceIdentifiers.hpp"
#include "Template/SpriteNode.hpp"

class Piece : public SpriteNode {
   public:
	const static int NAME;
	const static int SIZE;
	enum Type {
		King,
		Queen,
		Bishop,
		Knight,
		Rook,
		Pawn,
		Black = 1 << 3,
		White = 0,
	};
	typedef std::unique_ptr<Piece> Ptr;

   public:
	Piece(const sf::Texture& textures, int type);

	Piece* clone() const;

	// return 0 for white, 1 for black
	bool color() const;

   private:
	// assume that the textures is 510 x 170
	static sf::IntRect getRectByType(int type);

   private:
	int mType;
};

#endif  //CHESS_PIECE_HPP
