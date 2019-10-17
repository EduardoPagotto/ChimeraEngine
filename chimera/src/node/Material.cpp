// #include "chimera/node/Material.hpp"

// namespace Chimera {

// Material::Material(std::string _name) : Entity(EntityKind::MATERIAL, _name) {
//     tipoTexturasDisponiveis = -1;
//     texManager = Singleton<TextureManager>::getRefSingleton();
// }

// Material::~Material() { Singleton<TextureManager>::releaseRefSingleton(); }

// void Material::init() {

//     bool hasDifuse = false;
//     bool hasEspecular = false;
//     bool hasEmissive = false;

//     for (std::map<std::string, Texture*>::iterator iTex = mapTex.begin(); iTex != mapTex.end(); iTex++) {

//         Texture* pTex = iTex->second;
//         pTex->init();

//         switch (pTex->getIndexTextureSeq()) {
//             case TEX_KIND::DIFFUSE:
//                 hasDifuse = true;
//                 break;
//             case TEX_KIND::EMISSIVE:
//                 hasEmissive = true;
//                 break;
//             case TEX_KIND::SPECULAR:
//                 hasEspecular = true;
//                 break;
//             default:
//                 break;
//         }
//     }

//     if (mapTex.size() == 0)
//         tipoTexturasDisponiveis = 0;
//     else if ((hasDifuse == true) && (hasEspecular == false) && (hasEmissive == false))
//         tipoTexturasDisponiveis = 1;
//     else if ((hasDifuse == true) && (hasEspecular == true) && (hasEmissive == false))
//         tipoTexturasDisponiveis = 2;
//     else if ((hasDifuse == true) && (hasEspecular == true) && (hasEmissive == true))
//         tipoTexturasDisponiveis = 3;
//     else
//         tipoTexturasDisponiveis = 4;
// }

// void Material::setTexture(Texture* _pTex) {
//     switch (_pTex->getIndexTextureSeq()) {
//         case TEX_KIND::DIFFUSE:
//             mapTex[SHADE_TEXTURE_DIFFUSE] = _pTex;
//             break;
//         case TEX_KIND::EMISSIVE:
//             mapTex[SHADE_TEXTURE_EMISSIVE] = _pTex;
//             break;
//         case TEX_KIND::SPECULAR:
//             mapTex[SHADE_TEXTURE_SPECULA] = _pTex;
//             break;
//         default:
//             break;
//     }
// }

// void Material::loadTextureFromFile(const std::string& _nome, const TEX_KIND& _seq, const std::string& _arquivo) {
//     setTexture(texManager->fromFile(_nome, _seq, _arquivo));
// }

// void Material::createDefaultEffect() {
//     setDiffuse(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
//     setEmission(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
//     setAmbient(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
//     setSpecular(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
//     setShine(50.0f);
// }

// void Material::apply(Shader* _shader) {

//     // aplica todos os materiais passados
//     for (ShaderValue* shaderMaterial : listMaterial) {
//         shaderMaterial->apply(_shader);
//     }

//     _shader->setGlUniform1i(SHADE_TEXTURE_SELETOR_TIPO_VALIDO, tipoTexturasDisponiveis);

//     if (mapTex.size() > 0) {
//         for (std::map<std::string, Texture*>::iterator iTex = mapTex.begin(); iTex != mapTex.end(); iTex++) {
//             std::string name = iTex->first;
//             Texture* pTex = iTex->second;
//             pTex->apply(name, _shader);
//         }
//     } else {
//         glBindTexture(GL_TEXTURE_2D, 0);
//     }
// }

// } // namespace Chimera
