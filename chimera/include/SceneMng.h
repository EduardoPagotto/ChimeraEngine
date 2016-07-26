#ifndef SCENE_MNG_H_
#define SCENE_MNG_H_

#include <vector>

#include "Video.h"
#include "Camera.h"
#include "Light.h"
#include "ParticleEmitter.h"

#include "ShadowMap.h"
#include "SceneRoot.h"
#include "LoaderDae.h"

#include "Coord.h"

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

    Node *getNode ( EntityKind _type, const std::string &_nome );
    Node *getNode ( EntityKind _type, unsigned index );

    void initNodes(Node* u, InitVisitor *pVisit);
    void init();
    
    void setReader( LoaderDae *_pLoader );
    
    Group* createSceneGraph();
    
    inline void cameraAtiva ( Camera *_pCam ) {
        pCameraAtiva = _pCam;
    }

    inline void objetoAtivo ( Solid *_pObject ) {
		pOrigemDesenho = _pObject;
    }

	SceneRoot *getRoot() {
		return root;
	}

    void draw ();

    Video* getVideo() {
        return pVideo;
    }
    
private:
    void DFS(Node* u);

    void parseEntity ( Node *_pNode );
    void addEntityToScene ( Node *_pNode );

    LoaderDae *pLoader;
    
    SceneRoot *root;
    Camera *pCameraAtiva;
	Solid *pOrigemDesenho;

    std::vector<Camera*> m_vCamera;
    std::vector<Light*> m_vLight;

    ShadowMap shadoMap;
    Video *pVideo;
    
    RenderVisitor rv;
};

} /* namespace Chimera */
#endif /* GRADE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
