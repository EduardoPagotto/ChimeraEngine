#include "LibraryEffects.h"
#include "Singleton.h"
#include "ExceptionChimera.h"
#include "LoaderDaeUtils.h"
#include "LibraryImages.h"

namespace ChimeraLoaders {

LibraryEffects::LibraryEffects(tinyxml2::XMLElement* _root, const std::string &_url) : Library(_root, _url) {
    pTexManager = Chimera::Singleton<Chimera::TextureManager>::getRefSingleton();
}

LibraryEffects::~LibraryEffects() {
    Chimera::Singleton<Chimera::TextureManager>::releaseRefSingleton();
}

Chimera::Material *LibraryEffects::target() {

    tinyxml2::XMLElement* l_nLib = root->FirstChildElement("library_effects");
    if ( l_nLib != nullptr ) {

        tinyxml2::XMLElement* l_nNodeBase = l_nLib->FirstChildElement ("effect");
        if ( l_nNodeBase != nullptr ) {

            while ( l_nNodeBase != nullptr ) {
                std::string l_id = l_nNodeBase->Attribute ( "id" );
                if (url.compare(l_id) == 0) {

                    tinyxml2::XMLElement* l_nProfile = l_nNodeBase->FirstChildElement("profile_COMMON");
                    if (l_nProfile != nullptr) {

                        Chimera::Material *pMaterial = new Chimera::Material(l_id);

                        loadNewParam(l_nProfile);
                        loadColors(l_nProfile, pMaterial);
                        return pMaterial;
                    }    
                }
                l_nNodeBase = l_nNodeBase->NextSiblingElement();
            }
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node: " + std::string ( url ) );
        } else {
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node Tipo: " + std::string ( "camera" ) );
        }
    } else {
        throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Library: " + std::string ( "library_cameras" ) );
    }
}

Chimera::Color LibraryEffects::getColor( tinyxml2::XMLElement* l_nColorVal) {

    std::vector<float> l_arrayF;
    const char* l_cor = l_nColorVal->GetText();
    Chimera::loadArrayBtScalar ( l_cor, l_arrayF );
    return Chimera::Color(l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0);

}

unsigned LibraryEffects::getTexture(tinyxml2::XMLElement* _nTex) {

    std::string texture1 = _nTex->Attribute("texture");

    auto sampler2D = mapaEfeito.find(texture1);
    if (sampler2D != mapaEfeito.end()) {

        auto sampler2DTxt = sampler2D->second;
        auto surface = mapaEfeito.find(sampler2DTxt);

        if (surface != mapaEfeito.end()) {

            auto textureName = surface->second;

            LibraryImages li(root, textureName);
            std::tuple<std::string, std::string> val = li.target();

            std::string valId = std::get<0>(val);
            std::string valPathFile = std::get<1>(val);

            pTexManager->fromFile(valId, Chimera::TEX_SEQ::DIFFUSE, valPathFile);
            unsigned serialTex = pTexManager->getIdByName(textureName);
            return serialTex;
        }
    }

    throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::OPEN, "Texture definido mas nao encontrado");

}

