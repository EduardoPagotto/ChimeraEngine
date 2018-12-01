#include "chimera/node/Material.hpp"

namespace Chimera {

Material::Material(std::string _name) : Entity(EntityKind::MATERIAL, _name) {

    shine = 50.0f;
    diffuse = Color::BLACK;
    ambient = Color::BLACK;
    specular = Color::BLACK;
    emission = Color::BLACK;

    tipoTexturasDisponiveis = -1;

    texManager = Singleton<TextureManager>::getRefSingleton();
}

Material::~Material() { Singleton<TextureManager>::releaseRefSingleton(); }

void Material::init() {

    bool hasDifuse = false;
    bool hasEspecular = false;
    bool hasEmissive = false;

    for (std::map<std::string, Texture*>::iterator iTex = mapTex.begin(); iTex != mapTex.end(); iTex++) {

        Texture* pTex = iTex->second;

        switch (pTex->getIndexTextureSeq()) {
            case TEX_SEQ::DIFFUSE:
                hasDifuse = true;
                break;
            case TEX_SEQ::EMISSIVE:
                hasEmissive = true;
                break;
            case TEX_SEQ::SPECULAR:
                hasEspecular = true;
                break;
            default:
                break;
        }

        texManager->init(pTex->getSerial());
    }

    if (mapTex.size() == 0)
        tipoTexturasDisponiveis = 0;
    else if ((hasDifuse == true) && (hasEspecular == false) && (hasEmissive == false))
        tipoTexturasDisponiveis = 1;
    else if ((hasDifuse == true) && (hasEspecular == true) && (hasEmissive == false))
        tipoTexturasDisponiveis = 2;
    else if ((hasDifuse == true) && (hasEspecular == true) && (hasEmissive == true))
        tipoTexturasDisponiveis = 3;
    else
        tipoTexturasDisponiveis = 4;
}

void Material::setAmbient(const Color& _color) {
    ambient = _color;
    mapMatVal[SHADE_MAT_AMBIENTE] = &ambient;
}

void Material::setDiffuse(const Color& _color) {
    diffuse = _color;
    mapMatVal[SHADE_MAT_DIFFUSE] = &diffuse;
}

void Material::setEmission(const Color& _color) { // TODO implementar
    // 	emission = _color;
    //     mapMatVal[ SHADE_MAT_EMISSIVE ] = &emission;
}

void Material::setSpecular(const Color& _color) {
    specular = _color;
    mapMatVal[SHADE_MAT_SPECULA] = &specular;
}

void Material::setShine(const float& _val) {

    shine = _val;
    mapMatVal[SHADE_MAT_SHININESS] = &shine;
}

void Material::defineTextureByIndex(const unsigned int& _serial) {
    Texture* pTex = texManager->getTexture(_serial);
    switch (pTex->getIndexTextureSeq()) {
        case TEX_SEQ::DIFFUSE:
            mapTex[SHADE_TEXTURE_DIFFUSE] = pTex;
            break;
        case TEX_SEQ::EMISSIVE:
            mapTex[SHADE_TEXTURE_EMISSIVE] = pTex;
            break;
        case TEX_SEQ::SPECULAR:
            mapTex[SHADE_TEXTURE_SPECULA] = pTex;
            break;
        default:
            break;
    }
}

void Material::loadTextureFromFile(const std::string& _nome, const TEX_SEQ& _seq, const std::string& _arquivo) {
    int val = texManager->fromFile(_nome, _seq, _arquivo);
    defineTextureByIndex(val);
}

Color Material::getAmbient() const { return this->ambient; }

Color Material::getSpecular() const { return this->specular; }

Color Material::getDiffuse() const { return this->diffuse; }

Color Material::getEmission() const { return this->emission; }

float Material::getShine() const { return this->shine; }

void Material::createDefaultEffect() {

    setDiffuse(Color(0.6f, 0.6f, 0.6f));
    setEmission(Color(0.1f, 0.1f, 0.1f));
    setAmbient(Color(0.1f, 0.1f, 0.1f));
    setSpecular(Color(0.5f, 0.5f, 0.5f));
    setShine(50.0f);
}

void Material::apply(Shader* _shader) {

    for (std::map<std::string, void*>::iterator iter = mapMatVal.begin(); iter != mapMatVal.end(); ++iter) {

        std::string nome = iter->first;
        if (nome.compare(SHADE_MAT_SHININESS) != 0)
            _shader->setGlUniform4fv(nome.c_str(), 1,
                                     (float*)((Color*)iter->second)->ptr()); // ponteiro de um color
        else
            _shader->setGlUniform1fv(nome.c_str(), 1,
                                     (float*)iter->second); // ponteiro de um float
    }

    _shader->setGlUniform1i(SHADE_TEXTURE_SELETOR_TIPO_VALIDO, tipoTexturasDisponiveis);

    if (mapTex.size() > 0) {

        // glEnable(GL_TEXTURE_2D);
        for (std::map<std::string, Texture*>::iterator iTex = mapTex.begin(); iTex != mapTex.end(); iTex++) {

            std::string name = iTex->first;
            Texture* pTex = iTex->second;

            pTex->apply();
            _shader->setGlUniform1i(name.c_str(), (int)pTex->getIndexTextureSeq());
        }
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

} // namespace Chimera