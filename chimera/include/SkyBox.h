#ifndef SKY_BOX__H
#define SKY_BOX__H

#include "Node.h"
#include "DrawTriMesh.h"
#include "Material.h"

namespace Chimera {
namespace Graph {
   
class SkyBox : public Node //FIXME mudar para Draw??
{

public:
   SkyBox ( std::string _id, std::string _name );
   virtual ~SkyBox();
   
   virtual void update ( DataMsg *_dataMsg );

   void render ( bool _texture );

   Draw *pDraw;
   Material *pMaterial;
   
private:
   
   
};
}
}

# endif