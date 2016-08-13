#include "Material.h"
#include "ChimeraUtils.h"

namespace Chimera {
    
Material::Material ( std::string _name ) : Entity ( EntityKind::MATERIAL, _name ) {
    
    shine = 50.0f;
    diffuse = Color::BLACK;
    ambient = Color::BLACK;
    specular = Color::BLACK;
    emission = Color::BLACK;
    
    map_modes[ModeMaterial::AMBIENT]=false;
    map_modes[ModeMaterial::DIFFUSE]=false;
    map_modes[ModeMaterial::EMISSION]=false;
    map_modes[ModeMaterial::SPECULAR]=false;
    map_modes[ModeMaterial::SHININESS]=false;
    
	setFace(FaceMaterial::FRONT);

	texturePresent = false;
	pTexDiffuse = nullptr;
	pTexSpecular = nullptr;
	pTexEmissive = nullptr;
}

Material::~Material() {
}

void Material::init() {

	if (pTexDiffuse != nullptr)
		pTexDiffuse->init();

	if (pTexSpecular != nullptr)
		pTexSpecular->init();

	if (pTexEmissive != nullptr)
		pTexEmissive->init();
}

void Material::setAmbient(const Color &_color) {

	ambient = _color;
	map_modes[ModeMaterial::AMBIENT] = true;
	map_params[ModeMaterial::AMBIENT] = &ambient;

}

void Material::setDiffuse(const Color &_color) {

	diffuse = _color;
	map_modes[ModeMaterial::DIFFUSE] = true;
	map_params[ModeMaterial::DIFFUSE] = &diffuse;

}

void Material::setEmission(const Color &_color) {

	emission = _color;
	map_modes[ModeMaterial::EMISSION] = true;
	map_params[ModeMaterial::EMISSION] = &emission;

}

void Material::setSpecular(const Color &_color) {

	specular = _color;
	map_modes[ModeMaterial::SPECULAR] = true;
	map_params[ModeMaterial::SPECULAR] = &specular;

}

void Material::setShine(const float &_val) {

	shine = _val;
	map_modes[ModeMaterial::SHININESS] = true;
	map_params[ModeMaterial::SHININESS] = &shine;

}

Color Material::getAmbient() const{
    return this->ambient;
}
    
Color Material::getSpecular() const {
    return this->specular;
}

Color Material::getDiffuse() const {
    return this->diffuse;
}

Color Material::getEmission() const {
    return this->emission;
}

float Material::getShine() const {
    return this->shine;
}
      
bool Material::isSet(const ModeMaterial &_val) {
    return map_modes[_val];
}

void Material::setFace(const FaceMaterial &_val) {
    this->faceMaterial = _val;
}

void Material::createDefaultEffect() {

	setDiffuse(Color(0.6f, 0.6f, 0.6f));
	setEmission(Color(0.1f, 0.1f, 0.1f));
	setAmbient(Color(0.1f, 0.1f, 0.1f));
	setSpecular(Color(0.5f, 0.5f, 0.5f));
	setShine(50.0f);
}

void Material::apply(Shader *pShader) {

	for (std::map<ModeMaterial, void*>::iterator iter = map_params.begin(); iter != map_params.end(); ++iter) {

		ModeMaterial k = iter->first;
		float *p = (k != ModeMaterial::SHININESS) ? (float*)((Color*)iter->second)->ptr() : (float*)iter->second;

        if (pShader == nullptr) {
            
            glMaterialfv((GLenum)this->faceMaterial, (GLenum)k, p);
            
        } else {
        
            if (k == AMBIENT) {
                
                GLint matAmbientLoc  = glGetUniformLocation( pShader->getIdProgram(), "material.ambient" );
                glUniform4fv(matAmbientLoc, 1, p);
                
            } else if (k == DIFFUSE) {
                
                GLint matDiffuseLoc  = glGetUniformLocation( pShader->getIdProgram(), "material.diffuse" );
                glUniform4fv(matDiffuseLoc, 1, p);
                
            } else if (k == SPECULAR) { 
                
                GLint matSpecularLoc = glGetUniformLocation( pShader->getIdProgram(), "material.specular" );
                glUniform4fv(matSpecularLoc, 1, p);
                
            } else if (k == SHININESS) {
                
                GLint matShineLoc    = glGetUniformLocation( pShader->getIdProgram(), "material.shininess" ); 
                glUniform1fv(matShineLoc, 1, p);
            
            } else {
                //TODO erro
            }
        }
	}
	
	if (texturePresent == true) {

		glEnable(GL_TEXTURE_2D);

		if (pTexDiffuse != nullptr)
			pTexDiffuse->apply();

		if (pTexSpecular != nullptr)
			pTexSpecular->apply();

		if (pTexEmissive != nullptr)
			pTexEmissive->apply();
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//se ha textura coloque a cor como branca para nao interferir com a textura
	//if (hasTexture) {
	//	glMaterialfv(GL_FRONT, GL_DIFFUSE, Color::WHITE.ptr());
	//}

   //  if ( glIsEnabled ( GL_COLOR_MATERIAL ) == GL_TRUE ) {
   //      glMaterialfv ( GL_FRONT, GL_AMBIENT, ambient.ptr() );
   //	   glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.ptr() );
   //      glMaterialfv ( GL_FRONT, GL_SPECULAR, specular.ptr() );
   //      glMaterialfv ( GL_FRONT, GL_SHININESS, &shine );
   //      glMaterialfv ( GL_FRONT, GL_EMISSION, emission.ptr() );
   //  }
}

void Material::loadCollada(tinyxml2::XMLElement* root, tinyxml2::XMLElement* _nNode) {
    
    tinyxml2::XMLElement* l_nInstanceEffect = _nNode->FirstChildElement ( "instance_effect" );
    if (l_nInstanceEffect != nullptr) {
     
        tinyxml2::XMLElement* l_nNodeSourceData = nullptr;
        const char* l_pUrlEffect = l_nInstanceEffect->Attribute ( "url" );
        loadNodeLib ( root, ( const char* ) &l_pUrlEffect[1], "library_effects", "effect", &l_nNodeSourceData );
        
        if (l_nNodeSourceData != nullptr) {
            loadColladaProfile(l_nNodeSourceData); 
        }
    }
}

void Material::loadColladaProfile(tinyxml2::XMLElement* _nNode) {

	tinyxml2::XMLElement* l_nProfile = _nNode->FirstChildElement("profile_COMMON");
	if (l_nProfile != nullptr) {

        Color cor;
        if (getPhong("emission", cor, l_nProfile) == true) {
            setEmission(cor);
        }

        if (getPhong("ambient", cor, l_nProfile) == true) {
            setAmbient(cor);
        }

        if (getPhong("diffuse", cor, l_nProfile) == true) {
            setDiffuse(cor);
        }

        if (getPhong("specular", cor, l_nProfile) == true) {
            setSpecular(cor);
        }

        tinyxml2::XMLElement* l_nNode = l_nProfile->FirstChildElement("technique");
        if (l_nNode->Attribute("sid") != nullptr) {

            tinyxml2::XMLElement* l_nPhong = l_nNode->FirstChildElement("phong");
            if (l_nPhong != nullptr) {

                tinyxml2::XMLElement* l_nShinnes = l_nPhong->FirstChildElement("shininess");
                if (l_nShinnes != nullptr) {

                    const char *l_val = l_nShinnes->FirstChildElement("float")->GetText();
                    if (l_val != nullptr) {

                        setShine(atof(l_val));

                    }
                }
            }
        }
    }
}

bool Material::getPhong ( const char* _tipoCor, Color &_color, tinyxml2::XMLElement* _nNode ) {

    tinyxml2::XMLElement* l_nNode = _nNode->FirstChildElement ( "technique" );
    if ( l_nNode->Attribute ( "sid" ) != nullptr ) {

        tinyxml2::XMLElement* l_nPhong = l_nNode->FirstChildElement ( "phong" );
        if ( l_nPhong != nullptr ) {
            tinyxml2::XMLElement* l_nColor = l_nPhong->FirstChildElement ( _tipoCor );
            if ( l_nColor != nullptr ) {
				tinyxml2::XMLElement* l_nColorVal = l_nColor->FirstChildElement ( "color" );
                if (l_nColorVal != nullptr ) {
                    std::vector<float> l_arrayF;
                    const char* l_cor = l_nColorVal->GetText();
                    loadArrayBtScalar ( l_cor, l_arrayF );

                    _color.set ( l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0 );
                    return true;
                } else {

					l_nColorVal = l_nColor->FirstChildElement("texture");
					if (l_nColorVal != nullptr) {

						_color.set(Color::WHITE);
						return true;
					}
                }
            }
         }
    }

    return false;
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
