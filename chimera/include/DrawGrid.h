#ifndef DRAW_GRID_H_
#define DRAW_GRID_H_

#include "Color.h"
#include "Draw.h"

namespace Chimera {

	class DrawGrid : public Draw {
	public:
		DrawGrid(std::string _id, std::string _name);
		DrawGrid(const DrawGrid &_draw);
		virtual ~DrawGrid();

		void setSizeBox(const btVector3 &_sizeBox) {
			sizeBox = _sizeBox;
		}

		void setSpace(btScalar _space) {
			space = _space;
		}

		void setColorX(Color _color) {
			colorX = _color;
		}

		void setColorY(Color _color) {
			colorY = _color;
		}

		void setColorZ(Color _color) {
			colorZ = _color;
		}

		virtual void update(DataMsg *dataMsg);

		virtual void clone(Node **ppNode);

		virtual void init();
		virtual void render();
		virtual btVector3 getSizeBox(void);


	private:
		btVector3 sizeBox;
		btScalar space;

		Color colorX;
		Color colorY;
		Color colorZ;

	};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
