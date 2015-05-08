#include "BoardPieces.h"
#include "Gizmos.h"

BoardPiece::BoardPiece()
{
	m_selected = false;
	m_isOcupied = false;
	m_potentialMove = false;
	m_pieceToMove = false;
	m_checker = nullptr;
}

BoardPiece::~BoardPiece()
{

}

void BoardPiece::Draw()
{
	glm::mat4 *project; 
	project = new glm::mat4(1, 0, 0, 1,
							0, 1, 0, 1,
							0, 0, 1, 1,
							0, 0, 0, 1);

	glm::vec4 colour;

	if(m_colourIsBlack && !m_selected)
		colour = glm::vec4(0.0f, 0.1f, 0.1f, 1);
	else if(m_selected)
		colour = glm::vec4(1.0f, 1.0f, 0.0f, 1);
	else
		colour = glm::vec4(1.0f, 1.0f, 0.8f, 1);
	
	if(m_potentialMove)
		colour = glm::vec4(0.0f, 1.0f, 0.0f, 1);
	
	if(m_isOcupied)
		colour = glm::vec4(0.2f, 0.0f, 0.0f, 1);

	
	Gizmos::addAABBFilled(glm::vec3(0 + (20 * m_col), 0, 0 + (20 * m_row)), glm::vec3(10, 5, 10), colour, project);
}

void BoardPiece::CreatePiece( unsigned int _row, unsigned int _col, bool _black, glm::vec3 _pos)
{ 
	m_row = _row; 
	m_col = _col; 
	m_colourIsBlack = _black; 
	m_position = _pos;
}
