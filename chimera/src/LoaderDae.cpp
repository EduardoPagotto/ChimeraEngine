#include <iostream>

#include "LoaderDae.h"
#include "ChimeraUtils.h"
#include "ExceptionChimera.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "DrawTriMesh.h"
#include "Material.h"
#include "Effect.h"
#include "Texture.h"
#include "Singleton.h"

namespace ChimeraLoader {

	LoaderDae::LoaderDae(const std::string &textureDir, const std::string &modelDir){

		doc = nullptr;
		root = nullptr;
		this->textureDir = textureDir;
		this->modelDir = modelDir;

		pPhysicsControl = Chimera::Singleton<Chimera::PhysicsControl>::getRefSingleton();
		
	}

	LoaderDae::~LoaderDae(){

		if (doc != nullptr) {
			delete doc;
			doc = nullptr;
		}
		
		Chimera::Singleton<Chimera::PhysicsControl>::releaseRefSingleton();
	}

	Chimera::Node* LoaderDae::loadFile(const std::string &file) {

		Chimera::Node *pRootScene = nullptr;
#ifdef WIN32		
		std::string dir_arquivo = modelDir + "\\" + file;
#else
		std::string dir_arquivo = modelDir + "/" + file;
#endif
		
		doc = new tinyxml2::XMLDocument();

		if (doc->LoadFile(dir_arquivo.c_str()) != 0)
			throw Chimera::ExceptionChimera(Chimera::ExceptionCode::OPEN, "Falha ao ler arquivo");

		root = doc->FirstChildElement("COLLADA");
		if (root == nullptr)
			throw Chimera::ExceptionChimera(Chimera::ExceptionCode::OPEN, "Nao é um arquivo colada");

		pRootScene = getNodeSceneInFile();
		
		getPhysicSceneInfile();
		
		return pRootScene;
	}

	void LoaderDae::getPhysicSceneInfile(){

		tinyxml2::XMLElement* l_nPhysicScene = findSceneLib((const char*)"Physic Scene", (const char*)"instance_physics_scene",(const char*)"library_physics_scenes");
		if (l_nPhysicScene != nullptr) {
			
			const char *l_nome = l_nPhysicScene->Attribute("name");
			const char *l_id = l_nPhysicScene->Attribute("id");
					
			pPhysicsControl->loadCollada(l_nPhysicScene);
			
			getDadosInstancePhysicModel(l_nPhysicScene);
			
		}
	}
	
	void LoaderDae::getDadosInstancePhysicModel(tinyxml2::XMLElement* _nPhysicScene) {
		
		tinyxml2::XMLElement* l_nInstancePhysicModel = _nPhysicScene->FirstChildElement("instance_physics_model");
		if (l_nInstancePhysicModel != nullptr) {
			
			const char *l_url = l_nInstancePhysicModel->Attribute("url");
			
			tinyxml2::XMLElement* l_nInstanceRigidBody = l_nInstancePhysicModel->FirstChildElement("instance_rigid_body");
			if (l_nInstanceRigidBody != nullptr) {
				while (l_nInstanceRigidBody != nullptr) {
					
					const char *l_body = l_nInstanceRigidBody->Attribute("body");
					const char *l_target = l_nInstanceRigidBody->Attribute("target");
					
					
					
					l_nInstanceRigidBody = l_nInstanceRigidBody->NextSiblingElement("instance_rigid_body");
				}
			}
			
			tinyxml2::XMLElement* l_nInstanceRigidConstraint = l_nInstancePhysicModel->FirstChildElement("instance_rigid_constraint");
			if (l_nInstanceRigidConstraint != nullptr) {
				while (l_nInstanceRigidConstraint != nullptr) {
					
					const char *l_constraint = l_nInstanceRigidConstraint->Attribute("constraint");
					
					
					
					l_nInstanceRigidConstraint = l_nInstanceRigidConstraint->NextSiblingElement("instance_rigid_constraint");
				}
			}			
		} 
	}
	
