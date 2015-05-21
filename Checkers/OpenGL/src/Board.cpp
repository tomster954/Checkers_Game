#include "Board.h"
#include <iostream>

Board::Board()
{
	CreateBoard();
	m_checkerSelected = new BoardPiece();
}

Board::~Board()
{
	delete m_checkerSelected;
}

void Board::Update()
{

}

void Board::Draw()
{
	for (auto piece : m_boardPieces)
	{
		//draw each piece and the checker that it has on it
		piece->Draw();
		if (piece->GetOcupied() != NULL)
			piece->GetChecker()->Draw();
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

	//Loops through all the board pieces and check for potential moves based iff the selected checker
	for (BoardPiece* piece : m_boardPieces)
	{
		//Finding Potential moves for Blue
		//---------------------------------------------------------------------------
		if(m_checkerSelected->GetChecker() != nullptr && m_checkerSelected->GetChecker()->IsBlack())//checking if the selected checker has a valid checker
		{
			//[ ][X]
			//[0][ ]
			//[ ][ ] 
			//Check to see if the diaginal piece is vacant, if it is set it as a potential move
			//Else if its ocupied and its the opposite colour it might be edible so call the FindEdibleCheckers function
			if(piece->GetGridLocation() == glm::vec2(gridPos.x - 1, gridPos.y + 1) && piece->GetOcupied() == false)
				piece->SetPotentialMove(true);
			else if(piece->GetGridLocation() == glm::vec2(gridPos.x - 1, gridPos.y + 1) && piece->GetOcupied() && !piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
				FindEdibleCheckers(glm::vec2(gridPos.x - 2, gridPos.y + 2));//else find out if you can eat the piece there

			//[ ][ ]
			//[0][ ]
			//[ ][X]
			//Check to see if the diaginal piece is vacant, if it is set it as a potential move
			//Else if its ocupied and its the opposite colour it might be edible so call the FindEdibleCheckers function
			if(piece->GetGridLocation() == glm::vec2(gridPos.x + 1, gridPos.y + 1) && piece->GetOcupied() == false)
				piece->SetPotentialMove(true);
			else if(piece->GetGridLocation() == glm::vec2(gridPos.x + 1, gridPos.y + 1) && piece->GetOcupied() && !piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
				FindEdibleCheckers(glm::vec2(gridPos.x + 2, gridPos.y + 2));

			//Have the Ability to move in every direction once a king
			if(m_checkerSelected->GetChecker()->IsKing())
			{
				//[X][ ]
				//[ ][0]
				//[ ][ ]
				//Check to see if the diaginal piece is vacant, if it is set it as a potential move
				//Else if its ocupied and its the opposite colour it might be edible so call the FindEdibleCheckers function
				if(piece->GetGridLocation() == glm::vec2(gridPos.x - 1, gridPos.y - 1) && piece->GetOcupied() == false)
					piece->SetPotentialMove(true);
				else if(piece->GetGridLocation() == glm::vec2(gridPos.x - 1, gridPos.y - 1) && piece->GetOcupied() && !piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
					FindEdibleCheckers(glm::vec2(gridPos.x - 2, gridPos.y - 2));

				//[ ][ ]
				//[ ][0]
				//[X][ ]
				//Check to see if the diaginal piece is vacant, if it is set it as a potential move
				//Else if its ocupied and its the opposite colour it might be edible so call the FindEdibleCheckers function
				if(piece->GetGridLocation() == glm::vec2(gridPos.x + 1, gridPos.y - 1) && piece->GetOcupied() == false)
					piece->SetPotentialMove(true);
				else if(piece->GetGridLocation() == glm::vec2(gridPos.x + 1, gridPos.y - 1) && piece->GetOcupied() && !piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
					FindEdibleCheckers(glm::vec2(gridPos.x + 2, gridPos.y - 2));
			}
		}
		//---------------------------------------------------------------------------

		//Finding Potential moves for Red
		//---------------------------------------------------------------------------
		if(m_checkerSelected->GetChecker() != nullptr && m_checkerSelected->GetChecker()->IsBlack() != true)
		{
			//[X][ ]
			//[ ][0]
			//[ ][ ]
			//Check to see if the diaginal piece is vacant, if it is set it as a potential move
			//Else if its ocupied and its the opposite colour it might be edible so call the FindEdibleCheckers function
			if(piece->GetGridLocation() == glm::vec2(gridPos.x - 1, gridPos.y - 1) && piece->GetOcupied() == false)
				piece->SetPotentialMove(true);
			else if(piece->GetGridLocation() == glm::vec2(gridPos.x - 1, gridPos.y - 1) && piece->GetOcupied() && piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
				FindEdibleCheckers(glm::vec2(gridPos.x - 2, gridPos.y - 2));

			//[ ][ ]
			//[ ][0]
			//[X][ ]
			//Check to see if the diaginal piece is vacant, if it is set it as a potential move
			//Else if its ocupied and its the opposite colour it might be edible so call the FindEdibleCheckers function
			if(piece->GetGridLocation() == glm::vec2(gridPos.x + 1, gridPos.y - 1) && piece->GetOcupied() == false)
				piece->SetPotentialMove(true);
			else if(piece->GetGridLocation() == glm::vec2(gridPos.x + 1, gridPos.y - 1) && piece->GetOcupied() && piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
				FindEdibleCheckers(glm::vec2(gridPos.x + 2, gridPos.y - 2));
			
			
			//Have the Ability to move in every direction once a king
			if(m_checkerSelected->GetChecker()->IsKing())
			{
				//[ ][X]
				//[0][ ]
				//[ ][ ] 
				//Check to see if the diaginal piece is vacant, if it is set it as a potential move
				//Else if its ocupied and its the opposite colour it might be edible so call the FindEdibleCheckers function
				if(piece->GetGridLocation() == glm::vec2(gridPos.x - 1, gridPos.y + 1) && piece->GetOcupied() == false)
					piece->SetPotentialMove(true);
				else if(piece->GetGridLocation() == glm::vec2(gridPos.x - 1, gridPos.y + 1) && piece->GetOcupied() && piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
					FindEdibleCheckers(glm::vec2(gridPos.x - 2, gridPos.y + 2));//else find out if you can eat the piece there

				//[ ][ ]
				//[0][ ]
				//[ ][X]
				//Check to see if the diaginal piece is vacant, if it is set it as a potential move
				//Else if its ocupied and its the opposite colour it might be edible so call the FindEdibleCheckers function
				if(piece->GetGridLocation() == glm::vec2(gridPos.x + 1, gridPos.y + 1) && piece->GetOcupied() == false)
					piece->SetPotentialMove(true);
				else if(piece->GetGridLocation() == glm::vec2(gridPos.x + 1, gridPos.y + 1) && piece->GetOcupied() && piece->GetChecker()->IsBlack())//is true if the piece diagonal is ocupied. Means there is an edible checker
					FindEdibleCheckers(glm::vec2(gridPos.x + 2, gridPos.y + 2));
			}
		}
		//---------------------------------------------------------------------------
		CheckIfPieceRechedOtherEnd(piece);
	}
}

void Board::FindEdibleCheckers(glm::vec2 _gridPos)
{
	bool shouldReturn = false;

	for (BoardPiece* piece : m_boardPieces)
	{
		//Grid pos = one of the points around the selected checker
		//Set this position to a potential move if this board piece is vacant.
		//Else if it is vacant break from the loop because its not a potential move.
		//Else if its outside the grid return aswell.
		if(piece->GetGridLocation() == glm::vec2(_gridPos.x, _gridPos.y) && piece->GetOcupied() != true)
			piece->SetPotentialMove(true);
		else if(piece->GetGridLocation() == glm::vec2(_gridPos.x, _gridPos.y) && piece->GetOcupied() == true)
			break;
		else if (_gridPos.x <= -1 || _gridPos.y <= -1 || _gridPos.x >= 8 || _gridPos.y >= 7)
			break;
	}
}

void Board::DeselectingPotentialMoves()
{
	for (BoardPiece* bit : m_boardPieces)
		bit->SetPotentialMove(false);
}

void Board::FindEatenPiece(BoardPiece *_startLocation, BoardPiece *_endLocation)
{
	float x = _startLocation->GetGridLocation().x - _endLocation->GetGridLocation().x;
	float y = _startLocation->GetGridLocation().y - _endLocation->GetGridLocation().y; 

	//if the change on the x and y axis has changed 2 places meaning it has jumped something
	if(abs(x) == 2 && abs(y) == 2)
	{
		glm::vec2 eatenPieceGridLocataction = glm::vec2(0);
		
		//1 = start pos
		//2 = end pos
		//0 = the checker we need to kill

		if(x == 2 && y == -2)
		{
			//| | |2|
			//| |0| |
			//|1| | |
			eatenPieceGridLocataction = glm::vec2(_startLocation->GetGridLocation().x - 1, _startLocation->GetGridLocation().y + 1);
		}
		if(x == 2 && y == 2)
		{
			//|2| | |
			//| |0| |
			//| | |1|
			eatenPieceGridLocataction = glm::vec2(_startLocation->GetGridLocation().x - 1, _startLocation->GetGridLocation().y - 1);
		}
		if(x == -2 && y == -2)
		{
			//|1| | |
			//| |0| |
			//| | |2|
			eatenPieceGridLocataction = glm::vec2(_startLocation->GetGridLocation().x + 1, _startLocation->GetGridLocation().y + 1);
		}
		if(x == -2 && y == 2)
		{
			//| | |1|
			//| |0| |
			//|2| | |
			eatenPieceGridLocataction = glm::vec2(_startLocation->GetGridLocation().x + 1, _startLocation->GetGridLocation().y - 1);
		}

		//delete eaten checkers
		for (BoardPiece* bit : m_boardPieces)
			if(bit->GetGridLocation() == eatenPieceGridLocataction)
				bit->SetOcupied(NULL);
	}
}

void Board::CheckIfPieceRechedOtherEnd(BoardPiece* _piece)
{
	if(_piece->GetChecker() != nullptr && _piece->GetChecker()->IsBlack())//checking if the selected checker has a valid checker
	{
		if(_piece->GetGridLocation().y >=7 && !m_checkerSelected->GetChecker()->IsKing())
			_piece->GetChecker()->SetKing(true);
	}

	if(_piece->GetChecker() != nullptr && !_piece->GetChecker()->IsBlack())//checking if the selected checker has a valid checker
	{
		if(_piece->GetGridLocation().y <=0 && !m_checkerSelected->GetChecker()->IsKing())
			_piece->GetChecker()->SetKing(true);
	}
}