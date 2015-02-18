#ifndef __GAME_CLIENT_H
#define __GAME_CLIENT_H

//#include <log4cxx/logger.h>

#include "Timer.h"
#include "Video.h"
#include "SceneMng.h"
#include "PhysicsControl.h"
#include "HUD.h"

namespace Chimera {

	class GameClient {
	public:
		GameClient(Video *_pVideo, Chimera::SceneMng *_pScenMng);
		virtual ~GameClient();

		void open();
		void close();
		void gameLoop();

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void render() = 0;
		virtual void keyCapture(SDL_Keycode tecla) = 0;
		virtual void mouseButtonDownCapture(SDL_MouseButtonEvent mb) = 0;
		virtual void mouseButtonUpCapture(SDL_MouseButtonEvent mb) = 0;
		virtual void mouseMotionCapture(SDL_MouseMotionEvent mm) = 0;
		virtual void executeColisao(const KindOp &_kindOp, Node *_pNodeA, Node *_pNodeB) = 0;
		virtual void userEvent(const SDL_Event &_event) = 0;

	protected:
		SceneMng *pSceneMng;
		PhysicsControl *physicWorld;
		Font *pFont;
		HUD *pHUD;
		Video *pVideo;

	private:
		bool newFPS();
		void countFrame();
		void processaGame();
		void validaOpColisao(const SDL_Event &_event);

		unsigned int fps;
		std::string textoFPS;
		Timer timerFPS;
		Timer timerSegundo;

		//log4cxx::LoggerPtr logger;
	};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
