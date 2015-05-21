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
	
	void CheckForKings(BoardPiece* _piece); 
	void CheckForMoves(BoardPiece* _checkerSelected);

	void BlueMoves(BoardPiece* _checkerSelected);
	void RedMoves(BoardPiece* _checkerSelected);

	void FindEdibleCheckers(glm::vec2 _gridPos);

	void DeselectingPotentialMoves();
	void FindEatenPiece(BoardPiece *_startLocation, BoardPiece *_endLocation);

	void SetBluesTurn( bool _bluesTurn) { m_blueTurn = _bluesTurn; }
	bool BluesTurn() { return m_blueTurn; }

	bool MoreBlueMoves() { return m_moreBlueMoves; }
	bool MoreRedMoves() { return m_moreRedMoves; }

	std::vector<BoardPiece*> GetBoardPieces(){ return m_boardPieces; }

private:
	std::vector<BoardPiece*> m_boardPieces;
	std::vector<Checker*> m_checkers;
	BoardPiece *m_checkerSelected;
	BoardPiece *m_eatenPiece;

	bool m_blueTurn;

	bool m_moreBlueMoves;
	bool m_moreRedMoves;
	bool m_moreEdibleBits;

	bool m_prevMoveColorWasBlack;
};
#endif