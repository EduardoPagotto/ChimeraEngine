#include "RenderVisitor.h"

#include "OpenGLDefs.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

RenderVisitor::RenderVisitor() {

	pCoord = nullptr;
	pVideo = nullptr;
	HudOn = true;
	particleOn = true;
	eye = 0;

	projection = glm::mat4(1.0f);
	view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);

    runningShadow = false;

	shadoMap = nullptr;
	//shadoMap = new ShadowMap();

	shader =  Singleton<Shader>::getRefSingleton();
}

RenderVisitor::~RenderVisitor() {

	Singleton<Shader>::releaseRefSingleton();
}

void RenderVisitor::execute(Node * _node, const unsigned &_eye)
{
	if (shadoMap == nullptr) {

		this->eye = _eye;
		HudOn = true;
		particleOn = true;

		DFS(_node);

	}
	else {

		Light *nodeCam = (Light*)_node->findChild(Chimera::EntityKind::LIGHT, 0, true);
		glm::vec3 posicao = nodeCam->getPosition(); //root->findChild(Chimera::EntityKind::CAMERA, 0, true);//getState()->getLight()->getPosition();
		shadoMap->StoreLightMatrices(posicao);
		shadoMap->initSceneShadow();

		HudOn = false;
		particleOn = false;
		runningShadow = true;
		DFS(_node);

		shadoMap->endSceneShadow();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		HudOn = true;
		particleOn = true;
		runningShadow = false;

		DFS(_node);
	}
}

void RenderVisitor::DFS(Node* u)
{
	u->setColor(1);
	u->accept(this);

	std::vector<Node*>* children = u->getChilds();
	if ((children != nullptr) && (children->size() >0)) {

		for (size_t i = 0; i < children->size(); i++) {
			Node* child = children->at(i);
			if (child->getColor() == 0)
				DFS(child);
		}
	}
	u->setColor(0);
}

void RenderVisitor::visit ( Camera* _pCamera ) {

	shader->setGlUniform3fv("viewPos", 1, glm::value_ptr( _pCamera->getPosition() ));

	projection = pVideo->getPerspectiveProjectionMatrix(_pCamera->getFov(), _pCamera->getNear(), _pCamera->getFar(), eye);
	view = _pCamera->getViewMatrix();

}

void RenderVisitor::visit ( Mesh* _pMesh ) {

	int shadows = 0;
	shader->setGlUniform1i("shadows", shadows); //glUniform1i(glGetUniformLocation(shader.Program, "shadows"), shadows);

	// Get the variables from the shader to which data will be passed
	shader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
	shader->setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
	shader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));
	//shader->setGlUniformMatrix3fv("noMat", 1, false, glm::value_ptr( glm::inverseTranspose(glm::mat3(_view))));

	if (runningShadow == false)
		_pMesh->getMaterial()->apply();

    _pMesh->render(projection, view, model);

}

void RenderVisitor::visit ( Light* _pLight ) {

	shader->setGlUniform3fv("light.position", 1, glm::value_ptr(_pLight->getPosition()));
	shader->setGlUniform4fv("light.ambient", 1, _pLight->getAmbient().ptr());
	shader->setGlUniform4fv("light.diffuse", 1, _pLight->getDiffuse().ptr());
	shader->setGlUniform4fv("light.specular", 1, _pLight->getSpecular().ptr());
}

void RenderVisitor::visit ( ParticleEmitter* _pParticleEmitter ) {

	if (particleOn == true) {

		// Get the variables from the shader to which data will be passed
		shader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
		shader->setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
		//shader->setGlUniformMatrix3fv("noMat", 1, false, glm::value_ptr( glm::inverseTranspose(glm::mat3(_view))));

		shader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

		// We will need the camera's position in order to sort the particles
		// w.r.t the camera's distance.
		// There should be a getCameraPosition() function in common/controls.cpp,
		// but this works too.
		glm::vec3 CameraPosition(glm::inverse(view)[3]);

		// Vertex shader
		shader->setGlUniform3f("CameraRight_worldspace", view[0][0], view[1][0], view[2][0]);
		shader->setGlUniform3f("CameraUp_worldspace", view[0][1], view[1][1], view[2][1]);

		// fragment shader
		shader->setGlUniform1i("myTextureSampler", 0);

		_pParticleEmitter->CameraPosition = CameraPosition;
		_pParticleEmitter->render(projection, view, model);
	}

}

void RenderVisitor::visit ( SceneRoot* _pSceneRoot ) {

    _pSceneRoot->apply();

}

void RenderVisitor::visit ( Group* _pGroup ) {

	if (runningShadow == false) {

		shader->selectCurrent(_pGroup->getShaderName());
		shader->link();
//         glActiveTexture(GL_TEXTURE0);
//         glBindTexture(GL_TEXTURE_2D, woodTexture);

 //       glActiveTexture(GL_TEXTURE1);
 //       glBindTexture(GL_TEXTURE_2D, depthMap);

	} else  {


		//shader->setGlUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    }

    

}

void RenderVisitor::visit ( Chimera::Transform* _pTransform) {

	//TODO acumular esta matriz
	model = _pTransform->getModelMatrix(pCoord);
}

void RenderVisitor::visit ( Solid* _pSolid ) {

	//TODO acumular esta matriz
    model = _pSolid->getModelMatrix(pCoord);
}

void RenderVisitor::visit ( HUD* _pHUD ) {

	if (HudOn == true) {
		if (_pHUD->isOn() == true) {

			shader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr( pVideo->getOrthoProjectionMatrix(eye) ));
			_pHUD->render(projection, view, model);
			//pVideo->restoreMatrix();
		}
	}
}

}
