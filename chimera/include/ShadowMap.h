#ifndef SHADOW_MAP_H_
#define SHADOW_MAP_H_

# include <LinearMath/btVector3.h>
# include "Texture.h"
#include "Node.h"

namespace Chimera
{

class ShadowMap
{

public:
    ShadowMap();
    virtual ~ShadowMap();

    //void RenderSceneA ( void *pObjeto );
    //void ApplyShadowMap ( void *pObjeto );
    void StoreLightMatrices ( const btVector3 &posicao );
    void init ( Node *_pScene );

	void initSceneShadow();
	void endSceneShadow();

	void initApplyShadow();
	void endApplyShadow();

private:

    Node *pScene;

    // These arrays will store our 4x4 matrices for the light's
    // project and modelview matrix.  These will then be loaded
    // into the texture matrix for the shadow mapping.
    float g_mProjection[16];
    float g_mModelView[16];

    // We set the light's view position at the origin
    btVector3 g_LightView;

    Texture *pTexture;
};

}

# endif