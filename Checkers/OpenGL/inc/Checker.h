//-----------------------------------------------------------------------------
//Description: This is an individual checker piece on the board
//Author: Tommas Solarino	
//-----------------------------------------------------------------------------

#ifndef CHECKER
#define CHECKER

#include <glm/glm.hpp>

class Checker
{
public:
	Checker(unsigned int _row, unsigned int _col, bool _black, glm::vec3 _pos);
	~Checker();

	void Update(float _dt);
	void Draw();

	void CreateNewChecker();
	bool IsBlack() { return m_black; }
	
	bool IsKing() { return m_king; }
	void SetKing( bool _king) { m_king = _king; }

	void Move(int _row, int _col) {m_row = _row; m_col = _col; }

private:
	unsigned int m_row;
	unsigned int m_col;

	bool m_king;

	bool m_black;
	glm::vec3 m_pos;
};

#endif