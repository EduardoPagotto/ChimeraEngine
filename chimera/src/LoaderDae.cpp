#include <iostream>

#include "LoaderDae.h"

#include "ExceptionChimera.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "DrawTriMesh.h"
#include "Material.h"
#include "Effect.h"
#include "Texture.h"

namespace ChimeraLoader {

	LoaderDae::LoaderDae(const std::string &textureDir, const std::string &modelDir){

		doc = nullptr;
		root = nullptr;
		this->textureDir = textureDir;
		this->modelDir = modelDir;

	}

	LoaderDae::~LoaderDae(){

		if (doc != nullptr) {
			delete doc;
			doc = nullptr;
		}
	}

	Chimera::Node* LoaderDae::loadFile(const std::string &file) {

		Chimera::Node *pRootScene = nullptr;
		std::string dir_arquivo = modelDir + "\\" + file;
		doc = new tinyxml2::XMLDocument();

		if (doc->LoadFile(dir_arquivo.c_str()) != 0)
			throw Chimera::ExceptionChimera(Chimera::ExceptionCode::OPEN, "Falha ao ler arquivo");

		root = doc->FirstChildElement("COLLADA");
		if (root == nullptr)
			throw Chimera::ExceptionChimera(Chimera::ExceptionCode::OPEN, "Nao é um arquivo colada");

		tinyxml2::XMLElement* l_nVisualScene = findVisualScene();
		if (l_nVisualScene != nullptr) {

			const char *l_nome = l_nVisualScene->Attribute("name");
			const char *l_id = l_nVisualScene->Attribute("id");

			pRootScene = new Chimera::Node(Chimera::EntityKind::NODE, l_id, l_nome);
			tinyxml2::XMLElement* l_nNode = l_nVisualScene->FirstChildElement("node");
			if (l_nNode != nullptr) {
				while (l_nNode != nullptr) {

					const char * l_id = l_nNode->Attribute("id");
					const char * l_name = l_nNode->Attribute("name");
					const char * l_type = l_nNode->Attribute("type");

					tinyxml2::XMLElement* l_nDadoNode = l_nNode->FirstChildElement();
					if (l_nDadoNode != nullptr) {
						carregaNode(pRootScene, l_nDadoNode, l_id, l_name, l_type);
					}
					else {
						std::cout << "Node sem filho" << std::endl;
					}

					l_nNode = l_nNode->NextSiblingElement("node");
				}
			}
			else {
				std::cout << "Node: vazio " << std::endl;
			}
		}
		return pRootScene;
	}

