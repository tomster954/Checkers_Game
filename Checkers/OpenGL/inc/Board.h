//-----------------------------------------------------------------------------
//Desciption: this is the entire board
//Author: Tommas Solarino
//-----------------------------------------------------------------------------

#include "BoardPieces.h"
#include "Checker.h"

#include <vector>

#ifndef BOARD_H
#define BOARD_H

class Board
{
public:
	Board();
	~Board();

	void Update();
	void Draw();

	void CreateBoard();

	void CheckForMoves(BoardPiece* _checkerSelected);
	void FindEdibleCheckers(glm::vec2 _gridPos);

	void DeselectingPotentialMoves();

	std::vector<BoardPiece*> GetBoardPieces(){ return m_boardPieces; }

private:
	std::vector<BoardPiece*> m_boardPieces;
	std::vector<Checker*> m_checkers;
	BoardPiece *m_checkerSelected;
};
#endif