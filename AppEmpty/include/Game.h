#ifndef __GAME_EVENTS_TEST__H_
# define __GAME_EVENTS_TEST__H_

#include "GameClient.h"

class Game : public Chimera::GameClient
{
public:
	Game(Chimera::SceneMng *_pScenMng);
	virtual ~Game();

	virtual void start();
	virtual void stop();
	virtual void render();
	virtual void keyCapture(SDL_Keycode tecla);
	virtual void mouseButtonDownCapture(SDL_MouseButtonEvent mb);
	virtual void mouseButtonUpCapture(SDL_MouseButtonEvent mb);
	virtual void mouseMotionCapture(SDL_MouseMotionEvent mm);
	virtual void joystickCapture(Chimera::JoystickManager &joy);
	virtual void joystickStatus(Chimera::JoystickManager &joy);
	virtual void executeColisao(const Chimera::KindOp &_kindOp, Chimera::Node *_pNodeA, Chimera::Node *_pNodeB);
	virtual void newFPS(const unsigned int &fps);

private:
	int botaoIndex;
	int estadoBotao;
};

#endif