void LibraryEffects::loadColors(tinyxml2::XMLElement* _nProfile, Chimera::Material *_pMat) {

    tinyxml2::XMLElement* l_nTec = _nProfile->FirstChildElement("technique");
    if (l_nTec != nullptr) {

        tinyxml2::XMLElement* l_nPhong = l_nTec->FirstChildElement("phong");
        if (l_nPhong != nullptr) {

            tinyxml2::XMLElement* l_nCor = l_nPhong->FirstChildElement();
            while(l_nCor != nullptr) {

                if (strcmp(l_nCor->Value(), (const char*)"emission") == 0) {

                    tinyxml2::XMLElement* l_nColorVal = l_nCor->FirstChildElement ( "color" );
                    if (l_nColorVal != nullptr) {

                        _pMat->setEmission(getColor(l_nColorVal));

                    } else {
                        tinyxml2::XMLElement* l_nTex = l_nCor->FirstChildElement("texture");
                        if (l_nTex != nullptr)  {

                            _pMat->setEmission(Chimera::Color::WHITE);
                            _pMat->defineTextureByIndex( getTexture(l_nTex));

                        } else {
                            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::OPEN, "Tipo de cor indefinida");
                        }
                    }

                } else if (strcmp(l_nCor->Value(), (const char*)"ambient") == 0) {

                    tinyxml2::XMLElement* l_nColorVal = l_nCor->FirstChildElement ( "color" );
                    if (l_nColorVal != nullptr) {

                        _pMat->setAmbient(getColor(l_nColorVal));

                    } else {
                        tinyxml2::XMLElement* l_nTex = l_nCor->FirstChildElement("texture");
                        if (l_nTex != nullptr)  {

                            _pMat->setAmbient(Chimera::Color::WHITE);
                            _pMat->defineTextureByIndex( getTexture(l_nTex));
                            
                        } else {
                            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::OPEN, "Tipo de cor indefinida");
                        }
                    }

                } else if (strcmp(l_nCor->Value(), (const char*)"diffuse") == 0) {

                    tinyxml2::XMLElement* l_nColorVal = l_nCor->FirstChildElement ( "color" );
                    if (l_nColorVal != nullptr) {

                        _pMat->setDiffuse(getColor(l_nColorVal));

                    } else {
                        tinyxml2::XMLElement* l_nTex = l_nCor->FirstChildElement("texture");
                        if (l_nTex != nullptr)  {

                            _pMat->setDiffuse(Chimera::Color::WHITE);
                            _pMat->defineTextureByIndex( getTexture(l_nTex));
                            
                        } else {
                            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::OPEN, "Tipo de cor indefinida");
                        }
                    }

                } else if (strcmp(l_nCor->Value(), (const char*)"specular") == 0) {


                    tinyxml2::XMLElement* l_nColorVal = l_nCor->FirstChildElement ( "color" );
                    if (l_nColorVal != nullptr) {

                        _pMat->setSpecular(getColor(l_nColorVal));

                    } else {
                        tinyxml2::XMLElement* l_nTex = l_nCor->FirstChildElement("texture");
                        if (l_nTex != nullptr)  {

                            _pMat->setSpecular(Chimera::Color::WHITE);
                            _pMat->defineTextureByIndex( getTexture(l_nTex));
                            
                        } else {
                            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::OPEN, "Tipo de cor indefinida");
                        }
                    }

                } else if (strcmp(l_nCor->Value(), (const char*)"shininess") == 0) {


                    const char *l_val = l_nCor->FirstChildElement("float")->GetText();
                    if (l_val != nullptr) {
                        _pMat->setShine(atof(l_val));
                    } else {
                            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::OPEN, "Tipo de cor indefinida");
                    }
                }

                l_nCor = l_nCor->NextSiblingElement();
            }
        }
    }
}

void LibraryEffects::loadNewParam(tinyxml2::XMLElement* _nProfile) {

    tinyxml2::XMLElement* l_nParam = _nProfile->FirstChildElement("newparam");
    while (l_nParam != nullptr) {

        std::string sid = l_nParam->Attribute("sid");

        tinyxml2::XMLElement* l_nSurface = l_nParam->FirstChildElement("surface");
        if (l_nSurface != nullptr) {

            std::string val = l_nSurface->FirstChildElement("init_from")->GetText();
            mapaEfeito.insert(std::pair<std::string, std::string>(sid,val));
        }

        tinyxml2::XMLElement* l_nSample2D = l_nParam->FirstChildElement("sampler2D");
        if (l_nSample2D != nullptr) {

            std::string val = l_nSample2D->FirstChildElement("source")->GetText();
            mapaEfeito.insert(std::pair<std::string, std::string>(sid,val));
        }

        l_nParam = l_nParam->NextSiblingElement();
    }

}
}