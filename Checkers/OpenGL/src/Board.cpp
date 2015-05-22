#include "Board.h"
#include <iostream>

Board::Board()
{
	CreateBoard();
	m_checkerSelected = new BoardPiece();
	m_blueTurn = false;
	m_prevMoveColorWasBlack = false;
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

		for(unsigned int i = 0; i < rows; i++)
		{	
			for(unsigned int j = 0; j < cols; j++)
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

	//m_moreBlueMoves = false;
	//m_moreRedMoves = false;

	//Loops through all the board pieces and check for potential moves based iff the selected checker
	for (BoardPiece* piece : m_boardPieces)
	{
		//if(m_blueTurn)
		//	BlueMoves(piece);
		//
		//if(!m_blueTurn)
		//	RedMoves(piece);
		
		CheckForKings(piece);
	}
}

void Board::BlueMoves(BoardPiece* _pieceToMove)
{
	//Check For Any movoves
	if(CanBlueJump())
	{
		//Move only the pieces in the list that are moveable
	}
	else
	{
		float pieceToMoveRow = _pieceToMove->GetGridLocation().x;
		float pieceToMoveCol = _pieceToMove->GetGridLocation().y;
		//Search from normal moves based of the piece to move.
		for (BoardPiece* piece : m_boardPieces)
		{
			//If the current piece is ocupied it cant possibly be a potental move
			if(piece->GetOcupied())
				continue;

			float pieceRow = piece->GetGridLocation().x;
			float pieceCol = piece->GetGridLocation().y;
			
			//piece = bot right	of _pieceToMove
			if(pieceToMoveRow + 1 == pieceRow && pieceToMoveCol + 1 == pieceCol)
				piece->SetPotentialMove(true);
			
			//piece = top right of _pieceToMove
			if(pieceToMoveRow - 1 == pieceRow && pieceToMoveCol + 1 == pieceCol)
				piece->SetPotentialMove(true);
			
			//ABILITY TO MOVE BACKWARDS IF KING
			if(_pieceToMove->GetChecker()->IsKing())
			{
				//piece = bot left	of _pieceToMove
				if(pieceToMoveRow + 1 == pieceRow && pieceToMoveCol - 1 == pieceCol)
					piece->SetPotentialMove(true);
				
				//piece = top left of _pieceToMove
				if(pieceToMoveRow - 1 == pieceRow && pieceToMoveCol - 1 == pieceCol)
					piece->SetPotentialMove(true);
			}
		}
	}
}
void Board::RedMoves(BoardPiece* _pieceToMove)
{
	//Check For Any movoves
	if(CanRedJump())
	{
		//Move only the pieces in the list that are moveable
	}
	else
	{
		float pieceToMoveRow = _pieceToMove->GetGridLocation().x;
		float pieceToMoveCol = _pieceToMove->GetGridLocation().y;
		//Search from normal moves based of the piece to move.
		for (BoardPiece* piece : m_boardPieces)
		{
			//If the current piece is ocupied it cant possibly be a potental move
			if(piece->GetOcupied())
				continue;

			float pieceRow = piece->GetGridLocation().x;
			float pieceCol = piece->GetGridLocation().y;
			
			//piece = bot left	of _pieceToMove
			if(pieceToMoveRow + 1 == pieceRow && pieceToMoveCol - 1 == pieceCol)
				piece->SetPotentialMove(true);
				
			//piece = top left of _pieceToMove
			if(pieceToMoveRow - 1 == pieceRow && pieceToMoveCol - 1 == pieceCol)
				piece->SetPotentialMove(true);

			//ABILITY TO MOVE BACKWARDS IF KING
			if(_pieceToMove->GetChecker()->IsKing())
			{
				//piece = bot right	of _pieceToMove
				if(pieceToMoveRow + 1 == pieceRow && pieceToMoveCol + 1 == pieceCol)
					piece->SetPotentialMove(true);
			
				//piece = top right of _pieceToMove
				if(pieceToMoveRow - 1 == pieceRow && pieceToMoveCol + 1 == pieceCol)
					piece->SetPotentialMove(true);
			}
		}
	}
}

void Board::FindEdibleCheckers(glm::vec2 _gridPos)
{
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
		//checking for more moves if the player has made a jump
		CheckForMoves(_endLocation);

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

void Board::CheckForKings(BoardPiece* _piece)
{
	if(_piece->GetChecker() != nullptr && _piece->GetChecker()->IsBlack())//checking if the selected checker has a valid checker
		if(_piece->GetGridLocation().y >=7 && !m_checkerSelected->GetChecker()->IsKing())
			_piece->GetChecker()->SetKing(true);

	if(_piece->GetChecker() != nullptr && !_piece->GetChecker()->IsBlack())//checking if the selected checker has a valid checker
		if(_piece->GetGridLocation().y <=0 && !m_checkerSelected->GetChecker()->IsKing())
			_piece->GetChecker()->SetKing(true);
}

bool Board::CanBlueJump()
{
	m_blueCanJump = false;
	for (BoardPiece* piece : m_boardPieces)
	{
		//if the piece being searched isnt ocupied or it has a red piece on it break.
		if(!piece->GetOcupied() || !piece->GetChecker()->IsBlack())
			continue;

		float pieceRow = piece->GetGridLocation().x;
		float pieceCol = piece->GetGridLocation().y;

		for (BoardPiece* diagonalPiece : m_boardPieces)
		{
			//if the diagonal piece is black just break because u cant jump it
			if (diagonalPiece->GetOcupied())
				if(diagonalPiece->GetChecker()->IsBlack())
					continue;

			float diagonalPieceRow = diagonalPiece->GetGridLocation().x;
			float diagonalPieceCol = diagonalPiece->GetGridLocation().y;
			
			int rowOffset = NULL;
			int colOffset = NULL;

			//if the diagonalpiece == the spot diagonal of the piece and its ocupied, 
			//set the offsets for when you search for edible checkers at the end
			if(pieceRow + 1 == diagonalPieceRow && pieceCol + 1 == diagonalPieceCol && diagonalPiece->GetOcupied())
			{
				//| | | |
				//| |X| |
				//| | |?| ? = diagonalPiece.
				rowOffset = +1;
				colOffset = +1;
			}

			if(pieceRow - 1 == diagonalPieceRow && pieceCol + 1 == diagonalPieceCol && diagonalPiece->GetOcupied())
			{
				//| | |?|
				//| |X| |
				//| | | | ? = diagonalPiece.
				rowOffset = -1;
				colOffset = +1;
			}
			
			//if its a king it can move backwards
			if(piece->GetChecker()->IsKing())
			{
				if(pieceRow - 1 == diagonalPieceRow && pieceCol - 1 == diagonalPieceCol && diagonalPiece->GetOcupied())
				{
					//|?| | |
					//| |X| |
					//| | | | ? = diagonalPiece.
					rowOffset = -1;
					colOffset = -1;
				}
				if(pieceRow + 1 == diagonalPieceRow && pieceCol - 1 == diagonalPieceCol && diagonalPiece->GetOcupied())
				{
					//| | | |
					//| |X| |
					//|?| | | ? = diagonalPiece.
					rowOffset = +1;
					colOffset = -1;
				}
			}

			//Search for potential moves based off the offsets
			if(rowOffset != NULL && colOffset != NULL)
			{
				glm::vec2 gridPos = glm::vec2(diagonalPieceRow + rowOffset, diagonalPieceCol + colOffset);

				//find the place ur jumping to
				for (BoardPiece* itr : m_boardPieces)
				{
					if (gridPos.x <= -1 || gridPos.y <= -1 || gridPos.x >= 8 || gridPos.y >= 8)
					break;

					//Grid pos = one of the points around the selected checker
					//Set this position to a potential move if this board piece is vacant.
					//Else if it is ocupied break from the loop because its not a potential move.
					if(itr->GetGridLocation() == glm::vec2(gridPos.x, gridPos.y) && !itr->GetOcupied())
					{
						m_blueCanJump = true;
						itr->SetPotentialMove(true);

						//push back the piece that has a valid jump.
						m_mustMoveThese.push_back(&(*piece));
						break;
					}
					else if(itr->GetGridLocation() == glm::vec2(gridPos.x, gridPos.y) && itr->GetOcupied())
						break;
				}
			}
		}
	}
	return m_blueCanJump;
}

bool Board::CanRedJump()
{
	m_redCanJump = false;
	for (BoardPiece* piece : m_boardPieces)
	{
		//if the piece being searched isnt ocupied or it has a black piece on it continue.
		if(!piece->GetOcupied() || piece->GetChecker()->IsBlack())
			continue;

		float pieceRow = piece->GetGridLocation().x;
		float pieceCol = piece->GetGridLocation().y;

		for (BoardPiece* diagonalPiece : m_boardPieces)
		{
			//if the diagonal piece is Red just break because u cant jump it
			if (diagonalPiece->GetOcupied())
				if(!diagonalPiece->GetChecker()->IsBlack())
					continue;

			float diagonalPieceRow = diagonalPiece->GetGridLocation().x;
			float diagonalPieceCol = diagonalPiece->GetGridLocation().y;
			
			int rowOffset = NULL;
			int colOffset = NULL;

			//if the diagonalpiece == the spot diagonal of the piece and its ocupied, 
			//set the offsets for when you search for edible checkers at the end
			if(pieceRow - 1 == diagonalPieceRow && pieceCol - 1 == diagonalPieceCol && diagonalPiece->GetOcupied())
			{
				//|?| | |
				//| |X| |
				//| | | | ? = diagonalPiece.
				rowOffset = -1;
				colOffset = -1;
			}
			if(pieceRow + 1 == diagonalPieceRow && pieceCol - 1 == diagonalPieceCol && diagonalPiece->GetOcupied())
			{
				//| | | |
				//| |X| |
				//|?| | | ? = diagonalPiece.
				rowOffset = +1;
				colOffset = -1;
			}
			
			//if its a king it can move backwards
			if(piece->GetChecker()->IsKing())
			{
				if(pieceRow + 1 == diagonalPieceRow && pieceCol + 1 == diagonalPieceCol && diagonalPiece->GetOcupied())
				{
					//| | | |
					//| |X| |
					//| | |?| ? = diagonalPiece.
					rowOffset = +1;
					colOffset = +1;
				}

				if(pieceRow - 1 == diagonalPieceRow && pieceCol + 1 == diagonalPieceCol && diagonalPiece->GetOcupied())
				{
					//| | |?|
					//| |X| |
					//| | | | ? = diagonalPiece.
					rowOffset = -1;
					colOffset = +1;
				}
			}

			//Search for potential moves based off the offsets
			if(rowOffset != NULL && colOffset != NULL)
			{
				glm::vec2 gridPos = glm::vec2(diagonalPieceRow + rowOffset, diagonalPieceCol + colOffset);

				//find the place ur jumping to
				for (BoardPiece* itr : m_boardPieces)
				{
					if (gridPos.x <= -1 || gridPos.y <= -1 || gridPos.x >= 8 || gridPos.y >= 8)
					break;

					//Grid pos = one of the points around the selected checker
					//Set this position to a potential move if this board piece is vacant.
					//Else if it is ocupied break from the loop because its not a potential move.
					if(itr->GetGridLocation() == glm::vec2(gridPos.x, gridPos.y) && !itr->GetOcupied())
					{
						m_redCanJump = true;
						itr->SetPotentialMove(true);

						//push back the piece that has a valid jump.
						m_mustMoveThese.push_back(&(*piece));
						break;
					}
					else if(itr->GetGridLocation() == glm::vec2(gridPos.x, gridPos.y) && itr->GetOcupied())
						break;
				}
			}
		}
	}
	return m_redCanJump;
}