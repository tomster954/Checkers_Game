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

