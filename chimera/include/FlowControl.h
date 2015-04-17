#ifndef __FLOW_CONTROL_H
#define __FLOW_CONTROL_H

# include "Video.h"
# include "JoystickManager.h"
# include "Timer.h"

namespace Chimera {

class FlowControl {

public:
	FlowControl(Video *_pVideo);
	virtual ~FlowControl();

	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void render() = 0;
	virtual void keyCapture(SDL_Keycode tecla) = 0;
	virtual void mouseButtonDownCapture(SDL_MouseButtonEvent mb) = 0;
	virtual void mouseButtonUpCapture(SDL_MouseButtonEvent mb) = 0;
	virtual void mouseMotionCapture(SDL_MouseMotionEvent mm) = 0;
	virtual void joystickCapture(JoystickManager &joy) = 0;
	virtual void eventoUsuario(const SDL_Event &_event) = 0;

	virtual void open();
	virtual void close();
	virtual void gameLoop();

	virtual void processaGame();


protected:
	Video *pVideo;
	JoystickManager joystickManager;

private:

	bool newFPS();
	void countFrame();

	//double deadzone;
	unsigned int fps;
	Timer timerFPS;
	Timer timerSegundo;

};


}

# endif