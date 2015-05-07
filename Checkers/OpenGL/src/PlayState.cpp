#include "PlayState.h"
#include "Camera.h"
#include "BoardPieces.h"

#include <GLFW/glfw3.h>
#include <Gizmos.h>

PlayState::PlayState(GLFWwindow *_pWindow)
{
	Gizmos::create();
	m_pWindow = _pWindow;
	m_Board = new Board;

	//setting the first board piece as selected
	for (auto piece : m_Board->GetBoardPieces())
		if(piece->GetGridLocation() == glm::vec2(0, 0))
			piece->SetSelected(true);
	
	m_selectedRow = 0;
	m_selectedCol = 0;

	m_pieceToMove = new BoardPiece();
}

PlayState::~PlayState()
{

}

void PlayState::Update()
{
	SelectingCheckers();
	
	if( glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS || 
		glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS ||
		glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS ||
		glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS ||
		glfwGetKey(m_pWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_keyPressed = true;
	else
		m_keyPressed = false;
}

void PlayState::Draw(Camera *_camera)
{
	Gizmos::clear();

	m_Board->Draw();

	Gizmos::draw(_camera->getProjectionView());
}

void PlayState::SelectingCheckers()
{
	if(glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS && m_keyPressed != true)
		m_selectedRow -= 1;
	if(glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS && m_keyPressed != true)
		m_selectedCol -= 1;
	if(glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS && m_keyPressed != true)
		m_selectedRow += 1;
	if(glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS && m_keyPressed != true)
		m_selectedCol += 1;

	//Borders
	//------------------------------------
	if(m_selectedCol > 7)
		m_selectedCol = 0;
	if(m_selectedCol < 0)
		m_selectedCol = 7;

	if(m_selectedRow > 7)
		m_selectedRow = 0;
	if(m_selectedRow < 0)
		m_selectedRow = 7;
	//------------------------------------

	for (auto piece : m_Board->GetBoardPieces())
	{
		//if(piece->GetPieceToMove())
		//{
		//	
		//}
			

		if(piece->GetGridLocation() == glm::vec2(m_selectedRow, m_selectedCol))
			piece->SetSelected(true);
		else
			piece->SetSelected(false);

		//If Spcae is pressed 
		if(glfwGetKey(m_pWindow, GLFW_KEY_SPACE) == GLFW_PRESS && m_keyPressed != true)
		{
			//is the curr piece = to the piece ur on And its got a checker on it
			if(piece->GetGridLocation() == glm::vec2(m_selectedRow, m_selectedCol) && piece->GetOcupied())
			{
				m_pieceToMove = &(*piece);
				//pieceToMove = piece;
				CheckingForMoves(piece);
			}

			//If you press space on a potential move
			if(piece->GetGridLocation() == glm::vec2(m_selectedRow, m_selectedCol) && piece->GetPotentialMove())
			{
				//Move the selecte checker to piece
				m_pieceToMove->GetChecker()->Move(m_selectedRow, m_selectedCol);
				//set the new stop to ocupied witht the checker that moved
				piece->SetOcupied(m_pieceToMove->GetChecker());
				//set the place the checker moved from to empty
				m_pieceToMove->SetOcupied(NULL);
			}
		}
	}

	
}

void PlayState::CheckingForMoves(BoardPiece *_piece)
{
	for (auto piece : m_Board->GetBoardPieces())
	{
		if(piece->GetGridLocation() == glm::vec2(_piece->GetGridLocation().x - 1, _piece->GetGridLocation().y + 1))
		{
			//[ ][X]
			//[0][ ]
			//[ ][ ]
			piece->SetPotentialMove(true);
		}
		else if (piece->GetGridLocation() != glm::vec2(_piece->GetGridLocation().x + 1, _piece->GetGridLocation().y + 1))
			piece->SetPotentialMove(false);

		if(piece->GetGridLocation() == glm::vec2(_piece->GetGridLocation().x + 1, _piece->GetGridLocation().y + 1))
		{
			//[ ][ ]
			//[0][ ]
			//[ ][X]
			piece->SetPotentialMove(true);
		}
	
	}
}