#ifndef SCENE_MNG_H_
#define SCENE_MNG_H_

#include <vector>

#include "Video.h"
#include "Camera.h"
//#include "Light.h"
#include "ParticleEmitter.h"

#include "ShadowMap.h"
#include "SceneRoot.h"
#include "LoaderDae.h"

#include "InitVisitor.h"
#include "RenderVisitor.h"

namespace Chimera
{

/**
 * Class SceneMng
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class SceneMng
{
public:
    SceneMng ( Video *_pVideo );
    virtual ~SceneMng();

    void init();
	void draw();
    void setReader( LoaderDae *_pLoader );
    Group* createSceneGraph();
    
    inline void cameraAtiva ( Camera *_pCam ) {
        pCameraAtiva = _pCam;
    }

    inline void origemDesenho (Coord *_pCoord ) {
		pOrigem = _pCoord;
    }

	SceneRoot *getRoot() {
		return root;
	}

    Video* getVideo() {
        return pVideo;
    }
    
private:
	void initNodes(Node* u, InitVisitor *pVisit);
    void DFS(Node* u);

    LoaderDae *pLoader;
    SceneRoot *root;
    Camera *pCameraAtiva;
	Coord *pOrigem;
    Video *pVideo;
    
	ShadowMap shadoMap;
    RenderVisitor rv;
};

} /* namespace Chimera */
#endif /* GRADE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
