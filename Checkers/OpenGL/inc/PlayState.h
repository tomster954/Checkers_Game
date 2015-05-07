//-----------------------------------------------------------------------------
//Description:: this is the play state. where the main game runs
//Author:: Tommas Solarino
//-----------------------------------------------------------------------------
#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include "Camera.h"
#include "Board.h"
#include <vector>

struct GLFWwindow;

class PlayState
{
public:
	PlayState(GLFWwindow *_pWindow);
	~PlayState();
	void Update();
	void Draw(Camera *_camera);
	void SelectingCheckers();

	void CheckingForMoves(glm::vec2 _gridPos);

private:
	GLFWwindow *m_pWindow;
	Board *m_Board;
	BoardPiece* m_pieceToMove;

	bool m_keyPressed;

	int m_selectedRow;
	int m_selectedCol;

};

#endif