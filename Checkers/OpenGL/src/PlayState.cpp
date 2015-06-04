#include "PlayState.h"

#include <GLFW/glfw3.h>
#include <Gizmos.h>

#include "Camera.h"
#include "BoardPieces.h"

PlayState::PlayState(GLFWwindow *_pWindow)
{
	m_network = new Networking();

	Gizmos::create();
	m_pWindow = _pWindow;
	
	ResetGame();

	m_project = new glm::mat4(1, 0, 0, 1,
							0, 1, 0, 1,
							0, 0, 1, 1,
							0, 0, 0, 1);
}

PlayState::~PlayState()
{
	delete m_Board;
	delete m_pieceToMove;
	delete m_project;
}

void PlayState::Update()
{
	m_network->ServerLoop();
	
	//if there is a piece to move from the server move it.
	if (m_network->RecievedPieceToMove())
	{
		BoardPiece* endPos = NULL;
		for (BoardPiece* piece : m_Board->GetBoardPieces())
		{
			if (piece->GetGridLocation() == m_network->GetPieceToMove())
				m_pieceToMove = piece;				
		}
		if (m_pieceToMove->GetOcupied() == true)
			MoveChecker(m_network->GetEndPos());

		m_network->RecievedPieceToMove(false);
	}

	m_Board->Update();

	//if its the console and blues move it true
	if (!m_network->IsServer() && m_network->BluesTurn() == true)
		m_bluesTurn = m_network->BluesTurn();

	//if its the server and blues move is false
	if (m_network->IsServer() && m_network->BluesTurn() == false)
		m_bluesTurn = m_network->BluesTurn();

	//if this is the server and its the servers turn
	if (m_network->IsServer() && !m_bluesTurn)
		SelectingCheckers();

	//if this is the console and its the consols turn
	if (!m_network->IsServer() && m_bluesTurn)
		SelectingCheckers();
	
	if( glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS || 
		glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS ||
		glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS ||
		glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS ||
		glfwGetKey(m_pWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_keyPressed = true;
	else
		m_keyPressed = false;

	CheckForGameOver();
}

void PlayState::Draw(Camera *_camera)
{
	Gizmos::clear();

	m_Board->Draw();

	glm::vec4 colour;

	if(m_bluesTurn)
		colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	else
		colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	Gizmos::addAABBFilled(glm::vec3(70, 30, -20), glm::vec3(10, 5, 10), colour, m_project);


	Gizmos::draw(_camera->getProjectionView());
}

void PlayState::SelectingCheckers()
{
	
	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS && m_keyPressed != true)
		m_selectedRow -= 1;
	if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS && m_keyPressed != true)
		m_selectedCol -= 1;
	if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS && m_keyPressed != true)
		m_selectedRow += 1;
	if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS && m_keyPressed != true)
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
		//Highlights the board piece ur on
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
				if(m_bluesTurn && piece->GetChecker()->IsBlack())
				{
					m_pieceToMove = &(*piece);

					m_Board->DeselectingPotentialMoves();
 					m_Board->BlueMoves(m_pieceToMove);
				}

				if(!m_bluesTurn && !piece->GetChecker()->IsBlack())
				{
					m_pieceToMove = &(*piece);
					
					m_Board->DeselectingPotentialMoves();
					m_Board->RedMoves(m_pieceToMove);
				}

				//TODO:: swap between turns properly
			}

			//If you press space on a potential move
			if(piece->GetGridLocation() == glm::vec2(m_selectedRow, m_selectedCol) && piece->GetPotentialMove())
			{
				bool canMove = false;
				float counter = 0; //counts the number of must moves.
				for (BoardPiece* mustMoves : m_Board->GetBoardPieces())
				{
					//if the mustmoves must be moved and the current piece trying to be moved = it
					if(mustMoves->MustMove())
					{
						counter++;
						
						if(mustMoves->GetGridLocation() == m_pieceToMove->GetGridLocation())
							canMove = true;
					}
				}

				if(canMove)
				{
 					if(m_Board->FindEatenPiece(m_pieceToMove, piece, false))
					{
						m_network->SendPieceThatMoved(m_pieceToMove->GetGridLocation(), piece->GetGridLocation());
						MoveChecker(piece->GetGridLocation());
						//if there are no other moves to make.
						if (m_pieceToMove->GetChecker()->IsBlack() && !m_Board->CanBlueJump())
						{
							m_bluesTurn = !m_bluesTurn;
							m_network->SetWhosTurn(m_bluesTurn);
						}

						if (!m_pieceToMove->GetChecker()->IsBlack() && !m_Board->CanRedJump())
						{
							m_bluesTurn = !m_bluesTurn;
							m_network->SetWhosTurn(m_bluesTurn);
						}
					}
				}
				//if no mustmove where found
				if(counter <= 0)
				{
					m_bluesTurn = !m_bluesTurn;
					m_network->SendPieceThatMoved(m_pieceToMove->GetGridLocation(), piece->GetGridLocation());
					MoveChecker(piece->GetGridLocation());
					m_network->SetWhosTurn(m_bluesTurn);
				}
			}
		}
	}
}

void PlayState::MoveChecker(glm::vec2 _piecePos)
{
	//piece will be a potential move
	BoardPiece* endPos = new BoardPiece();

	for (BoardPiece* piece : m_Board->GetBoardPieces())
		if (piece->GetGridLocation() == _piecePos)
			endPos = &(*piece);
	
	if (endPos == NULL)
		return;
	if (m_pieceToMove->GetOcupied() == false || m_pieceToMove->GetChecker() == NULL)
		return;

	//Move the selected checker to piece
	m_pieceToMove->GetChecker()->Move(endPos->GetGridLocation().x, endPos->GetGridLocation().y);
	
	//set the new spot to ocupied witht the checker that moved
	endPos->SetOcupied(m_pieceToMove->GetChecker());
	
	BoardPiece *start = &(*m_pieceToMove);

	//set the place the checker moved from to empty
	m_pieceToMove->SetOcupied(NULL);
	
	//Check For More moves from where u are now
	m_pieceToMove = &(*endPos);
	m_Board->DeselectingPotentialMoves();

	m_Board->FindEatenPiece(start, m_pieceToMove, true); //get the start pos and end pos and find the checker bettween and remove it
	m_Board->CheckForKings();
}

void PlayState::CheckForGameOver()
{
	int blackPiecesCount = 0;
	int redPiecesCount = 0;
	for (BoardPiece* piece : m_Board->GetBoardPieces())
	{
		if(!piece->GetOcupied())
			continue;

		//Check is someone has won
		if(piece->GetChecker()->IsBlack())
			blackPiecesCount++;
		
		if(!piece->GetChecker()->IsBlack())
			redPiecesCount++;
	}

	if(blackPiecesCount <= 0)
		ResetGame();

	if (redPiecesCount <= 0)
		ResetGame();

	if (m_network->ResetGame())
		ResetGame();
}

void PlayState::ResetGame()
{
	m_Board = new Board;
	m_bluesTurn = false;
	m_gameOver = false;

	//setting the first board piece as selected
	for (auto piece : m_Board->GetBoardPieces())
	if (piece->GetGridLocation() == glm::vec2(0, 0))
	{
		piece->SetSelected(true);
		break;
	}

	m_selectedRow = 0;
	m_selectedCol = 0;

	m_pieceToMove = new BoardPiece();
}