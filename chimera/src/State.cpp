#include "State.h"

/*
#define FORI(x,j) \
    for(int x=0;x<j;x++)*/

namespace Chimera {

State::State() {

	m = nullptr;
	t = nullptr;
	l = nullptr;
}

State::~State() { 
}

void State::setEnable(GLenum face,bool flag) {

    if (flag) 
		glEnable(face);
    else
        glDisable(face);
}

void State::setEnableClient(GLenum face,bool flag) {

    if(flag) 
		glEnableClientState(face);
    else{
        glDisableClientState(face);
    }
}

void State::setEnableLight(LightNum state,bool flag) {
    //setEnable(state,flag);
    this->map_light[state]=flag;
}

void State::setEnablePolygonMode(PolygonMode state,FaceMaterial face) {    
    this->map_polygonmode[state]=face;
}

void State::setEnableSmooth(bool flag) {
    
    if(flag) 
        glShadeModel(GL_SMOOTH);
    else{
        glShadeModel(GL_FLAT);
    }
}

void State::setEnableCullFace(CullFace state,bool flag) {
    //setEnable(state,flag);
    this->map_cullface[state]=flag;
}

void State::setEnableClientState(ClientState state,bool flag) {
    this->map_clientstate[state]=flag;
}

void State::setEnableColorMaterial(ColorMaterial state,bool flag) {
    //setEnable(state,flag);
   
    this->map_colormaterial[state]=flag;
}

void State::setEnableMaterial(Material* m,bool flag) {

    this->m=m;
    this->map_material[m]=flag;
}

void State::setEnableTexture(Texture* t,bool flag) {

    this->t=t;
    this->map_texture[t]=flag;
}

void State::setEnableLighting(Light* l,bool flag) {

    this->l=l;
    this->map_lighting[l]=flag;
}

void State::setEnableStateMachine(StateMachine _state, bool _flag) {
 
    this->map_stateMachine[_state] = _flag;
}

void State::setStateMachine(StateMachine _state, bool _flag) {
    
    if (_flag) 
        glEnable(_state);
    else
        glDisable(_state);
}

void State::apply() {

    using namespace std;
    
    for(map<StateMachine,bool>::iterator iter = this->map_stateMachine.begin(); iter != map_stateMachine.end(); ++iter)
        setStateMachine(iter->first, iter->second);
    
    for(map<LightNum,bool>::iterator iter = this->map_light.begin(); iter != this->map_light.end(); ++iter)
		setEnable(iter->first, iter->second);
    
    for(map<PolygonMode,FaceMaterial>::iterator iter = this->map_polygonmode.begin(); iter != this->map_polygonmode.end(); ++iter)
        glPolygonMode((GLenum)iter->second,(GLenum)iter->first); //TODO Testar se é isso mesmo!!
    
    for(map<CullFace,bool>::iterator iter = this->map_cullface.begin(); iter != this->map_cullface.end(); ++iter) 
		setEnable(iter->first, iter->second);
    
    for(map<ClientState,bool>::iterator iter = this->map_clientstate.begin(); iter != this->map_clientstate.end(); ++iter)
		setEnableClient(iter->first, iter->second);
    
    for(map<ColorMaterial,bool>::iterator iter = this->map_colormaterial.begin(); iter != this->map_colormaterial.end(); ++iter) 
        setEnable(iter->first, iter->second);
}

void State::applyWireFrame() {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

void State::appyMaterial() {
    
    using namespace std;
    
    //TODO implementar
     for(map<Material*,bool>::iterator iter = this->map_material.begin(); iter != this->map_material.end(); ++iter){
        Material* k= iter->first;
        bool flag=iter->second;
		if (flag)
			k->apply();
     }
}

void State::appyTexture(){
 
    glEnable(GL_TEXTURE_2D);//FIXME preciso??????
 
    using namespace std;
    
    for(map<Texture*,bool>::iterator iter = this->map_texture.begin(); iter != this->map_texture.end(); ++iter){
        Texture* k= iter->first;
        bool flag=iter->second;
                
        if(flag) {
                    
            if(k->textureLoad() == false)
                k->init();
                        
            k->apply();
        }
            
    }
    
//     for(map<Texture*,bool>::iterator iter = this->map_texture.begin(); iter != this->map_texture.end(); ++iter){
//                 Texture* k= iter->first;
//                 bool flag=iter->second;
//                 if(flag){
//                     if(k->hasTexture()){
//                         GLuint textureID;
//                         const unsigned char* data=k->getTImageData();
//                         int width=k->getTImageWidth();
//                         int height=k->getTImageHeight();
//                         k->createTexture(1,&textureID);
//                         k->BindTexture(GL_TEXTURE_2D,&textureID);
//                         k->applyGLTexture2D(GL_TEXTURE_2D , 0 , GL_RGB , 
//                             width , height , 0, GL_BGR , GL_UNSIGNED_BYTE , 
//                             data);
//                         
//                         glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT);
//                         glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP);
// 
//                         
//   
//                     }
//                 }
//                     
//     }
}

void State::appyLighting(){
    
    using namespace std;
    for(map<Light*,bool>::iterator iter = this->map_lighting.begin(); iter != this->map_lighting.end(); ++iter){
		Light* k= iter->first;
		bool flag=iter->second;
		if(flag) {

			k->apply(LIGHT0);
		}
    }
}
}