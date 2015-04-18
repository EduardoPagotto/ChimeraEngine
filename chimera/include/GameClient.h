#ifndef __GAME_CLIENT_H
#define __GAME_CLIENT_H

#include "FlowControl.h"
#include "SceneMng.h"
#include "PhysicsControl.h"
#include "HUD.h"

namespace Chimera {

	class GameClient : public FlowControl {
	public:
		GameClient(Video *_pVideo, Chimera::SceneMng *_pScenMng);
		virtual ~GameClient();
		virtual void open();
		virtual void close();
		virtual void executeColisao(const KindOp &_kindOp, Node *_pNodeA, Node *_pNodeB) = 0;
		virtual void newFPS(const unsigned int &fps);

	protected:
		SceneMng *pSceneMng;
		PhysicsControl *physicWorld;
		Font *pFont;
		HUD *pHUD;

	private:
		void userEvent(const SDL_Event &_event);
		void validaOpColisao(const SDL_Event &_event);
		void processaGame();

		double deadzone;
		std::string textoFPS;
	};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
