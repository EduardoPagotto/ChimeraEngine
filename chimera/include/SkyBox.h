#ifndef SKY_BOX__H
#define SKY_BOX__H

#include "DrawTriMesh.h"

namespace Chimera {

	class SkyBox : Node  {

	public:
		SkyBox(std::string _id, std::string _name);
		virtual ~SkyBox();
		virtual void update(DataMsg *_dataMsg);
  	private:
		Draw *pDraw;
	};

}

# endif