	void LoaderDae::getDadosPhysicModel(const char* _url) {
		
		tinyxml2::XMLElement* l_nNodeSourceData = nullptr;
		
		loadNodeLib((const char*)&_url[1], "library_physics_models", "physics_model", &l_nNodeSourceData);
		tinyxml2::XMLElement* l_nRigidBody = l_nNodeSourceData->FirstChildElement("rigid_body");
		if (l_nRigidBody != nullptr) {
			while (l_nRigidBody != nullptr) {
				
				const char *l_name = l_nRigidBody->Attribute("name");
				const char *l_sid = l_nRigidBody->Attribute("sid");
				
				const char * l_teste = l_nRigidBody->Name();
				std::cout << "DEBUG: " << l_teste <<std::endl;
				
				
				l_nRigidBody = l_nRigidBody->NextSiblingElement("rigid_body");
			}
		}
		
		tinyxml2::XMLElement* l_nRigidBodyConstraint = l_nNodeSourceData->FirstChildElement("rigid_constraint");
		if (l_nRigidBodyConstraint != nullptr) {
			while (l_nRigidBodyConstraint != nullptr) {
				//TODO constraints
				const char *l_name = l_nRigidBodyConstraint->Attribute("name");
				const char *l_sid = l_nRigidBodyConstraint->Attribute("sid");
				
				const char * l_teste = l_nRigidBodyConstraint->Name();
				std::cout << "DEBUG: " << l_teste <<std::endl;
				
				
				l_nRigidBodyConstraint = l_nRigidBodyConstraint->NextSiblingElement("rigid_constraint");
			}
		}
	}
	
	Chimera::Node *LoaderDae::getNodeSceneInFile() {
		
		Chimera::Node *pRootScene = nullptr;
		
		tinyxml2::XMLElement* l_nVisualScene = findSceneLib((const char*)"Visual Scene", (const char*)"instance_visual_scene",(const char*)"library_visual_scenes");
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
					Chimera::loadTransformMatrix(_nNode, l_pTransform);

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

				Chimera::DrawTriMesh *pDrawTriMesh = new Chimera::DrawTriMesh(Chimera::retornaAtributo("id", l_nNodeSourceData), Chimera::retornaAtributo("name", l_nNodeSourceData));
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

				Chimera::Material *pMaterial = new Chimera::Material(Chimera::retornaAtributo("id", l_nNodeSourceData), Chimera::retornaAtributo("name", l_nNodeSourceData));
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
				Chimera::Effect *pEffect = new Chimera::Effect(Chimera::retornaAtributo("id", l_nNodeSourceData), Chimera::retornaAtributo("id", l_nNodeSourceData));
				pEffect->loadCollada(l_nNodeSourceData);
				_pNodePai->addChild(pEffect);

				pLastNodeDone = pEffect;

				if (pEffect->getNameTextureId().size() > 0) {

					loadNodeLib((const char*)pEffect->getNameTextureId().c_str(), "library_images", "image", &l_nNodeSourceData);
					Chimera::Texture *pTexture = new Chimera::Texture(Chimera::retornaAtributo("id", l_nNodeSourceData), Chimera::retornaAtributo("id", l_nNodeSourceData));

					const char* l_val = l_nNodeSourceData->FirstChildElement("init_from")->GetText();
#ifdef WIN32
					pTexture->setPathFile(textureDir + "\\" + std::string(l_val));
#else
					pTexture->setPathFile(textureDir + "/" + std::string(l_val));
#endif
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

	tinyxml2::XMLElement* LoaderDae::findSceneLib(const char *rotina, const char* instance, const char* library) {
		
		tinyxml2::XMLElement* l_nNodeScene = root->FirstChildElement("scene");
		if (l_nNodeScene != nullptr) {
			
			tinyxml2::XMLElement* l_nNode = l_nNodeScene->FirstChildElement(instance);
			if (l_nNode != nullptr) {
				
				const char* l_url = l_nNode->Attribute("url");
				l_nNode = root->FirstChildElement(library);
				if (l_nNode != nullptr) {
					
					l_nNode = l_nNode->FirstChildElement();//TODO colocar um while para percorrer todas as instancias de scena(pode haver mais de 1)
					if (l_nNode != nullptr) {
						while (l_nNode != nullptr) {
							
							const char *l_id = l_nNode->Attribute("id");
							if (strcmp(l_id, (const char*)&l_url[1]) == 0) {
								return l_nNode;
							}
							
							l_nNode = l_nNode->NextSiblingElement();
						}
						
						std::cout << rotina << " : " << (const char*)&l_url[1] << " nao encontrado " << std::endl;
					}
					else {
						std::cout << rotina << " : vazio " << std::endl;
					}
					// 					std::cout << "Visual Scene: " << (const char*)&l_url[1] << " nao encontrado " << std::endl;
				}
				else {
					std::cout << library << " nao encontrado " << std::endl;
				}
			}
			else {
				std::cout << instance << " nao encontrado " << std::endl;
			}
		}
		
		return nullptr;
	}
}
