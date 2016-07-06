#include "Material.h"

namespace Chimera {
namespace Graph {
    
Material::Material ( std::string _id, std::string _name ) : Entity ( EntityKind::MATERIAL, _id, _name ) {
    pEffect = nullptr;
    pTexture = nullptr;
}

Material::Material ( const Material &_cpy ) : Entity ( _cpy ) {
}

Material::~Material() {
}

void Material::clone ( Entity **ppNode ) {
    *ppNode = new Material ( *this );
    //Node::clone ( ppNode ); //FIXME necessario descer ao pai?
}

void  Material::init() {

	if (pTexture != nullptr)
		pTexture->init();

	if (pEffect == nullptr) {
		pEffect = new Effect("effect_interno", "effect_interno");
		pEffect->createDefaultEffect();
	}

}

void Material::end() {

	if (( hasTextureAtive == true ) && (pTexture != nullptr))
		pTexture->end();
}

void Material::begin ( bool _texture ) {

    hasTextureAtive = _texture;
   

    if (( _texture == true ) && (pTexture != nullptr))
		pTexture->begin();

	pEffect->apply();
       
}
}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
