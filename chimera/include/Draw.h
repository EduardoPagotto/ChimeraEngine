#ifndef DRAW_H_
#define DRAW_H_

#include <LinearMath/btVector3.h>

#include "Material.h"

namespace Chimera {

	enum class DrawType {
		BOX,
		GRID,
		SPHERE,
		MESH
	};

	class Draw : public Node {
	public:
		Draw(DrawType _type, std::string _id, std::string _name);
		Draw(const Draw &_draw);

		virtual ~Draw();

		virtual void update(DataMsg *dataMsg);

		virtual void clone(Node **ppNode) {
			Node::clone(ppNode);
		}

		void render();

		void init();

        virtual void renderExecute() = 0;

		virtual btVector3 getSizeBox() = 0;

		DrawType getType() const {
			return type;
		}

	protected:
		DrawType type;
        Material *pMaterial;
	};

}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
