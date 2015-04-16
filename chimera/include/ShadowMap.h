#ifndef SHADOW_MAP_H_
#define SHADOW_MAP_H_

# include <LinearMath/btVector3.h>
# include "Texture.h"

namespace Chimera {

// These store our width and height for the shadow texture.  The higher the
// texture size the better quality shadow.  Must be power of two for most video cards.
# define SHADOW_WIDTH 512
# define SHADOW_HEIGHT 512

// This is the index into the g_Texture array that will hold our depth texture
# define SHADOW_ID   0

// The max textures we will use in our array
# define MAX_TEXTURES 1000


class ShadowMap {

public:
	ShadowMap();
	virtual ~ShadowMap();

	void RenderSceneA(void *pObjeto);
	void ApplyShadowMap(void *pObjeto);
	void StoreLightMatrices(const btVector3 &posicao);
	//void CreateRenderTexture(unsigned int textureArray[], int sizeX, int sizeY, int channels, int type, int textureID);
	void init(Node *_pScene);

private:

	Node *pScene;

	// The texture array where we store our image data
	//unsigned int g_Texture[MAX_TEXTURES];

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