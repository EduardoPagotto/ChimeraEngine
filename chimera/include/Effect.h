#ifndef EFFECT_H_
#define EFFECT_H_

// #include <tinyxml2.h>
// 
// #include "Color.h"
// #include "Entity.h"
// 
// #include "OpenGLDefs.h"
// #include <map>
// 
// // #ifdef WIN32
// // #include "windows.h"
// // #endif
// // 
// // #include <map>
// // #include <GL/gl.h>
// 
// namespace Chimera {
//     
// class Effect : public Entity
// {
// public:
// 
//     Effect ( std::string _name );
// 
//     //Effect ( const Effect& _cpy );
// 
//     virtual ~Effect() {}
// 
// 	void setAmbient(const Color &_color);
// 
//     inline Color getAmbient() const {
//         return this->ambient;
//     }
//     
// 	void setSpecular(const Color &_color);
// 
// 	inline Color getSpecular() const {
//         return this->specular;
//     }
// 
// 	void setDiffuse(const Color &_color);
// 
// 	inline Color getDiffuse() const {
//         return this->diffuse;
//     }
// 
// 	void setEmission(const Color &_color);
// 
// 	inline Color getEmission() const {
//         return this->emission;
//     }
// 
// 	void setShine(const float &_val);
// 
// 	inline float getShine() const {
//         return this->shine;
//     }
//       
// 	inline bool isSet(const ModeMaterial &_val) {
// 		return map_modes[_val];
// 	}
// 
// 	inline void setFace(const FaceMaterial &_val) {
// 		this->faceMaterial = _val;
// 	}
// 
// 	virtual void init();
// 
// 	void apply();
// 
//     void setNameTextureId (const std::string &_name ) {
//         nameTextureId = _name;
//     }
// 
//     std::string getNameTextureId() const {
//         return nameTextureId;
//     }
// 
// 	void createDefaultEffect();
// 
//     void loadCollada ( tinyxml2::XMLElement* _nNode );
// 
// private:
// 
//     bool getPhong ( const char* _tipoCor, Color &_color, tinyxml2::XMLElement* _nNode );
// 
//     Color diffuse;        /* Diffuse color RGBA */
//     Color ambient;        /* Ambient color RGB */
//     Color specular;       /* Specular 'shininess' */
//     Color emission;       /* Emissive color RGB */
// 	float shine;
// 
//     //float shininess;
//     std::string nameTextureId;
//     
//     FaceMaterial faceMaterial;
// 
// 	std::map<ModeMaterial, bool> map_modes;
// 	std::map<ModeMaterial, void*> map_params;
// };
// }

#endif
