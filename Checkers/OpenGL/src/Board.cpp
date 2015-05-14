#include "Board.h"

Board::Board()
{
	CreateBoard();
	m_checkerSelected = new BoardPiece();
}

Board::~Board()
{

}

void Board::Update()
{


}

void Board::Draw()
{
	for (auto piece : m_boardPieces)
	{
		piece->Draw();
	}
	
	for (auto checker : m_checkers)
	{
		checker->Draw();
	}
}

void Board::CreateBoard()
{
	unsigned int rows, cols;
	rows = 8;
	cols = 8;

	bool black = true;

		for(int i = 0; i < rows; i++)
		{	
			for(int j = 0; j < cols; j++)
			{
				BoardPiece *boardPiece = new BoardPiece(); 
				boardPiece->CreatePiece(i, j, black, glm::vec3(0 + (20 * j), 0, 0 + (20 * i)));
				m_boardPieces.push_back(boardPiece);

				if(j >= 0 && j < 3 && black)
				{
					Checker *checker = new Checker(i, j, true, m_boardPieces.back()->GetPos());
					m_checkers.push_back(checker);
					m_boardPieces.back()->SetOcupied(checker);
				}
				if(j >= 5 && j < 8 && black)
				{
					Checker *checker = new Checker(i, j, false, m_boardPieces.back()->GetPos());
					m_checkers.push_back(checker);
					m_boardPieces.back()->SetOcupied(checker);
				}

				black = !black;
			}
			black = !black;
		}
}

void Board::CheckForMoves(BoardPiece* _checkerSelected)
{
	m_checkerSelected = &(*_checkerSelected);

	glm::vec2 gridPos = m_checkerSelected->GetGridLocation();

	for (BoardPiece* piece : m_boardPieces)
	{
		//Moving Blue Checkers
		//---------------------------------------------------------------------------
		if(m_checkerSelected->GetChecker() != nullptr && m_checkerSelected->GetChecker()->IsBlack())//checking if the selected checker has a valid checker
		{
			//[ ][X]
			//[0][ ]
			//[ ][ ] //Checking if top right is free and if it is set it as a potential move
			if(piece->GetGridLocation() == glm::vec2(gridPos.x - 1, gridPos.y + 1) && piece->GetOcupied() == false)
				piece->SetPotentialMove(true);
			else if(piece->GetGridLocation() == glm::vec2(gridPos.x - 1, gridPos.y + 1) && piece->GetOcupied() && !piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
				FindEdibleCheckers(glm::vec2(gridPos.x - 2, gridPos.y + 2));//else find out if you can eat the piece there

			//[ ][ ]
			//[0][ ]
			//[ ][X] //Checking if bottom right is free
			if(piece->GetGridLocation() == glm::vec2(gridPos.x + 1, gridPos.y + 1) && piece->GetOcupied() == false)
				piece->SetPotentialMove(true);
			else if(piece->GetGridLocation() == glm::vec2(gridPos.x + 1, gridPos.y + 1) && piece->GetOcupied() && !piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
				FindEdibleCheckers(glm::vec2(gridPos.x + 2, gridPos.y + 2));
		}
		//---------------------------------------------------------------------------

		//Moving Red Checkers
		//---------------------------------------------------------------------------
		if(m_checkerSelected->GetChecker() != nullptr && m_checkerSelected->GetChecker()->IsBlack() != true)
		{
			//[X][ ]
			//[ ][0]
			//[ ][ ]
			if(piece->GetGridLocation() == glm::vec2(gridPos.x - 1, gridPos.y - 1) && piece->GetOcupied() == false)
				piece->SetPotentialMove(true);
			else if(piece->GetGridLocation() == glm::vec2(gridPos.x - 1, gridPos.y - 1) && piece->GetOcupied() && piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
				FindEdibleCheckers(glm::vec2(gridPos.x - 2, gridPos.y - 2));

			//[ ][ ]
			//[ ][0]
			//[X][ ]
			if(piece->GetGridLocation() == glm::vec2(gridPos.x + 1, gridPos.y - 1) && piece->GetOcupied() == false)
				piece->SetPotentialMove(true);
			else if(piece->GetGridLocation() == glm::vec2(gridPos.x + 1, gridPos.y - 1) && piece->GetOcupied() && piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
				FindEdibleCheckers(glm::vec2(gridPos.x + 2, gridPos.y - 2));
		}
		//---------------------------------------------------------------------------
	}
}

void Board::FindEdibleCheckers(glm::vec2 _gridPos)
{
	bool shouldReturn = false;

	for (BoardPiece* piece : m_boardPieces)
	{
		//set the fist one ur one to potential move if there is a checker there break
		if(piece->GetGridLocation() == glm::vec2(_gridPos.x, _gridPos.y) && piece->GetOcupied() != true)
		{
			piece->SetPotentialMove(true);
		}
		else if(piece->GetGridLocation() == glm::vec2(_gridPos.x, _gridPos.y) && piece->GetOcupied() == true)
		{
			shouldReturn = true;
			break;
		}
		else
			shouldReturn = true;
	}

	if(shouldReturn)
		return;

	for (BoardPiece* piece : m_boardPieces)
	{
		//Checking that the current checker isnt the same colour as the originally selected one.
		if(piece->GetOcupied() == true && piece->GetChecker()->IsBlack() != m_checkerSelected->GetChecker()->IsBlack())
		{
			//Finding Blue moves (blue checker is selected)
			//---------------------------------------------------------------------------
			if (m_checkerSelected->GetChecker()->IsBlack())
			{
				if(piece->GetGridLocation() == glm::vec2(_gridPos.x + 1, _gridPos.y + 1))
				{
					FindEdibleCheckers(glm::vec2(_gridPos.x + 2, _gridPos.y + 2));
				}
				if(piece->GetGridLocation() == glm::vec2(_gridPos.x - 1, _gridPos.y + 1))
				{
					FindEdibleCheckers(glm::vec2(_gridPos.x - 2, _gridPos.y + 2));
				}
			}
			//---------------------------------------------------------------------------

			//Finding Red moves
			//---------------------------------------------------------------------------
			if (!m_checkerSelected->GetChecker()->IsBlack()) //only do this if the checker originally selected is red
			{
				if(piece->GetGridLocation() == glm::vec2(_gridPos.x + 1, _gridPos.y - 1))
				{
					FindEdibleCheckers(glm::vec2(_gridPos.x + 2, _gridPos.y - 2));
				}
				if(piece->GetGridLocation() == glm::vec2(_gridPos.x - 1, _gridPos.y - 1))
				{
					FindEdibleCheckers(glm::vec2(_gridPos.x - 2, _gridPos.y - 2));
				}
			}
			//---------------------------------------------------------------------------
		}

	}
}

void Board::DeselectingPotentialMoves()
{
	for (BoardPiece* bit : m_boardPieces)
		bit->SetPotentialMove(false);
}