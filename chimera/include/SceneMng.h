#ifndef __SCENE_MNG_H
#define __SCENE_MNG_H

#include <vector>

#include "Video.h"
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
    SceneMng();
    virtual ~SceneMng() noexcept;

    virtual void accept(class NodeVisitor* v);

    void init();
	void draw(Video *_pVideo);

    inline void origemDesenho (Coord *_pCoord ) {
		pOrigem = _pCoord;
    }

private:
	Coord *pOrigem;
    RenderVisitor rv;
};
} /* namespace Chimera */
#endif /* GRADE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
