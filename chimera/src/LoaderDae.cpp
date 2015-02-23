#include <iostream>

#include "LoaderDae.h"

#include "Node.h"
#include "ExceptionChimera.h"



namespace ChimeraLoader {

	LoaderDae::LoaderDae(const std::string &textureDir, const std::string &file){
	
		doc = nullptr;
		root = nullptr;
		this->textureDir = textureDir;
		
		doc = new tinyxml2::XMLDocument();
		if (doc->LoadFile(file.c_str()) != 0)
			throw Chimera::ExceptionChimera(Chimera::ExceptionCode::OPEN,"Falha ao ler arquivo");
			

		root = doc->FirstChildElement("COLLADA");
		if (root == nullptr)
			throw Chimera::ExceptionChimera(Chimera::ExceptionCode::OPEN,"Nao é um arquivo colada");
		
		
		tinyxml2::XMLElement* l_nVisualScene = findVisualScene();
		if (l_nVisualScene != nullptr) {
			
			const char *l_nome = l_nVisualScene->Attribute("name");
			const char *l_id = l_nVisualScene->Attribute("id");
			
			pRootScene = new Chimera::Node(Chimera::EntityKind::NODE,l_id, l_nome);
			tinyxml2::XMLElement* l_nNode = l_nVisualScene->FirstChildElement("node");
			if (l_nNode != nullptr) {
				while (l_nNode != nullptr) {
					
					const char * l_id = l_nNode->Attribute("id");
					const char * l_name = l_nNode->Attribute("name");
					const char * l_type = l_nNode->Attribute("type");
					
					tinyxml2::XMLElement* l_nDadoNode = l_nNode->FirstChildElement();
					if (l_nDadoNode != nullptr) {
						
						carregaNode(l_nDadoNode, l_id, l_name, l_type);
						
					} else {
						
						std::cout << "Node sem filho" << std::endl;
						
					}
					
					l_nNode = l_nNode->NextSiblingElement("node");
				}
			} else {
				
				std::cout << "Node: vazio " << std::endl;
				
			}
		}
	}

	LoaderDae::~LoaderDae(){
	
		if (doc != nullptr) {
			delete doc;
			doc = nullptr;
		}
	
	}
	
	void LoaderDae::carregaNode(tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char* type) {
		
		btTransform *l_pTransform = nullptr;
		
		Chimera::Node *pNodeAtual = pRootScene;
		
		while(_nNode != nullptr) {
			const char* l_nomeElemento = _nNode->Value();
			
			if (strcmp(l_nomeElemento,(const char*)"matrix")==0) {

				const char* l_tipoTransform = _nNode->Attribute("sid");
				if (strcmp(l_tipoTransform,(const char*)"transform")==0) {
					
					l_pTransform = new btTransform;				
					loadTransformMatrix(_nNode,l_pTransform);

				}else {
					//TODO: implementar carga de posicao, rotacao e transformar em matricial em l_pTransform
				}
				
			} else if (strcmp(l_nomeElemento,(const char*)"instance_camera")==0) {
				
				const char* l_url = _nNode->Attribute("url");
				Chimera::Camera *pCamera = nullptr;
				loadCamera((const char*)&l_url[1], &pCamera);
				pCamera->setTransform(*l_pTransform);
				pNodeAtual->addChild(pCamera);
				
				
			} else if (strcmp(l_nomeElemento,(const char*)"instance_light")==0) {
				
				const char* l_url = _nNode->Attribute("url");
				Chimera::Light *pLight = nullptr;
				loadLight((const char*)&l_url[1], &pLight);
				pLight->setTransform(*l_pTransform);
				pNodeAtual->addChild(pLight);
				
			} else if (strcmp(l_nomeElemento,(const char*)"instance_geometry")==0) {
				
				const char* l_url = _nNode->Attribute("url");
				Chimera::DrawTriMesh *pDrawTriMesh = nullptr;
				loadTriMesh((const char*)&l_url[1], &pDrawTriMesh);
				//pDrawTriMesh->setTransform(*l_pTransform); FIXME: AQUI
				pNodeAtual->addChild(pDrawTriMesh);
				
// 				tinyxml2::XMLElement* l_nNode = root->FirstChildElement("library_geometries");
// 				if (l_nNode != nullptr) {
// 					l_nNode = l_nNode->FirstChildElement("geometry");
// 					while (l_nNode != nullptr) {
// 						
// 						DrawTriMesh *pDrawTriMesh = new DrawTriMesh(retornaAtributo("id", l_nNode), retornaAtributo("name", l_nNode));
// 						listaNodeRemover.push(pDrawTriMesh);
// 						
// 						pDrawTriMesh->loadCollada(l_nNode);
// 						
// 						l_nNode = l_nNode->NextSiblingElement("geometry");
// 					}
// 				}
				
				
			} else if (strcmp(l_nomeElemento,(const char*)"node")==0) {
			} else  {
			}
			
		
			_nNode = _nNode->NextSiblingElement();
		}
		
		
	}
	
