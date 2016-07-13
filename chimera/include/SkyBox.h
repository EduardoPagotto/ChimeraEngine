#ifndef SKY_BOX__H
#define SKY_BOX__H

#include "Node.h"
#include "DrawTriMesh.h"
#include "Material.h"
#include "Texture.h"

namespace Chimera {
   
class SkyBox : public Node //FIXME mudar para Draw??
{

public:
   SkyBox (Node *_parent, std::string _name );
   virtual ~SkyBox();
   
   virtual void update ( DataMsg *_dataMsg );

   virtual void accept(class NodeVisitor* v);

   void render ( bool _texture );

   Draw *pDraw;
   Material *pMaterial;
   Texture *pTexture;
   
private:
   
   
};
}

# endif