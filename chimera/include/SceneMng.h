#ifndef SCENE_MNG_H_
#define SCENE_MNG_H_

#include <vector>
#include "ExceptionSDL.h"
#include "Camera.h"
#include "Object.h"
#include "Light.h"

namespace Chimera {

	/**
	 * Class SceneMng
	 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
	 *  @since 20130925
	 */
	class SceneMng {
	public:
		SceneMng(Node *_pRoot);
		virtual ~SceneMng();

		void addChildToScene(Node *_pNode);

		Node *getNode(EntityKind _type, unsigned index);

		void update(DataMsg *dataMsg);

		Node* getRoot() {
			return pRoot;
		}

		void cameraAtiva(Camera *_pCam) {
			pCameraAtiva = _pCam;
		}

		void objetoAtivo(Object *_pObject) {
			pObjeto = _pObject;
		}

		void draw3d();

	private:
		void execLight();
		void parseEntity(Node *_pNode);
		void addEntityToScene(Node *_pNode);

		bool hasLight;
		bool hasMaterial;

		Node *pRoot;

		Camera *pCameraAtiva;
		Object *pObjeto;

		std::vector<Camera*> m_vCamera;
		std::vector<Light*> m_vLight;
		std::vector<Object*> m_vObject;

	};

} /* namespace Chimera */
#endif /* GRADE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
