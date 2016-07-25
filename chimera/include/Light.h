#ifndef LIGHT_H_
#define LIGHT_H_

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>

#include <tinyxml2.h>

#include "Color.h"
#include "Node.h"

#include "OpenGLDefs.h"
#include <map>

namespace Chimera {
    
enum class LightType
{
    DIRECTIONAL = 0,
    POSITIONAL = 1,
};

class Light : public Node
{
public:
    Light (Node* _parent, std::string _name );
    virtual ~Light();

	virtual void init();

    virtual void apply(LightNum lightNum);

	virtual void accept(class NodeVisitor* v);

    Color getAmbient() {
        return ambient;
    }

    Color getSpecular() {
        return specular;
    }

    Color getDiffuse() {
        return diffuse;
    }

    void setAmbient ( Color _color ) {
        ambient = _color;
        map_params[ModeMaterial::AMBIENT] = &ambient;
    }

    void setSpecular ( Color _color ) {
        specular = _color;
        map_params[ModeMaterial::SPECULAR] = &specular;
    }

    void setDiffuse ( Color _color ) {
        diffuse = _color;
        map_params[ModeMaterial::DIFFUSE] = &diffuse;
    }

    void setType ( LightType _type ) {
        type = _type;
    }

    void setTransform ( const btTransform &_trans ) {
        transform = _trans;
    }

    btVector3 getPosition() const {
        return position;
    }

    void setPositionRotation ( const btVector3 &_posicao, const btVector3 &_rotation );
    void loadCollada ( tinyxml2::XMLElement* _nNode );

private:

    btTransform transform;
    btVector3 position;
    
    Color ambient;
    Color specular;
    Color diffuse;
    
    int number;
    LightType type;
    
    std::map<ModeMaterial, void*> map_params;
};
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
