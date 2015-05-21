//-----------------------------------------------------------------------------
//Description:: this is each pices that makes up the board
//Author:: Tommas Solarino
//-----------------------------------------------------------------------------
#ifndef	BOARD_PIECE_H
#define BOARD_PIECE_H
#include <glm/glm.hpp>

#include "Checker.h"

class BoardPiece
{

public:
	BoardPiece();
	~BoardPiece();

	void Draw();

	void CreatePiece( unsigned int _row, unsigned int _col, bool _black, glm::vec3 _pos);
	void SetSelected(bool _bool){ m_selected = _bool; }

	void SetPotentialMove( bool _potentialMove) { m_potentialMove = _potentialMove; }
	bool GetPotentialMove() { return m_potentialMove; }

	bool GetPieceToMove() { return m_pieceToMove; }

	glm::vec3 GetPos(){ return m_position; }
	glm::vec2 GetGridLocation() { return glm::vec2(m_row, m_col); }
	
	Checker* GetChecker() { return m_checker; }

	void SetOcupied(Checker *_checker)
	{
		if(_checker == NULL)
		{
			m_isOcupied = false; 
			m_checker = _checker; 
		}
		else
			m_isOcupied = true; m_checker = _checker; 
	}
	bool GetOcupied() {return m_isOcupied; }

private:
	unsigned int m_row;
	unsigned int m_col;
	bool m_colourIsBlack;
	bool m_selected;
	bool m_isOcupied; //is true if a checker is on it
	bool m_potentialMove;
	bool m_forward;

	bool m_pieceToMove;

	Checker* m_checker;

	glm::vec3 m_position;	
};

#endif