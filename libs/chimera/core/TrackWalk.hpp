#ifndef __CHIMERA_TRACK_WALK__HPP
#define __CHIMERA_TRACK_WALK__HPP

#include "chimera/core/ViewPoint.hpp"

namespace Chimera {

enum class WalkType { Land, Air };

class TrackWalk {
  public:
    TrackWalk();
    virtual ~TrackWalk();

    virtual void init(ViewPoint* _vp, WalkType _type);
    virtual void walking();

    void pitch(float _theta);
    void yaw(float _theta);
    void roll(float _theta);
    void walk(float _delta, bool _wall[4]);
    void strafe(float _delta, bool _wall[4]);
    void fly(float _delta, bool _wall[4]);
    void reset();

  private:
    glm::vec3 along;
    glm::vec3 up;
    glm::vec3 forward;
    ViewPoint* pVp;
    WalkType type;
};
} // namespace Chimera
#endif