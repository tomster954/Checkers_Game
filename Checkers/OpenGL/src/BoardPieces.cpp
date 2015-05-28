#include "BoardPieces.h"
#include "Gizmos.h"

BoardPiece::BoardPiece()
{
	m_selected = false;
	m_isOcupied = false;
	m_potentialMove = false;
	m_pieceToMove = false;
	m_checker = nullptr;
	m_mustMove = false;

	m_project = new glm::mat4(1, 0, 0, 1,
							0, 1, 0, 1,
							0, 0, 1, 1,
							0, 0, 0, 1);
}

BoardPiece::~BoardPiece()
{
	
}

void BoardPiece::Draw()
{
	glm::vec4 colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	if(m_selected)
		colour = (glm::vec4(0.0f, 1.0f, 1.0f, 1));
	else if(m_potentialMove)
		colour = (glm::vec4(0.0f, 1.0f, 0.0f, 1));
	else if(m_colourIsBlack)
		colour = (glm::vec4(0.0f, 0.1f, 0.1f, 1));

	if(m_mustMove)
		colour = (glm::vec4(1.0f, 0.0f, 0.0f, 1));

	Gizmos::addAABBFilled(glm::vec3(0 + (20 * m_col), 0, 0 + (20 * m_row)), glm::vec3(10, 5, 10), colour, m_project);
}

void BoardPiece::CreatePiece( unsigned int _row, unsigned int _col, bool _black, glm::vec3 _pos)
{ 
	m_row = _row; 
	m_col = _col; 
	m_colourIsBlack = _black; 
	m_position = _pos;
}
