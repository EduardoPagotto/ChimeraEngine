#ifndef _STATE__H
#define _STATE__H

#include "Material.h"
#include "Light.h"
#include "Shader.h"

namespace Chimera {

class State{
public:
    State();
    ~State();

	void init();

     void setEnableLight(LightNum state,bool flag);
     void setEnablePolygonMode(PolygonMode state,FaceMaterial face);
     void setEnableSmooth(bool flag);
     void setEnableCullFace(CullFace state,bool flag);
     void setEnableColorMaterial(ColorMaterial state,bool flag);
     void setEnableClientState(ClientState state,bool flag);
     
	 void setMaterial(Material* _pMat);

     void setEnableLighting(Light* l,bool flag);
     
     void setEnableStateMachine(StateMachine _state, bool _flag);
     
     void appyMaterial(Shader *_pShader);

     void appyLighting();
     
     void applyWireFrame();
     void apply();
    
 	 Light* getLight() {
 		 return l;
 	 }
 
	 Material* getMaterial() {
		 return pMaterialAtivo;
	 }
	 
private:
     void setStateMachine(StateMachine _state, bool _flag) ;
     void setEnable(GLenum face,bool flag);
     void setEnableClient(GLenum face,bool flag);
    
     std::map<LightNum, bool> map_light;
     std::map<PolygonMode, FaceMaterial> map_polygonmode;
     std::map<CullFace,bool> map_cullface;
     std::map<ClientState,bool> map_clientstate;
     std::map<ColorMaterial,bool> map_colormaterial;

	 Material *pMaterialAtivo;
    
     std::map<Light*,bool> map_lighting;
 
     std::map<StateMachine,bool> map_stateMachine;
          
     Light* l;
};
}

#endif	/* STATE_H */
