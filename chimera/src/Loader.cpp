#include "Loader.h"

namespace Chimera {

Loader::Loader(Scene *_pScene)
{
	m_pScene = _pScene;
	m_numNodes = 0;
	m_physicWorld = Singleton<PhysicWorld>::getRefSingleton();
}

Loader::~Loader()
{
	Singleton<PhysicWorld>::releaseRefSingleton();
}

bool Loader::exec(const char *_file)
{
	m_doc = xmlParseFile(_file);
	if (m_doc == NULL )
		return false;
	
	m_root = xmlDocGetRootElement(m_doc);
	if (m_root == NULL)
		return false;

	if (xmlStrcmp(m_root->name, (const xmlChar *) "COLLADA"))
	{
		xmlFreeDoc(m_doc);
		return false;
	}

	printf("Cameras Registradas %d\n",libCam());
	printf("Luzes Registradas %d\n",libLight());
	printf("Effects Registrados %d\n",libEffect());
	printf("Images Registradas %d\n",libImage());
	printf("Material Registrados %d\n",libMaterial());
	printf("Geometrias Registradas %d\n",libGeometry());
	printf("Nodes Registrados %d\n",libScene());

	printf("PhysicMaterial Registradas %d\n",libPhysicsMaterial());

	printf("PhysicModels Registradors %d\n",libPhysicsModels());
	printf("PhysicScenes Registradas %d\n",libPhysicsScenes());
	
	return true;
}

void Loader::setIdentity(Node *_pNode, xmlNodePtr _xmlNode)
{
	xmlChar *pName = xmlGetProp(_xmlNode,(const xmlChar*)"name");
	if (pName)
		_pNode->setName((char*)pName);
	else
		_pNode->setName("ERRO");
}

void Loader::loadArrayI(const char *_val, std::vector<int> &_arrayI)
{
	char l_numTemp[32];
	const char *pchFim;
	const char *pchInit = _val;
	const char *deadEnd = _val + strlen(_val);
	do
	{
		pchFim = strchr(pchInit,' ');
		if (pchFim==NULL)
			pchFim = deadEnd;

		int l_tam = pchFim - pchInit;
		memcpy(l_numTemp,pchInit,l_tam);
		l_numTemp[l_tam ] = 0;
		_arrayI.push_back( atoi(l_numTemp) );

		pchInit = pchFim+1;
	}
	while (pchInit < deadEnd);
}

void Loader::loadArrayF(const char *_val, std::vector<float> &_arrayF)
{
	char l_numTemp[32];
	const char *pchFim;
	const char *pchInit = _val;
	const char *deadEnd = _val + strlen(_val);
	do
	{
		pchFim = strchr(pchInit,' ');
		if (pchFim==NULL)
			pchFim = deadEnd;

		int l_tam = pchFim - pchInit;
		memcpy(l_numTemp,pchInit,l_tam);
		l_numTemp[l_tam ] = 0;
		_arrayF.push_back( (float)atof(l_numTemp) );

		pchInit = pchFim+1;
	}
	while (pchInit < deadEnd);
}

xmlNodePtr Loader::findNode(const char* _nomeNode, xmlNodePtr _nodePos)
{
	xmlNodePtr retorno = NULL;
	if (_nodePos!=NULL)
	{
		if (_nodePos->type == XML_ELEMENT_NODE)
		{
			if ( _strcmpi((const char*)_nodePos->name,_nomeNode)==0)
				return _nodePos;
		}

		if (_nodePos->children != NULL)
			retorno = findNode(_nomeNode, _nodePos->children);

		if ( ( retorno == NULL ) && (_nodePos->next != NULL) )
			retorno = findNode(_nomeNode, _nodePos->next);
	}

	return retorno;
}

Color Loader::getPhong(xmlNodePtr _nPos,const char* _name)
{
	Color retorno;

	xmlChar *l_pVal = NULL;
	std::vector<float> l_arrayValores;
	xmlNodePtr l_nPos = _nPos;

	l_nPos = findNode(_name,_nPos->children);
	if (l_nPos)
	{
		l_nPos = findNode((char*)"color",l_nPos->children);
		if (l_nPos)
		{
			l_pVal = xmlNodeListGetString(m_doc, l_nPos->children, 1);
			if (l_pVal)
			{
				loadArrayF((char*)l_pVal ,l_arrayValores);
				retorno.set(l_arrayValores[0],l_arrayValores[1],l_arrayValores[2],l_arrayValores[3]);

				l_arrayValores.clear();
				xmlFree(l_pVal);
			}
		}
	}
	return retorno;
}

int Loader::getSource(xmlNodePtr _nSource, ListPtr<float> &_arrayValores)
{
	int l_numSource = 0;
	xmlNodePtr l_nSource = findNode((char*)"float_array", _nSource);
	if (l_nSource)
	{
		xmlChar* l_pVal = xmlGetProp(l_nSource,(const xmlChar*)"count");
		if (l_pVal)
		{
			l_numSource = atoi((char*)l_pVal);
			l_pVal = xmlNodeListGetString(m_doc, l_nSource->children, 1);
			if (l_pVal)
			{
				std::vector<float> l_array;
				loadArrayF((char*)l_pVal ,l_array);
				if (l_numSource == l_array.size())
				{
					_arrayValores.create(l_numSource);
					for(unsigned l_contador = 0 ; l_contador < l_array.size() ; l_contador++)
						_arrayValores[l_contador] = l_array[l_contador];
				}
				else
					l_numSource = -1; //ERRO
				
				xmlFree(l_pVal);
				l_array.clear();
			}
		}
	}
	return l_numSource;
}

//void Loader::createNode(xmlNodePtr _nodeXML, Node *_pNode)
//{
//	while(_nodeXML)
//	{
//		if (_nodeXML->type==XML_ELEMENT_NODE)
//		{
//			xmlChar *pSID = NULL;
//			xmlChar* l_pVal = NULL;
//			Transform *pTempNode = NULL;
//			std::vector<float> l_arrayValores;
//
//			if (xmlStrcmp(_nodeXML->name,(xmlChar*)"translate")==0)
//			{
//				pSID = xmlGetProp(_nodeXML,(const xmlChar*)"sid");
//				if (xmlStrcmp(pSID,(xmlChar*)"translate")==0)
//				{
//					pTempNode = (Transform*)_pNode;
//
//					l_pVal = xmlNodeListGetString(m_doc, _nodeXML->children, 1);
//					loadArrayF((char*)l_pVal ,l_arrayValores);
//
//					pTempNode->m_position.setValue(l_arrayValores[0], l_arrayValores[1] , l_arrayValores[2] );
//
//					l_arrayValores.clear();
//					xmlFree(l_pVal);
//				}
//			}
//			else if (xmlStrcmp(_nodeXML->name,(xmlChar*)"rotate")==0) //TODO: errado isto é uma matrix e nao 
//			{
//				pTempNode = (Transform*)_pNode;
//				pSID = xmlGetProp(_nodeXML,(const xmlChar*)"sid");
//
//				l_pVal = xmlNodeListGetString(m_doc, _nodeXML->children, 1);
//				loadArrayF((char*)l_pVal ,l_arrayValores);
//
//				//ultimo elemento apenas
//				if (xmlStrcmp(pSID,(xmlChar*)"rotateZ")==0)
//					pTempNode->m_rotation.setZ(l_arrayValores[3]);
//				else if (xmlStrcmp(pSID,(xmlChar*)"rotateY")==0)
//					pTempNode->m_rotation.setY(l_arrayValores[3]);
//				else if (xmlStrcmp(pSID,(xmlChar*)"rotateX")==0)
//					pTempNode->m_rotation.setX(l_arrayValores[3]);
//
//				l_arrayValores.clear();
//				xmlFree(l_pVal);
//			}
//			else if (xmlStrcmp(_nodeXML->name,(xmlChar*)"scale")==0)
//			{
//				pTempNode = (Transform*)_pNode;
//				pSID = xmlGetProp(_nodeXML,(const xmlChar*)"sid");
//				if (xmlStrcmp(pSID,(xmlChar*)"scale")==0)
//				{
//					l_pVal = xmlNodeListGetString(m_doc, _nodeXML->children, 1);
//					loadArrayF((char*)l_pVal ,l_arrayValores);
//					pTempNode->m_scale.setValue( l_arrayValores[0] , l_arrayValores[1] , l_arrayValores[2] );
//					l_arrayValores.clear();
//					xmlFree(l_pVal);
//				}
//
//			}
//			else if (xmlStrcmp(_nodeXML->name,(xmlChar*)"instance_geometry")==0)
//			{
//				Object *pObj = new Object;
//				m_numNodes++;
//				
//				m_pScene->m_vObject.push_back(pObj);
//
//				xmlChar *pURL = xmlGetProp(_nodeXML,(const xmlChar*)"url");
//				if (pURL != NULL)
//				{
//					std::string l_url = (char*)&pURL[1];
//					pObj->setName(l_url + std::string("-OBJ"));
//					_pNode->add(pObj);
//
//					Mesh *pMesh = (Mesh*)m_pScene->m_mDraw[l_url];//(Mesh*)m_pScene->findInList(l_url,DRAW);
//					if (pMesh)
//					{
//						pObj->m_pDraw = pMesh;
//						
//						//Carrega Materia se existir
//						if (_nodeXML->children)
//						{
//							xmlNodePtr l_nBindMat = findNode((char*)"bind_material", _nodeXML->children);
//							if (l_nBindMat)
//							{
//								l_nBindMat = findNode((char*)"technique_common", l_nBindMat->children);
//								if (l_nBindMat)
//								{
//									l_nBindMat = findNode((char*)"instance_material", l_nBindMat->children);
//									if (l_nBindMat)
//									{
//										pURL = xmlGetProp(l_nBindMat,(const xmlChar*)"target");
//										if (pURL)
//										{
//											Material *pMat = m_pScene->m_mMaterial[(char*)&pURL[1]];
//											if (pMat)
//											{
//												pMesh->m_pMaterial = pMat;
//											}
//										}
//									}
//								}
//							}
//						}
//					}
//				}
//			}
//			else if (xmlStrcmp(_nodeXML->name,(xmlChar*)"instance_light")==0)
//			{
//				xmlChar *pURL = xmlGetProp(_nodeXML,(const xmlChar*)"url");
//				if (pURL != NULL)
//				{
//					std::string l_url = (char*)&pURL[1];
//					Light *pLight = (Light*)m_pScene->findInList(l_url,LIGHT);
//					if (pLight!=NULL)
//						_pNode->add(pLight);
//				}
//			}
//			else if (xmlStrcmp(_nodeXML->name,(xmlChar*)"instance_camera")==0)
//			{
//				xmlChar *pURL = xmlGetProp(_nodeXML,(const xmlChar*)"url");
//				if (pURL != NULL)
//				{
//					std::string l_url = (char*)&pURL[1];
//					Camera *pCam = (Camera*)m_pScene->findInList(l_url,CAMERA);
//					if (pCam!=NULL)
//						_pNode->add(pCam);
//				}
//			}
//			else if (xmlStrcmp(_nodeXML->name,(xmlChar*)"node")==0)
//			{
//				m_numNodes++;
//				Node *l_pNodeParent = _pNode;
//				_pNode = new Transform;
//
//				setIdentity(_pNode,_nodeXML);
//
//				l_pNodeParent->add(_pNode);
//				
//				createNode(_nodeXML->children->next,_pNode);
//				_pNode = l_pNodeParent;
//			}
//		}
//		_nodeXML = _nodeXML->next;
//	}
//}

int Loader::libCam(void)
{
	int l_count = 0;

	//Pega Raiz de Elemento conjunto Cameras
	xmlNodePtr l_nCam = findNode((char*)"library_cameras", m_root);

	//Encontra o primeiro elemento
	l_nCam = findNode((char*)"Camera", l_nCam->children);	
	while (l_nCam != NULL)
	{
		//Cria camera
		Camera *pCamera = new Camera;

		//Carrega atributos
		setIdentity(pCamera,l_nCam);
		
		//Salva na lista de cameras
		m_pScene->m_vCameras.push_back(pCamera);
		
		//Verifica se exite Elemento Perspectiva
		xmlNodePtr l_nTech = findNode((char*)"perspective",l_nCam);
		if (l_nTech!=NULL)
		{
			pCamera->m_perspective = true;

			xmlChar* l_pVal = NULL;
			xmlNodePtr l_nElem;

			l_nElem = findNode((char*)"yfov",l_nTech);
			if (l_nElem!=NULL)
			{
				l_pVal = xmlNodeListGetString(m_doc, l_nElem->children, 1);
				pCamera->m_fov = (float)atof((char*)l_pVal);
				xmlFree(l_pVal);				
			}

			l_nElem = findNode((char*)"znear",l_nTech);
			if (l_nElem!=NULL)
			{
				l_pVal = xmlNodeListGetString(m_doc, l_nElem->children, 1);
				pCamera->m_near = (float)atof((char*)l_pVal);
				xmlFree(l_pVal);				
			}

			l_nElem = findNode((char*)"zfar",l_nTech);
			if (l_nElem!=NULL)
			{
				l_pVal = xmlNodeListGetString(m_doc, l_nElem->children, 1);
				pCamera->m_far = (float)atof((char*)l_pVal);
				xmlFree(l_pVal);				
			}

		}
		
		l_nTech = findNode((char*)"orthogonal",l_nCam);
		if (l_nTech!=NULL)
		{
			//TODO: implementar
		}

		l_nCam = findNode((char*)"Camera", l_nCam->next);	
		l_count++;
	}
	return l_count;
}

int Loader::libLight(void)
{
	int l_count = 0;

	//Pega Raiz de Elemento conjunto Luzes
	xmlNodePtr l_nLight = findNode((char*)"library_lights", m_root);
	if (l_nLight!=NULL)
	{
		xmlChar* l_pVal = NULL;
		std::vector<float> l_arrayValores;
		l_nLight = findNode((char*)"light", l_nLight->children);
		while(l_nLight!=NULL)
		{
			//Carrega atributos
			Light *pLight = new Light(POINT, l_count);
			setIdentity(pLight,l_nLight);
			m_pScene->m_vLights.push_back(pLight);

			xmlNodePtr l_nTipo = findNode((char*)"point", l_nLight->children);
			if (l_nTipo != NULL)
			{
				l_nTipo = findNode((char*)"color", l_nTipo->children);
				if (l_nTipo!=NULL)
				{				
					l_pVal = xmlNodeListGetString(m_doc, l_nTipo->children, 1);
					
					loadArrayF((char*)l_pVal ,l_arrayValores);
					pLight->ambient.set( l_arrayValores[0] , l_arrayValores[1] , l_arrayValores[2], 1.0f );

					l_arrayValores.clear();

					xmlFree(l_pVal);
				}
			}
			l_nLight = findNode((char*)"light", l_nLight->next);	
			l_count++;
		}
	}
	return l_count;
}

int Loader::libEffect(void)
{
	int l_count = 0;
	xmlNodePtr l_nMat = findNode((char*)"library_effects", m_root);
	if (l_nMat)
	{
		l_nMat = findNode((char*)"effect",l_nMat->children);
		while (l_nMat != NULL)
		{
			l_count++;
			Effect *pMat = new Effect;

			xmlChar *pName = xmlGetProp(l_nMat,(const xmlChar*)"name");
			m_pScene->m_mEffect[(const char*)pName] = pMat;

			//pega referencia textura
			xmlNodePtr l_nTex = findNode((char*)"init_from",l_nMat->children);
			if (l_nTex)
			{
				xmlChar *l_pVal = NULL;
				l_pVal = xmlNodeListGetString(m_doc, l_nTex->children, 1);
				if (l_pVal)
				{
					pMat->m_nameTexture = (char*)l_pVal;
					xmlFree(l_pVal);
				}				
			}

			//pega cores
			xmlNodePtr l_nPos = findNode((char*)"phong",l_nMat->children);
			if (l_nPos)
			{
				xmlNodePtr l_nRootPhong = l_nPos;
				pMat->m_emissive.set(getPhong(l_nPos,(char*)"emission"));
				pMat->m_ambient.set(getPhong(l_nPos,(char*)"ambient"));
				pMat->m_diffuse.set(getPhong(l_nPos,(char*)"diffuse"));
				pMat->m_specular.set(getPhong(l_nPos,(char*)"specular"));

				l_nPos = findNode((char*)"shininess",l_nPos->children);
				if (l_nPos)
				{
					l_nPos = findNode((char*)"float",l_nPos->children);
					if (l_nPos)
					{
						xmlChar *l_pVal = NULL;
						l_pVal = xmlNodeListGetString(m_doc, l_nPos->children, 1);
						if (l_pVal)
						{
							pMat->m_shininess = (float)atof((char*)l_pVal);
							xmlFree(l_pVal);
						}
					}
				}
			}
			l_nMat = findNode((char*)"effect", l_nMat->next);	
		}
	}
	return l_count;
}

int Loader::libImage(void)
{
	int l_count = 0;
	xmlNodePtr l_nImage = findNode((char*)"library_images", m_root);
	if(l_nImage)
	{
		l_nImage = findNode((char*)"image", l_nImage->children);
		while (l_nImage)
		{
			Image *pImage = new Image;
			xmlChar *pName = xmlGetProp(l_nImage,(const xmlChar*)"name");

			m_pScene->m_mImage[(const char*)pName] = pImage;

			xmlNodePtr l_nFile = findNode((char*)"init_from", l_nImage->children);
			if (l_nFile)
			{
				xmlChar *l_pVal = xmlNodeListGetString(m_doc, l_nFile->children, 1);
				if (l_pVal)
				{
					if (pImage->load((char*)l_pVal)==0)
						l_count++;
					
					xmlFree(l_pVal);					
				}
			}
			l_nImage = findNode((char*)"image", l_nImage->next);
		}
	}
	return l_count;
}

int Loader::libMaterial(void)
{
	int l_count = 0;
	xmlNodePtr l_nMat = findNode((char*)"library_materials", m_root);
	if (l_nMat)
	{
		l_nMat = findNode((char*)"material", m_root);
		while (l_nMat)
		{
			l_count++;

			Material *pMat = new Material;
			xmlChar *pName = xmlGetProp(l_nMat,(const xmlChar*)"name");
			m_pScene->m_mMaterial[(const char*)pName] = pMat;

			xmlNodePtr l_nEffect = findNode((char*)"instance_effect", l_nMat->children);//TODO:COrrigir ha um bug aqui ele passa 2 vezes
			if (l_nEffect)
			{
				xmlChar *l_val = xmlGetProp(l_nEffect,(const xmlChar*)"url");
				if (l_val)
				{
					Effect *pEffe = m_pScene->m_mEffect[(char*)&l_val[1]];
					if (pEffe)
					{
						pMat->m_effect = (*pEffe);
						if (pEffe->m_nameTexture.size() > 0)
							pMat->m_pTexture = m_pScene->m_mImage[pEffe->m_nameTexture];
					}
				}
			}
			l_nMat = findNode((char*)"material", l_nMat->next);	
		}
	}
	return l_count;
}

int Loader::libGeometry(void)
{
	int l_count = 0;

	xmlNodePtr l_nGeom = findNode((char*)"library_geometries", m_root);
	if (l_nGeom!=NULL)
	{
		char *l_pVal = NULL;
		l_nGeom = findNode((char*)"geometry", l_nGeom->children);
		while(l_nGeom!=NULL)
		{
			Mesh *pMesh = new Mesh;
			xmlChar *pName = xmlGetProp(l_nGeom,(const xmlChar*)"name");
			m_pScene->m_mDraw[(const char*)pName] = pMesh;

			xmlNodePtr l_nMesh = findNode((char*)"mesh", l_nGeom->children);
			if (l_nMesh)
			{
				l_nMesh = findNode((char*)"source",l_nMesh->children);
				while (l_nMesh)
				{
					l_pVal = (char*)xmlGetProp(l_nMesh,(const xmlChar*)"id");
					if (strstr(l_pVal,(char*)"-Position")!=NULL)
						getSource(l_nMesh,pMesh->m_vertexList);
					else if (strstr(l_pVal,(char*)"-Normals")!=NULL)
						getSource(l_nMesh,pMesh->m_normalList);
					else if (strstr(l_pVal,(char*)"-UV")!=NULL)
						getSource(l_nMesh,pMesh->m_uv);

					l_nMesh = findNode((char*)"source",l_nMesh->next);
				}

				xmlNodePtr l_nMesh = findNode((char*)"triangles", l_nGeom->children);
				if (l_nMesh)
				{
					l_pVal = (char*)xmlGetProp(l_nMesh,(const xmlChar*)"count");
					char *l_pMaterialFile = (char*)xmlGetProp(l_nMesh,(const xmlChar*)"material");

					if (l_pVal)
					{
						xmlNodePtr l_nAnterior;
						unsigned l_numTriangles = atoi(l_pVal);
						l_nMesh = findNode((char*)"input", l_nMesh->children);

						std::vector<char*> l_vOffset;
						std::vector<char*> l_vSemantic;
						std::vector<char*> l_vSource;

						while (l_nMesh)
						{
							char *l_offSet = (char*)xmlGetProp(l_nMesh,(const xmlChar*)"offset");
							char *l_semantic = (char*)xmlGetProp(l_nMesh,(const xmlChar*)"semantic");
							char *l_source = (char*)xmlGetProp(l_nMesh,(const xmlChar*)"source");

							l_vOffset.push_back(l_offSet);
							l_vSemantic.push_back(l_semantic);
							l_vSource.push_back(l_source);

							l_nAnterior = l_nMesh;

							l_nMesh = findNode((char*)"input",l_nMesh->next);
						}

						l_nMesh = findNode((char*)"p", l_nAnterior);
						if (l_nMesh)
						{
							l_pVal = (char*)xmlNodeListGetString(m_doc, l_nMesh->children, 1);
							if (l_pVal)
							{
								std::vector<int> l_arrayIndex;
								loadArrayI(l_pVal,l_arrayIndex);

								unsigned l_veCount = 0;
								unsigned l_noCount = 0;
								unsigned l_uvCount = 0;

								for (unsigned l_contador = 0 ; l_contador < l_arrayIndex.size() ; l_contador++ )
								{
									int index = l_contador % l_vOffset.size();

									char *l_offSet = l_vOffset[index];
									char *l_semantic = l_vSemantic[index];
									char *l_source = l_vSource[index];
									if (strstr(l_source,(char*)"-Vertex") != NULL)
									{
										if (pMesh->m_vertexIndex.size() == 0)
											pMesh->m_vertexIndex.create(l_numTriangles * 3);

										pMesh->m_vertexIndex[l_veCount] = l_arrayIndex[l_contador];
										l_veCount++;
									}
									else if (strstr(l_source,(char*)"-Normals") != NULL)
									{
										if (pMesh->m_normalIndex.size()==0)
											pMesh->m_normalIndex.create(l_numTriangles * 3);

										pMesh->m_normalIndex[l_noCount] = l_arrayIndex[l_contador];
										l_noCount++;
									}
									else if (strstr(l_source,(char*)"-UV") != NULL)
									{
										if (pMesh->m_textureIndex.size()==0)
											pMesh->m_textureIndex.create(l_numTriangles* 3);

										pMesh->m_textureIndex[l_uvCount] = l_arrayIndex[l_contador];
										l_uvCount++;
									}
								}
								l_arrayIndex.clear();
							}
							xmlFree((xmlChar*)l_pVal);
						}
						l_vOffset.clear();
						l_vSemantic.clear();
						l_vSource.clear();
					}
				}
				//pObj->setMesh((*pMesh));
			}
			l_nGeom = findNode((char*)"geometry", l_nGeom->next);	
			l_count++;
		}
	}
	return l_count;
}

void Loader::createNode(xmlNodePtr _nodeXML, Node *_pNode)
{
	xmlNodePtr l_nNode = _nodeXML;//findNode((char*)"node", _nodeXML->children);

	while(l_nNode)
	{
		if (l_nNode->type==XML_ELEMENT_NODE)
		{
			xmlChar* l_pVal = NULL;
			xmlChar *pSID = NULL;
			Transform l_trans;
			std::vector<float> l_arrayValores;
			Node *pFilho = NULL;

			const xmlChar *l_pNomeNode = l_nNode->name;
			xmlChar *l_pName = xmlGetProp(l_nNode,(const xmlChar*)"name");

			xmlNodePtr l_nTransforma = findNode((char*)"translate", l_nNode->children);

			if (l_nTransforma)
			{
				pSID = xmlGetProp(l_nTransforma,(const xmlChar*)"sid");
				if (xmlStrcmp(pSID,(xmlChar*)"translate")==0)
				{
					l_pVal = xmlNodeListGetString(m_doc, l_nTransforma->children, 1);
					loadArrayF((char*)l_pVal ,l_arrayValores);
					l_trans.position.setValue(l_arrayValores[0], l_arrayValores[1] , l_arrayValores[2] );

					l_arrayValores.clear();
					xmlFree(l_pVal);
				}
			}

			l_nTransforma = findNode((char*)"rotate", l_nNode->children);//rotate
			while (l_nTransforma)
			{
				pSID = xmlGetProp(l_nTransforma,(const xmlChar*)"sid");
				l_pVal = xmlNodeListGetString(m_doc, l_nTransforma ->children, 1);
				loadArrayF((char*)l_pVal ,l_arrayValores);

				//ultimo elemento apenas
				if (xmlStrcmp(pSID,(xmlChar*)"rotateZ")==0)
					l_trans.rotation.setZ(l_arrayValores[3]);
				else if (xmlStrcmp(pSID,(xmlChar*)"rotateY")==0)
					l_trans.rotation.setY(l_arrayValores[3]);
				else if (xmlStrcmp(pSID,(xmlChar*)"rotateX")==0)
				{
					l_trans.rotation.setX(l_arrayValores[3]);
					l_arrayValores.clear();
					break;
				}

				l_arrayValores.clear();

				l_nTransforma = findNode((char*)"rotate",l_nTransforma->next);
			}

			l_nTransforma = findNode((char*)"scale", l_nNode->children);
			if (l_nTransforma)
			{
				pSID = xmlGetProp(l_nTransforma,(const xmlChar*)"sid");
				if (xmlStrcmp(pSID,(xmlChar*)"scale")==0)
				{
					l_pVal = xmlNodeListGetString(m_doc, l_nTransforma->children, 1);
					loadArrayF((char*)l_pVal ,l_arrayValores);
					l_trans.scale.setValue(l_arrayValores[0], l_arrayValores[1] , l_arrayValores[2] );

					l_arrayValores.clear();
					xmlFree(l_pVal);
				}
			}

			xmlNodePtr l_nInstance;;
			if (l_nInstance = findNode((char*)"instance_geometry", l_nNode->children))
			{
				Object *pObj = new Object;
				pObj->setName((const char*)l_pName);

				m_pScene->m_vObject.push_back(pObj);
				_pNode->add(pObj); 
				pObj->m_trans = l_trans;

				pFilho = pObj;
				m_numNodes++;

				xmlChar *pURL = xmlGetProp(l_nInstance,(const xmlChar*)"url");
				if (pURL != NULL)
				{
					Mesh *pMesh = (Mesh*)m_pScene->m_mDraw[((char*)&pURL[1])];
					if (pMesh)
					{
						pObj->m_pDraw = pMesh;
						
						//Carrega Material se existir
						if (l_nInstance->children)
						{
							xmlNodePtr l_nBindMat = findNode((char*)"bind_material", l_nInstance->children);
							if (l_nBindMat)
							{
								l_nBindMat = findNode((char*)"technique_common", l_nBindMat->children);
								if (l_nBindMat)
								{
									l_nBindMat = findNode((char*)"instance_material", l_nBindMat->children);
									if (l_nBindMat)
									{
										pURL = xmlGetProp(l_nBindMat,(const xmlChar*)"target");
										if (pURL)
										{
											Material *pMat = m_pScene->m_mMaterial[(char*)&pURL[1]];
											if (pMat)
											{
												pMesh->m_pMaterial = pMat;
											}
										}
									}
								}
							}
						}
					}
				}
			}
			else if (l_nInstance = findNode((char*)"instance_light", l_nNode->children))
			{
				xmlChar *pURL = xmlGetProp(l_nInstance,(const xmlChar*)"url");
				if (pURL != NULL)
				{
					Light *pLight = (Light*)m_pScene->findInList((char*)&pURL[1],LIGHT);
					if (pLight!=NULL)
					{
						_pNode->add(pLight);
						pFilho = pLight;
						pLight->m_trans = l_trans;
					}
				}
			}
			else if (l_nInstance = findNode((char*)"instance_camera", l_nNode->children))
			{
				xmlChar *pURL = xmlGetProp(l_nInstance,(const xmlChar*)"url");
				if (pURL != NULL)
				{
					Camera *pCam = (Camera*)m_pScene->findInList( (char*)&pURL[1],CAMERA);
					if (pCam!=NULL)
					{
						_pNode->add(pCam);
						pFilho = pCam;
						pCam->m_trans = l_trans;
					}
				}
			}

			Node* l_buffer = _pNode;
			createNode(l_nInstance->next, pFilho);
			_pNode = l_buffer;
		}
		l_nNode = l_nNode->next;
	}
}

int Loader::libScene(void)
{	
	xmlNodePtr l_nScene = findNode((char*)"library_visual_scenes", m_root);
	if (l_nScene!=NULL)
	{
		l_nScene = findNode((char*)"visual_scene", l_nScene->children);
		if (l_nScene!=NULL)
		{
			xmlNodePtr l_nNode = findNode((char*)"node",l_nScene->children);
			createNode(l_nNode, m_pScene);
		}
	}

	return m_numNodes;
}

int Loader::libPhysicsMaterial(void)
{
	unsigned l_num = 0;
	xmlNodePtr l_nPhMat = findNode((char*)"library_physics_materials", m_root);
	if (l_nPhMat)
	{
		l_nPhMat =  findNode((char*)"physics_material",l_nPhMat->children);
		while (l_nPhMat)
		{
			l_num++;
			xmlChar *l_pName = xmlGetProp(l_nPhMat,(const xmlChar*)"name");

			btMaterial *pPMat = new btMaterial;
			m_pScene->m_PMaterial[(const char*)l_pName] = pPMat;

			xmlNodePtr l_nTec =  findNode((char*)"technique_common",l_nPhMat->children);
			if (l_nTec)
			{
				xmlNodePtr l_nFric =  findNode((char*)"dynamic_friction",l_nTec->children);
				if (l_nFric)
				{
					xmlChar *l_pVal = xmlNodeListGetString(m_doc, l_nFric->children, 1);
					if (l_pVal)
					{
						pPMat->m_friction = atof((const char*)l_pVal);
						xmlFree(l_pVal);
					}
				}

				l_nFric =  findNode((char*)"restitution",l_nTec->children);
				if (l_nFric)
				{
					xmlChar *l_pVal = xmlNodeListGetString(m_doc, l_nFric->children, 1);
					if (l_pVal)
					{
						pPMat->m_restitution = atof((const char*)l_pVal);
						xmlFree(l_pVal);
					}
				}
			}

			l_nPhMat =  findNode((char*)"physics_material",l_nPhMat->next);
		}
	}
	return l_num;
}

int Loader::libPhysicsModels(void)
{
	int l_count = 0;
	xmlNodePtr l_nPhysics = findNode((char*)"library_physics_models", m_root);
	if (l_nPhysics)
	{
		l_nPhysics = findNode((char*)"physics_model", l_nPhysics->children);
		if (l_nPhysics)
		{
			l_nPhysics = findNode((char*)"rigid_body", l_nPhysics);
			while (l_nPhysics)
			{
				xmlChar *l_pName = xmlGetProp(l_nPhysics,(const xmlChar*)"name");
				xmlChar *l_pSid = xmlGetProp(l_nPhysics,(const xmlChar*)"sid");
				if ((l_pName)&&(l_pSid))
				{
					Physics *pPhysc = new Physics;
					m_pScene->m_mPhysics[(const char*)l_pName] = pPhysc;

					xmlNodePtr l_nTec = findNode((char*)"technique_common", l_nPhysics->children);
					if (l_nTec)
					{
						xmlNodePtr l_nProp = findNode((char*)"dynamic", l_nTec->children);
						if (l_nProp)
						{
							xmlChar *l_pVal = xmlNodeListGetString(m_doc, l_nProp->children, 1);
							if (l_pVal)
							{
								if ( strcmp((const char*)l_pVal,(const char*)"true")==0)
								{
									l_nProp = findNode((char*)"mass", l_nProp);
									if (l_nProp)
									{
										xmlFree(l_pVal);
										l_pVal = xmlNodeListGetString(m_doc, l_nProp->children, 1);
										if (l_pVal)
											pPhysc->setMass((float)atof((char*)l_pVal));
									}
								}

								xmlFree(l_pVal);
							}
						}

						l_nProp = findNode((char*)"instance_physics_material", l_nTec->children);
						if (l_nProp)
						{
							xmlChar *l_pName = xmlGetProp(l_nProp,(const xmlChar*)"url");
							if (l_pName)
							{
								btMaterial *pPMat = m_pScene->m_PMaterial[(const char*)&l_pName[1]];
								if (pPMat)
									pPhysc->m_pPhysicMaterial = pPMat;
							}
						}



					}
				}
				l_nPhysics = findNode((char*)"rigid_body", l_nPhysics->next);
			}
		}
	}
	return l_count;
}

int Loader::libPhysicsScenes(void)
{
	unsigned l_num=0;
	xmlNodePtr l_nPhyScene = findNode((char*)"library_physics_scenes", m_root);
	if (l_nPhyScene!=NULL)
	{
		l_nPhyScene = findNode((char*)"instance_physics_model", l_nPhyScene->children);
		if (l_nPhyScene)
		{
			xmlNodePtr l_nRigid = findNode((char*)"instance_rigid_body", l_nPhyScene->children);
			while (l_nRigid)
			{
				xmlChar *l_pBody = xmlGetProp(l_nRigid,(const xmlChar*)"body");
				xmlChar *l_pTarget = xmlGetProp(l_nRigid,(const xmlChar*)"target");
				if ((l_pBody)&&(l_pTarget))
				{
					Physics *pPhysic = m_pScene->m_mPhysics[(const char*)l_pBody];
					Object *pObj = (Object*)m_pScene->findInList((const char*)&l_pTarget[1],OBJECT);

					if ((pPhysic)&&(pObj))
					{
						Mesh *pMesh = (Mesh*)pObj->m_pDraw;//todo pode ser o draw
						if (pMesh)
							pPhysic->setShapeBox( pMesh->sizeQuadratic());

						pObj->m_pPhysic = pPhysic;
						l_num++;
					}
				}
				l_nRigid = findNode((char*)"instance_rigid_body",l_nRigid->next);
			}
		}
	}

	return l_num;
}

}