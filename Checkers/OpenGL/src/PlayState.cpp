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
	m_Board->Update();

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
				
				m_Board->DeselectingPotentialMoves();
				m_Board->CheckForMoves(m_pieceToMove);
			}

			//If you press space on a potential move
			if(piece->GetGridLocation() == glm::vec2(m_selectedRow, m_selectedCol) && piece->GetPotentialMove())
			{
				//Move the selecte checker to piece
				m_pieceToMove->GetChecker()->Move(m_selectedRow, m_selectedCol);
				
				//set the new spot to ocupied witht the checker that moved
				piece->SetOcupied(m_pieceToMove->GetChecker());

				BoardPiece *start = &(*m_pieceToMove);
				//set the place the checker moved from to empty
				m_pieceToMove->SetOcupied(NULL);

				//Check For More moves from where u are now
				m_pieceToMove = &(*piece);
 				m_Board->DeselectingPotentialMoves();
				
				m_Board->FindEatenPiece(start, m_pieceToMove); //get the start pos and end pos and find the checker bettween.
				
				
				float x = start->GetGridLocation().x - m_pieceToMove->GetGridLocation().x;
				float y = start->GetGridLocation().y - m_pieceToMove->GetGridLocation().y; 

				//if the change on the x and y axis has changed 2 places meaning it has jumped something
				if(abs(x) == 2 && abs(y) == 2)
					m_Board->CheckForMoves(m_pieceToMove);

				if(m_pieceToMove->GetChecker()->IsBlack() && !m_Board->MoreBlueMoves())
				{
 					m_Board->DeselectingPotentialMoves();
					m_Board->SetBluesTurn(!m_Board->BluesTurn());
				}
				if(!m_pieceToMove->GetChecker()->IsBlack() && !m_Board->MoreRedMoves())
				{
					m_Board->DeselectingPotentialMoves();
					m_Board->SetBluesTurn(!m_Board->BluesTurn());
				}
				
			}
		}
	}
}