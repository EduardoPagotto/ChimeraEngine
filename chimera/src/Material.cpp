#include "Material.h"

namespace Chimera {
    
Material::Material ( std::string _name ) : Entity ( EntityKind::MATERIAL, _name ) {
    pEffect = nullptr;
    pTexture = nullptr;
}

Material::Material ( const Material &_cpy ) : Entity ( _cpy ) {
}

Material::~Material() {
}

void  Material::init() {

	if (pTexture != nullptr)
		pTexture->init();

	if (pEffect == nullptr) {
		pEffect = new Effect( "effect_interno");
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
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
