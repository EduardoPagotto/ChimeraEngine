#ifndef __CHIMERA_STATE__HPP
#define __CHIMERA_STATE__HPP

#include "Light.h"
#include "OpenGLDefs.h"

namespace Chimera {

class State {
  public:
    State();
    ~State();

    void init();

    void setEnableLight(LightNum state, bool flag);
    void setEnablePolygonMode(PolygonMode state, FaceMaterial face);
    void setEnableSmooth(bool flag);
    void setEnableCullFace(CullFace state, bool flag);
    void setEnableColorMaterial(ColorMaterial state, bool flag);
    void setEnableClientState(ClientState state, bool flag);
    void setEnableLighting(Light* l, bool flag);
    void setEnableStateMachine(StateMachine _state, bool _flag);

    void apply();
    // void appyLighting();
    void applyWireFrame();

    Light* getLight() { return l; }

  private:
    void setStateMachine(StateMachine _state, bool _flag);
    void setEnable(GLenum face, bool flag);
    void setEnableClient(GLenum face, bool flag);

    Light* l;

    std::map<LightNum, bool> map_light;
    std::map<PolygonMode, FaceMaterial> map_polygonmode;
    std::map<CullFace, bool> map_cullface;
    std::map<ClientState, bool> map_clientstate;
    std::map<ColorMaterial, bool> map_colormaterial;
    std::map<Light*, bool> map_lighting;
    std::map<StateMachine, bool> map_stateMachine;
};
} // namespace Chimera

#endif
