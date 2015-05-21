#include "Checker.h"
#include "Gizmos.h"
Checker::Checker(unsigned int _row, unsigned int _col, bool _black, glm::vec3 _pos)
{
	m_row = _row;
	m_col = _col;
	m_black = _black;
	m_pos = _pos;
	m_king = false;
}

Checker::~Checker()
{

}

void Checker::Update(float _dt)
{
	
}

void Checker::Draw()
{
	//will set this piece to a king if it reaches the other end of the board

	glm::mat4 *project; 
	project = new glm::mat4(1, 0, 0, 1,
							0, 1, 0, 1,
							0, 0, 1, 1,
							0, 0, 0, 1);

	glm::vec4 colour = glm::vec4(1, 1, 1, 1);

	if(m_black)
		colour = glm::vec4(0.0f, 0.0f, 1.0f, 1);	
	else if (!m_black)		
		colour = glm::vec4(1.0f, 0.0f, 0.0f, 1);

	Gizmos::addCylinderFilled(glm::vec3(20 * m_col, 7, 20 * m_row), 8, 2, 20, colour, project);

	if(m_king)
		Gizmos::addCylinderFilled(glm::vec3(20 * m_col, 11, 20 * m_row), 8, 2, 20, colour, project);

}