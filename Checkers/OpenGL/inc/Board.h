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

	void BlueMoves(BoardPiece* _pieceToMove);
	void RedMoves(BoardPiece* _pieceToMove);

	void FindEdibleCheckers(glm::vec2 _gridPos);

	void DeselectingPotentialMoves();
	void FindEatenPiece(BoardPiece *_startLocation, BoardPiece *_endLocation);

	void SetMustMoves();

	void SetBluesTurn( bool _bluesTurn) { m_blueTurn = _bluesTurn; }
	bool BluesTurn() { return m_blueTurn; }

	bool CanBlueJump();
	bool CanRedJump();

	std::vector<BoardPiece*> GetBoardPieces(){ return m_boardPieces; }

private:
	std::vector<BoardPiece*> m_boardPieces;
	std::vector<BoardPiece*> m_mustMoveThese;

	std::vector<Checker*> m_checkers;
	BoardPiece *m_checkerSelected;
	BoardPiece *m_eatenPiece;

	bool m_blueTurn;

	bool m_blueCanJump;
	bool m_redCanJump;
	bool m_moreEdibleBits;

	bool m_prevMoveColorWasBlack;
};
#endif