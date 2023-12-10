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
		None,
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
	Piece(const sf::Texture& textures, int piece);

	static int getPieceFromChar(char ch);
	static char getCharFromPiece(int piece);
	static int getColor(int piece);
	static int getType(int piece);
	static bool valid(int piece);

	void snap(int x, int y);
	void setPosition(sf::Vector2f position, bool smooth = false);
	void setPosition(float x, float y, bool smooth = false);
	void updateTargetPosition();

	Piece* clone() const;

	// return 0 for white, 1 for black
	bool color() const;

   private:
	void updateCurrent(sf::Time dt) override;

	// assume that the textures is 510 x 170
	static sf::IntRect getRectByType(int type);

   private:
	sf::Vector2f mVelocity;
	sf::Vector2f mTargetPosition;
	int mType;
};

#endif  //CHESS_PIECE_HPP
