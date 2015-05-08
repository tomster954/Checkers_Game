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
		{
			piece->SetSelected(true);
			break;
		}
	
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

	for (BoardPiece* piece : m_Board->GetBoardPieces())
	{
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
				
				DeselectingPotentialMoves();
				CheckingForMoves(piece->GetGridLocation());
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
				DeselectingPotentialMoves();
			}
		}
	}

	
}

void PlayState::CheckingForMoves(glm::vec2 _gridPos)
{
	for (BoardPiece* piece : m_Board->GetBoardPieces())
	{

		//Moving Blue Checkers
		//---------------------------------------------------------------------------
		if(m_pieceToMove->GetChecker() != nullptr && m_pieceToMove->GetChecker()->IsBlack())
		{
			//[ ][X]
			//[0][ ]
			//[ ][ ]
			if(piece->GetGridLocation() == glm::vec2(_gridPos.x - 1, _gridPos.y + 1) && piece->GetOcupied() == false)
				piece->SetPotentialMove(true);
			else if(piece->GetGridLocation() == glm::vec2(_gridPos.x - 1, _gridPos.y + 1) && piece->GetOcupied() && !piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
				FindEdibleCheckers(glm::vec2(_gridPos.x - 2, _gridPos.y + 2));

			//[ ][ ]
			//[0][ ]
			//[ ][X]
			if(piece->GetGridLocation() == glm::vec2(_gridPos.x + 1, _gridPos.y + 1) && piece->GetOcupied() == false)
				piece->SetPotentialMove(true);
			else if(piece->GetGridLocation() == glm::vec2(_gridPos.x + 1, _gridPos.y + 1) && piece->GetOcupied() && !piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
				FindEdibleCheckers(glm::vec2(_gridPos.x + 2, _gridPos.y + 2));
		}
		//---------------------------------------------------------------------------

		//Moving Red Checkers
		//---------------------------------------------------------------------------
		if(m_pieceToMove->GetChecker() != nullptr && m_pieceToMove->GetChecker()->IsBlack() != true)
		{
			//[X][ ]
			//[ ][0]
			//[ ][ ]
			if(piece->GetGridLocation() == glm::vec2(_gridPos.x - 1, _gridPos.y - 1) && piece->GetOcupied() == false)
				piece->SetPotentialMove(true);
			else if(piece->GetGridLocation() == glm::vec2(_gridPos.x - 1, _gridPos.y - 1) && piece->GetOcupied() && piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
				FindEdibleCheckers(glm::vec2(_gridPos.x - 2, _gridPos.y - 2));

			//[ ][ ]
			//[ ][0]
			//[X][ ]
			if(piece->GetGridLocation() == glm::vec2(_gridPos.x + 1, _gridPos.y - 1) && piece->GetOcupied() == false)
				piece->SetPotentialMove(true);
			else if(piece->GetGridLocation() == glm::vec2(_gridPos.x + 1, _gridPos.y - 1) && piece->GetOcupied() && piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
				FindEdibleCheckers(glm::vec2(_gridPos.x + 2, _gridPos.y - 2));
		}
		//---------------------------------------------------------------------------
	}
}

void PlayState::DeselectingPotentialMoves()
{
	for (BoardPiece* bit : m_Board->GetBoardPieces())
		bit->SetPotentialMove(false);
}

void PlayState::FindEdibleCheckers(glm::vec2 _gridPos)
{
	for (BoardPiece* piece : m_Board->GetBoardPieces())
	{
		//set the fist one ur one to potential move
		if(piece->GetGridLocation() == glm::vec2(_gridPos.x, _gridPos.y) && piece->GetOcupied() != true)
		{
			piece->SetPotentialMove(true);
		}

		//Moving Blue Checkers
		//---------------------------------------------------------------------------
		if(piece->GetGridLocation() == glm::vec2(_gridPos.x + 1, _gridPos.y + 1) && piece->GetOcupied() == true && !piece->GetChecker()->IsBlack())
		{
			FindEdibleCheckers(glm::vec2(_gridPos.x + 2, _gridPos.y + 2));
		}
		if(piece->GetGridLocation() == glm::vec2(_gridPos.x - 1, _gridPos.y + 1) && piece->GetOcupied() == true && !piece->GetChecker()->IsBlack())
		{
			FindEdibleCheckers(glm::vec2(_gridPos.x - 2, _gridPos.y + 2));
		}
		//---------------------------------------------------------------------------

		//Moving Red Checkers TODO Check IF red
		//---------------------------------------------------------------------------
		if(piece->GetGridLocation() == glm::vec2(_gridPos.x + 1, _gridPos.y - 1) && piece->GetOcupied() == true && piece->GetChecker()->IsBlack())
		{
			FindEdibleCheckers(glm::vec2(_gridPos.x + 2, _gridPos.y - 2));
		}
		if(piece->GetGridLocation() == glm::vec2(_gridPos.x - 1, _gridPos.y - 1) && piece->GetOcupied() == true && piece->GetChecker()->IsBlack())
		{
			FindEdibleCheckers(glm::vec2(_gridPos.x - 2, _gridPos.y - 2));
		}
		//---------------------------------------------------------------------------
	}
}