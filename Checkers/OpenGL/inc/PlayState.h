//-----------------------------------------------------------------------------
//Description:: this is the play state. where the main game runs
//Author:: Tommas Solarino
//-----------------------------------------------------------------------------
#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include <vector>

#include "Networking.h"
#include "Camera.h"
#include "Board.h"

struct GLFWwindow;

class PlayState
{
public:
	PlayState(GLFWwindow *_pWindow);
	~PlayState();
	void Update();
	void Draw(Camera *_camera);
	void SelectingCheckers();
	void MoveChecker(glm::vec2 _piecePos);

	void CheckForGameOver();

	bool IsGameOver(){ return m_gameOver; }
private:
	GLFWwindow *m_pWindow;
	Board *m_Board;
	BoardPiece* m_pieceToMove;

	Networking* m_network;

	glm::mat4* m_project;

	bool m_keyPressed;
	bool m_bluesTurn;
	bool m_gameOver;

	int m_selectedRow;
	int m_selectedCol;

};

#endif