	void LoaderDae::carregaNode(Chimera::Node *_pNodePai, tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char* type) {

		btTransform *l_pTransform = nullptr;
		Chimera::Node *pLastNodeDone = nullptr;

		while (_nNode != nullptr) {

			tinyxml2::XMLElement* l_nNodeSourceData = nullptr;
			const char* l_url = _nNode->Attribute("url");
			const char* l_target = _nNode->Attribute("target");
			const char* l_nomeElemento = _nNode->Value();

			if (strcmp(l_nomeElemento, (const char*)"matrix") == 0) {

				const char* l_tipoTransform = _nNode->Attribute("sid");
				if (strcmp(l_tipoTransform, (const char*)"transform") == 0) {

					l_pTransform = new btTransform;
					loadTransformMatrix(_nNode, l_pTransform);

				}
				else {
					//TODO: implementar carga de posicao, rotacao e transformar em matricial em l_pTransform
				}

			}
			else if (strcmp(l_nomeElemento, (const char*)"instance_camera") == 0) {

				loadNodeLib((const char*)&l_url[1], "library_cameras", "camera", &l_nNodeSourceData);

				Chimera::Camera *pCamera = new Chimera::Camera(Chimera::CameraType::Base, _id, _name);
				pCamera->loadCollada(l_nNodeSourceData);
				pCamera->setTransform(*l_pTransform);

				_pNodePai->addChild(pCamera);

				pLastNodeDone = pCamera;

			}
			else if (strcmp(l_nomeElemento, (const char*)"instance_light") == 0) {

				loadNodeLib((const char*)&l_url[1], "library_lights", "light", &l_nNodeSourceData);

				Chimera::Light *pLight = new Chimera::Light(Chimera::LightType::POINT, _id, _name);
				pLight->loadCollada(l_nNodeSourceData);
				pLight->setTransform(*l_pTransform);

				_pNodePai->addChild(pLight);

				pLastNodeDone = pLight;

			}
			else if (strcmp(l_nomeElemento, (const char*)"instance_geometry") == 0) {

				loadNodeLib((const char*)&l_url[1], "library_geometries", "geometry", &l_nNodeSourceData);

				Chimera::DrawTriMesh *pDrawTriMesh = new Chimera::DrawTriMesh(retornaAtributo("id", l_nNodeSourceData), retornaAtributo("name", l_nNodeSourceData));
				pDrawTriMesh->loadCollada(l_nNodeSourceData);

				Chimera::Object *pObj = new Chimera::Object(_id, _name);
				pObj->setTransform(*l_pTransform);
				pObj->addChild(pDrawTriMesh);
				_pNodePai->addChild(pObj);

				pLastNodeDone = pObj;

				tinyxml2::XMLElement* l_nBindMat = _nNode->FirstChildElement("bind_material");
				if (l_nBindMat != nullptr) {
					tinyxml2::XMLElement* l_nTecnicCommon = l_nBindMat->FirstChildElement("technique_common");
					if (l_nTecnicCommon != nullptr) {

						tinyxml2::XMLElement* l_nProximoFilho = l_nTecnicCommon->FirstChildElement();
						if (l_nProximoFilho != nullptr) {
							carregaNode(pObj, l_nProximoFilho, nullptr, nullptr, nullptr);
						}
						else {
							std::cout << "technique_common sem filho(bind_material missing) " << std::endl;
						}
					}
					else {
						std::cout << "bind_material sem filho(instance_material missing) " << std::endl;
					}
				}

			}
			else if (strcmp(l_nomeElemento, (const char*)"instance_material") == 0) {

				loadNodeLib((const char*)&l_target[1], "library_materials", "material", &l_nNodeSourceData);

				Chimera::Material *pMaterial = new Chimera::Material(retornaAtributo("id", l_nNodeSourceData), retornaAtributo("name", l_nNodeSourceData));
				_pNodePai->addChild(pMaterial);

				pLastNodeDone = pMaterial;

				tinyxml2::XMLElement* l_nEffe = l_nNodeSourceData->FirstChildElement();
				if (l_nEffe != nullptr) {
					carregaNode(pMaterial, l_nEffe, nullptr, nullptr, nullptr);
				}
				else {
					std::cout << "Falha Effeito incompleto: " << std::string(l_target) << std::endl;
				}

			}
			else if (strcmp(l_nomeElemento, (const char*)"instance_effect") == 0) {

				loadNodeLib((const char*)&l_url[1], "library_effects", "effect", &l_nNodeSourceData);
				Chimera::Effect *pEffect = new Chimera::Effect(retornaAtributo("id", l_nNodeSourceData), retornaAtributo("id", l_nNodeSourceData));
				pEffect->loadCollada(l_nNodeSourceData);
				_pNodePai->addChild(pEffect);

				pLastNodeDone = pEffect;

				if (pEffect->getNameTextureId().size() > 0) {

					loadNodeLib((const char*)pEffect->getNameTextureId().c_str(), "library_images", "image", &l_nNodeSourceData);
					Chimera::Texture *pTexture = new Chimera::Texture(retornaAtributo("id", l_nNodeSourceData), retornaAtributo("id", l_nNodeSourceData));

					const char* l_val = l_nNodeSourceData->FirstChildElement("init_from")->GetText();
					pTexture->setPathFile(textureDir + "\\" + std::string(l_val));
					pTexture->init();

					_pNodePai->addChild(pTexture);

				}

			}
			else if (strcmp(l_nomeElemento, (const char*)"node") == 0) {

				const char * l_id = _nNode->Attribute("id");
				const char * l_name = _nNode->Attribute("name");
				const char * l_type = _nNode->Attribute("type");

				if (pLastNodeDone != nullptr)
					carregaNode(pLastNodeDone, _nNode->FirstChildElement(), l_id, l_name, l_type);
				else {
					throw Chimera::ExceptionChimera(Chimera::ExceptionCode::READ, "Falha, objeto hierarquia: " + std::string(l_id));
				}
			}
			else  {
				throw Chimera::ExceptionChimera(Chimera::ExceptionCode::READ, "Falha, objeto desconhecido: " + std::string(l_nomeElemento));
			}
			_nNode = _nNode->NextSiblingElement();
		}
	}

