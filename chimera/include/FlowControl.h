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
		virtual void open();
		virtual void close();
		void gameLoop();

	protected:
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void render() = 0;
		virtual void keyCapture(SDL_Keycode tecla) = 0;
		virtual void mouseButtonDownCapture(SDL_MouseButtonEvent mb) = 0;
		virtual void mouseButtonUpCapture(SDL_MouseButtonEvent mb) = 0;
		virtual void mouseMotionCapture(SDL_MouseMotionEvent mm) = 0;
		virtual void joystickCapture(JoystickManager &joy) = 0;
		virtual void joystickStatus(JoystickManager &joy) = 0;
		virtual void userEvent(const SDL_Event &_event) = 0;
		virtual void newFPS(const unsigned int &fps) = 0;
		virtual void processaGame();

		Video *pVideo;
		
	private:
		void countFrame();

		JoystickManager joystickManager;
		Timer timerFPS;
	};
}

# endif