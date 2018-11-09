#ifndef __SCENE_MNG_H
#define __SCENE_MNG_H

#include <vector>

#include "Video.h"
#include "Camera.h"
#include "ParticleEmitter.h"
#include "RenderVisitor.h"

namespace Chimera
{

/**
 * Class SceneMng
 *  @author <a href="mailto:edupagotto@gmail.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class SceneMng : public Node
{
public:
    SceneMng(Video *_pVideo);
    virtual ~SceneMng() noexcept;

    virtual void accept(class NodeVisitor* v);

    void init();
	void draw();

    inline void cameraAtiva ( Camera *_pCam ) {
        pCameraAtiva = _pCam;
    }

    inline Camera* getCamere() {
        return pCameraAtiva;
    }

    inline void origemDesenho (Coord *_pCoord ) {
		pOrigem = _pCoord;
    }

    Video* getVideo() {
        return pVideo;
    }

private:
    Camera *pCameraAtiva;
	Coord *pOrigem;
    Video *pVideo;
    RenderVisitor rv;
};
} /* namespace Chimera */
#endif /* GRADE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
