#include "Board.h"

Board::Board()
{
	CreateBoard();
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
	glm::vec2 gridPos = _checkerSelected->GetGridLocation();

	for (BoardPiece* piece : m_boardPieces)
	{

		//Moving Blue Checkers
		//---------------------------------------------------------------------------
		if(_checkerSelected->GetChecker() != nullptr && _checkerSelected->GetChecker()->IsBlack())
		{
			//[ ][X]
			//[0][ ]
			//[ ][ ] //Checking if top right is free
			if(piece->GetGridLocation() == glm::vec2(gridPos.x - 1, gridPos.y + 1) && piece->GetOcupied() == false)
				piece->SetPotentialMove(true);
			else if(piece->GetGridLocation() == glm::vec2(gridPos.x - 1, gridPos.y + 1) && piece->GetOcupied() && !piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
				FindEdibleCheckers(glm::vec2(gridPos.x - 2, gridPos.y + 2));

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
		if(_checkerSelected->GetChecker() != nullptr && _checkerSelected->GetChecker()->IsBlack() != true)
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
	for (BoardPiece* piece : m_boardPieces)
	{
		//set the fist one ur one to potential move if there is a checker there break
		if(piece->GetGridLocation() == glm::vec2(_gridPos.x, _gridPos.y) && piece->GetOcupied() != true)
		{
			piece->SetPotentialMove(true);
		}
		else if(piece->GetGridLocation() == glm::vec2(_gridPos.x, _gridPos.y) && piece->GetOcupied() == true)
			break;

		//Finding Blue moves
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

		//TODO Check the starting chechers colour and compare it to the peice ur on now instead of asking if its black. that doesnt stop eating of same colours.
		//Finding Red moves
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

void Board::DeselectingPotentialMoves()
{
	for (BoardPiece* bit : m_boardPieces)
		bit->SetPotentialMove(false);
}