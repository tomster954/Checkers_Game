//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Descriptio: The main application
//-----------------------------------------------------------------------------

#ifndef APPLICATION
#define APPLICATION

//Includes
#include "PlayState.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;

struct GLFWwindow;

class Camera;
class Application
{
public:
	//Constructor
	Application(vec4 _backgroundColour);
	//Destructor
	~Application();

	void RunGame();

	void Update();
	void Draw();
	void ResetGame();

protected:

private:
	GLFWwindow *m_pWindow;
	Camera *m_camera;
	PlayState *m_playState;

	bool m_gameOver;

	float	m_lastTime;
	float	m_currentTime;
	float	m_deltaTime;
};

#endif