	void LoaderDae::loadNodeLib(const char* _url, const char* _libName, const char* _nodeName, tinyxml2::XMLElement** _nNode) {

		tinyxml2::XMLElement* l_nLib = root->FirstChildElement(_libName);
		if (l_nLib != nullptr) {

			tinyxml2::XMLElement* l_nNodeBase = l_nLib->FirstChildElement(_nodeName);
			if (l_nNodeBase != nullptr) {

				while (l_nNodeBase != nullptr) {

					const char *l_id = l_nNodeBase->Attribute("id");
					if (strcmp(l_id, _url) == 0) {

						*_nNode = l_nNodeBase;
						return;
					}
					l_nNodeBase = l_nNodeBase->NextSiblingElement();
				}
				throw Chimera::ExceptionChimera(Chimera::ExceptionCode::READ, "Falha, nao encontrado Node: " + std::string(_url));
			}
			else {
				throw Chimera::ExceptionChimera(Chimera::ExceptionCode::READ, "Falha, nao encontrado Node Tipo: " + std::string(_nodeName));
			}
		}
		else {
			throw Chimera::ExceptionChimera(Chimera::ExceptionCode::READ, "Falha, nao encontrado Library: " + std::string(_libName));
		}

	}

	std::string LoaderDae::retornaAtributo(const std::string &_atributo, tinyxml2::XMLElement* _node) {

		const char *l_value = _node->Attribute(_atributo.c_str());
		if (l_value != nullptr) {
			return std::string(l_value);
		}

		std::cout << std::string("Atributo [ " + _atributo + " ] Elemento [ " + _node->Value() + " ] nao encontrado") << std::endl;
		return std::string("");
	}

	void LoaderDae::loadTransformMatrix(tinyxml2::XMLElement* _nNode, btTransform *_pTransform) {

		std::vector<float> l_arrayValores;
		const char* l_matrix = _nNode->GetText();
		loadArrayBtScalar(l_matrix, l_arrayValores);
		carregaMatrix(_pTransform, l_arrayValores);

	}

	void LoaderDae::loadArrayBtScalar(const char *_val, std::vector<btScalar> &_arrayF) {

		char l_numTemp[32];
		const char *pchFim;
		const char *pchInit = _val;
		const char *deadEnd = _val + strlen(_val);
		do {
			pchFim = strchr(pchInit, ' ');
			if (pchFim == nullptr)
				pchFim = deadEnd;

			int l_tam = pchFim - pchInit;
			memcpy(l_numTemp, pchInit, l_tam);
			l_numTemp[l_tam] = 0;
			_arrayF.push_back((float)atof(l_numTemp));

			pchInit = pchFim + 1;
		} while (pchInit < deadEnd);
	}

	void LoaderDae::carregaMatrix(btTransform *_pTrans, const std::vector<float> &listaMatrix) {

		if (listaMatrix.size() != 16)
			throw Chimera::ExceptionChimera(Chimera::ExceptionCode::READ, "Tamanho da Matrix invalido" + std::to_string(listaMatrix.size()));

		btScalar ponteiroFloat[16];
		int indice = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				int pos = i + (4 * j);
				ponteiroFloat[pos] = listaMatrix[indice];
				indice++;
			}
		}

		_pTrans->setFromOpenGLMatrix(ponteiroFloat);
	}


	tinyxml2::XMLElement* LoaderDae::findVisualScene() {

		tinyxml2::XMLElement* l_nNodeScene = root->FirstChildElement("scene");
		if (l_nNodeScene != nullptr) {

			tinyxml2::XMLElement* l_nNode = l_nNodeScene->FirstChildElement("instance_visual_scene");
			if (l_nNode != nullptr) {

				const char* l_url = l_nNode->Attribute("url");
				l_nNode = root->FirstChildElement("library_visual_scenes");
				if (l_nNode != nullptr) {

					l_nNode = l_nNode->FirstChildElement();
					if (l_nNode != nullptr) {
						while (l_nNode != nullptr) {

							const char *l_id = l_nNode->Attribute("id");
							if (strcmp(l_id, (const char*)&l_url[1]) == 0) {
								return l_nNode;
							}

							l_nNode = l_nNode->NextSiblingElement();
						}

						std::cout << "Visual Scene: " << (const char*)&l_url[1] << " nao encontrado " << std::endl;
					}
					else {
						std::cout << "Visual Scene: vazio " << std::endl;
					}

					// 					std::cout << "Visual Scene: " << (const char*)&l_url[1] << " nao encontrado " << std::endl;
				}
				else {
					std::cout << "library_visual_scenes nao encontrado " << std::endl;
				}
			}
			else {
				std::cout << "instance_visual_scene nao encontrado " << std::endl;
			}
		}

		return nullptr;
	}
}