	void LoaderDae::loadLight(const char* _id, Chimera::Light **_ppLight) {
		
		tinyxml2::XMLElement* l_nLibLight = root->FirstChildElement("library_lights");
		if (l_nLibLight != nullptr) {
			
			tinyxml2::XMLElement* l_nLight = l_nLibLight->FirstChildElement("light");
			if (l_nLight != nullptr) {
				
				while (l_nLight != nullptr) {
					
					const char *l_id = l_nLight->Attribute("id");
					if (strcmp(l_id, _id)==0) {
						
						*_ppLight = new Chimera::Light(Chimera::LightType::POINT, retornaAtributo("id", l_nLight), retornaAtributo("name", l_nLight));
						(*_ppLight)->loadCollada(l_nLight);
						return;
					}
					l_nLight = l_nLight->NextSiblingElement();
				}
			} else {
				throw Chimera::ExceptionChimera(Chimera::ExceptionCode::READ,"Falha, nao encontrado: Light");
			}
		} else {
			throw Chimera::ExceptionChimera(Chimera::ExceptionCode::READ,"Falha, nao encontrado: library_lights");
		}
		
	}
	
	void LoaderDae::loadCamera(const char* _id, Chimera::Camera **_ppCam) {
		
		tinyxml2::XMLElement* l_nLibCam = root->FirstChildElement("library_cameras");
		if (l_nLibCam != nullptr) {
			
			tinyxml2::XMLElement* l_nCam = l_nLibCam->FirstChildElement("camera");
			if (l_nCam != nullptr) {
				
				while (l_nCam != nullptr) {
					
					const char *l_id = l_nCam->Attribute("id");
					if (strcmp(l_id, _id)==0) {
						
						*_ppCam = new Chimera::Camera(Chimera::CameraType::Base, l_id, retornaAtributo("name", l_nCam));
						(*_ppCam)->loadCollada(l_nCam);
						return;
					}
					
					l_nCam = l_nCam->NextSiblingElement();
				}
				
			} else {
				throw Chimera::ExceptionChimera(Chimera::ExceptionCode::READ,"Falha, nao encontrado: camera");
			}
			
		} else {
			throw Chimera::ExceptionChimera(Chimera::ExceptionCode::READ,"Falha, nao encontrado: library_cameras");
		}
		
	}
	
	std::string LoaderDae::retornaAtributo(const std::string &_atributo, tinyxml2::XMLElement* _node) {
		
		const char *l_value = _node->Attribute(_atributo.c_str());
		if (l_value != nullptr) {
			return std::string(l_value);
		}
		
		std::cout << std::string ( "Atributo [ " + _atributo + " ] Elemento [ " + _node->Value() + " ] nao encontrado") << std::endl;
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
							if (strcmp(l_id,(const char*)&l_url[1])==0) {
								return l_nNode;
							}
							
							l_nNode = l_nNode->NextSiblingElement();
						}
						
						std::cout << "Visual Scene: " << (const char*)&l_url[1] << " nao encontrado " << std::endl;
						
					} else {
						
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
