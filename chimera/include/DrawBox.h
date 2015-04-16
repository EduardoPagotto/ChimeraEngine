#ifndef DRAW_BOX_H_
#define DRAW_BOX_H_

#include "Draw.h"

namespace Chimera {

	class DrawBox : public Draw {
	public:
		DrawBox(std::string _id, std::string _name);
		DrawBox(const DrawBox &_draw);
		virtual ~DrawBox();

		void setSizeBox(const btVector3 &_sizeBox) {
			sizeBox = _sizeBox;
		}

		virtual void update(DataMsg *dataMsg);

		virtual void clone(Node **ppNode);

		void init();

		virtual void renderExecute(bool _texture);
		virtual btVector3 getSizeBox(void);

	private:
		btVector3 sizeBox;

	};

}

#endif
