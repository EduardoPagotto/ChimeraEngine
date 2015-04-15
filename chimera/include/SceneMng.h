#ifndef SCENE_MNG_H_
#define SCENE_MNG_H_

#include <Video.h>

#include <vector>
#include "ExceptionSDL.h"
#include "Camera.h"
#include "Object.h"
#include "Light.h"
#include "ParticleEmitter.h"
#include "SkyBox.h"

// These store our width and height for the shadow texture.  The higher the
// texture size the better quality shadow.  Must be power of two for most video cards.
#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

// This is the index into the g_Texture array that will hold our depth texture
#define SHADOW_ID   0

// The max textures we will use in our array
#define MAX_TEXTURES 1000

namespace Chimera {

	/**
	 * Class SceneMng
	 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
	 *  @since 20130925
	 */
	class SceneMng {
	public:
		SceneMng(Node *_pRoot);
		virtual ~SceneMng();

		void addChildToScene(Node *_pNode);

        Node *getNode(EntityKind _type, const std::string &_nome);
		Node *getNode(EntityKind _type, unsigned index);

		void update(DataMsg *dataMsg);

		Node* getRoot() {
			return pRoot;
		}

		void cameraAtiva(Camera *_pCam) {
			pCameraAtiva = _pCam;
		}

		void objetoAtivo(Object *_pObject) {
			pObjeto = _pObject;
		}

		void skyBoxAtivo(SkyBox *_pSkyBox) {
            pSkyBox = _pSkyBox;
        }

		//void draw3d();

        void RenderSceneA();
        void ApplyShadowMap();

        void execLight();

	private:

        void StoreLightMatrices(const btVector3 &posicao);
        void CreateRenderTexture(unsigned int textureArray[], int sizeX, int sizeY, int channels, int type, int textureID);

        void init();


		void parseEntity(Node *_pNode);
		void addEntityToScene(Node *_pNode);

		bool hasLight;
		bool hasMaterial;

		Node *pRoot;
		Camera *pCameraAtiva;
		Object *pObjeto;
        SkyBox *pSkyBox;

		std::vector<Camera*> m_vCamera;
		std::vector<Light*> m_vLight;
		std::vector<Object*> m_vObject;
        std::vector<ParticleEmitter*> m_vParticle;
        std::vector<SkyBox*> m_vSkyBox;


        // The texture array where we store our image data
        unsigned int g_Texture[MAX_TEXTURES];

        // These arrays will store our 4x4 matrices for the light's
        // project and modelview matrix.  These will then be loaded
        // into the texture matrix for the shadow mapping.
        float g_mProjection[16];
        float g_mModelView[16];

        // We set the light's view position at the origin
        btVector3 g_LightView;

	};

} /* namespace Chimera */
#endif /* GRADE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
