#include "Effect.h"

#ifdef WIN32
#include "windows.h"
#endif

#include <GL/gl.h>
//#include <gl/GLU.h>

#include "ChimeraUtils.h"

namespace Chimera {

	Effect::Effect(std::string _id, std::string _name) : Node(EntityKind::EFFECT, _id, _name) {

		shininess = 10.5f;
		diffuse = Color::BLACK;
		ambient = Color::BLACK;
		specular = Color::BLACK;
		emissive = Color::BLACK;
		nameTextureId = "";

	}

	Effect::Effect(const Effect& _cpy) : Node(_cpy) {

		diffuse = _cpy.diffuse;
		ambient = _cpy.ambient;
		specular = _cpy.specular;
		emissive = _cpy.emissive;
		shininess = _cpy.shininess;
		nameTextureId = _cpy.nameTextureId;

	}

	void Effect::render() {

		if (glIsEnabled(GL_COLOR_MATERIAL) == GL_TRUE) {
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient.ptr());
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.ptr());
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular.ptr());
			glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
			glMaterialfv(GL_FRONT, GL_EMISSION, emissive.ptr());
		}

	}

	void Effect::init() {

	}

	void Effect::clone(Node **ppNode) {
		*ppNode = new Effect(*this);
		Node::clone(ppNode);
	}

	void Effect::update(DataMsg *dataMsg) {

		if (dataMsg->getKindOp() == KindOp::START) {

			init();
		}

		Node::update(dataMsg);
	}

	bool Effect::getPhong(const char* _tipoCor, Color &_color, tinyxml2::XMLElement* _nNode) {

		tinyxml2::XMLElement* l_nNode = _nNode->FirstChildElement("technique");
		if (l_nNode->Attribute("sid") != nullptr) {

			tinyxml2::XMLElement* l_nPhong = l_nNode->FirstChildElement("phong");
			if (l_nPhong != nullptr) {
				tinyxml2::XMLElement* l_nColor = l_nPhong->FirstChildElement(_tipoCor);
				if (l_nColor != nullptr) {
					l_nColor = l_nColor->FirstChildElement("color");
					if (l_nColor != nullptr) {
						std::vector<btScalar> l_arrayF;
						const char* l_cor = l_nColor->GetText();
						loadArrayBtScalar(l_cor, l_arrayF);

						_color.set(l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0);
						return true;
					}
					else {

						_color.set(1.0, 1.0, 1.0, 1.0);//FIXME preciso??

					}
				}
			}
		}

		return false;
	}

	void Effect::loadCollada(tinyxml2::XMLElement* _nNode) {

		tinyxml2::XMLElement* l_nProfile = _nNode->FirstChildElement("profile_COMMON");
		if (l_nProfile != nullptr) {
			tinyxml2::XMLElement* l_nParam = l_nProfile->FirstChildElement("newparam");
			if (l_nParam != nullptr) {
				const char* l_val = l_nParam->FirstChildElement("surface")->FirstChildElement("init_from")->GetText();
				if (l_val != nullptr) {
					setNameTextureId(l_val);
				}
			}

			Color cor;
			if (getPhong("emission", cor, l_nProfile) == true) {
				setEmissive(cor);
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

							setShininess(atof(l_val));

						}

					}
				}
			}

		}
	}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
