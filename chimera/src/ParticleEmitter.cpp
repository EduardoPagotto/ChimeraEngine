#include "ParticleEmitter.h"
#include "ExceptionChimera.h"
#include "Object.h"
#include <algorithm>

namespace Chimera {

	ParticleEmitter::ParticleEmitter(std::string _id, std::string _name) : Node(EntityKind::PARTICLE_EMITTER, _id, _name) {

		coresPart.push_back(Color(1.0f, 0.5f, 0.5f));
		coresPart.push_back(Color(1.0f, 0.75f, 0.5f));
		coresPart.push_back(Color(1.0f, 1.0f, 0.5f));
		coresPart.push_back(Color(0.75f, 1.0f, 0.5f));
		coresPart.push_back(Color(0.5f, 1.0f, 0.5f));
		coresPart.push_back(Color(0.5f, 1.0f, 0.75f));
		coresPart.push_back(Color(0.5f, 1.0f, 1.0f));
		coresPart.push_back(Color(0.5f, 0.75f, 1.0f));
		coresPart.push_back(Color(0.75f, 0.5f, 1.0f));
		coresPart.push_back(Color(1.0f, 0.5f, 1.0f));
		coresPart.push_back(Color(1.0f, 0.5f, 0.75f));

		pTexture = nullptr;
	}

	ParticleEmitter::~ParticleEmitter() {

		if (pTexture != nullptr)
			delete pTexture;

	}

	void ParticleEmitter::initialize(int _max) {

		particles.reserve(_max);

		// Reset all the particles
		for (int loop = 0; loop < _max; loop++)
		{
			int indiceCor = loop % coresPart.size();                 // (loop + 1) / (_max / coresPart.size());

	        btVector3 novo((float)((rand() % 50) - 26.0f) * 10.0f,
							(float)((rand() % 50) - 25.0f) * 10.0f,
							(float)((rand() % 50) - 25.0f) * 10.0f  );           // yi e zi repetiam no original


			Particle *pParticle = new Particle();

	  		pParticle->ResetParticle( coresPart[indiceCor] , novo);

			particles.push_back(pParticle);
		}
	}

	void ParticleEmitter::update(DataMsg *_dataMsg) {

		if (_dataMsg->getKindOp() == KindOp::START) {

			Node::update(_dataMsg);
			initialize(100);

			pTexture->init();

		}
		else if (_dataMsg->getKindOp() == KindOp::DRAW3D) {

			glPushMatrix();

			glPushAttrib(GL_ENABLE_BIT);
			glPushAttrib(GL_CURRENT_BIT);

			setGL();

			//FIXME ajustar matrix
			//float matrix[16];
			//glGetFloatv(GL_PROJECTION_MATRIX, matrix);

			//Object *pSource = (Object *)_dataMsg->getParam();

			////pega posicao do objeto horigem de desenho (camera travada)
			////btVector3 l_vec = _pPhysic->pRigidBody->getWorldTransform().getOrigin();
			//btVector3 l_vec = pSource->getPosition();

			////desloca desenha para o pbjeto horigem
			//matrix[12] -= l_vec.getX();
			//matrix[13] -= l_vec.getY();
			//matrix[14] -= l_vec.getZ();

			//glMultMatrixf(matrix);

			render(); //em baixo???

			glPopAttrib();
			glPopAttrib();

			Node::update(_dataMsg);

			glPopMatrix();
		}
		else if (_dataMsg->getKindOp() == KindOp::IS_ALLOW_COLLIDE) {

		}
	}

	void ParticleEmitter::setGL() {

		// Enable smooth shading
		glShadeModel(GL_SMOOTH);

		// Set the background black
		//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		// Depth buffer setup
		//glClearDepth(1.0f);

		// Enables Depth Testing
		//glDisable(GL_DEPTH_TEST);

		// Enable Blending
		glEnable(GL_BLEND);

		// Type Of Blending To Perform
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Really Nice Perspective Calculations
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		// Really Nice Point Smoothing
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

		// Enable Texture Mapping
		glEnable(GL_TEXTURE_2D);

		glDisable(GL_LIGHTING);

	}

	void ParticleEmitter::loadImage(const char *_file) {

		pTexture = new Texture("testeZ1", "testeZ1");
		pTexture->setPathFile(std::string(_file));

	}

	void ParticleEmitter::render() {

		SortParticles();

		// Select Our Texture
		pTexture->render();

		for (unsigned loop = 0; loop < particles.size() ; loop++)
		{
			Particle *pParticle = particles[loop];
			pParticle->render();
		}

	}

	bool compare(Particle* a, Particle* b) {

	 float m[16];
	 glGetFloatv(GL_MODELVIEW_MATRIX, m);
	 btVector3 camera(m[12], m[13], m[14]);

	 float d1 = camera.distance(a->position);
	 float d2 = camera.distance(b->position);

	 return (d1 < d2);


	}


	//TODO rotina sera usada para ordenar as particulas em relacao a distancia com a camera
	void ParticleEmitter::SortParticles(){

// 	 std::sort(particles.begin(), particles.end(), [](Particle* a, Particle* b){
// 			return false;                                            //a->getId() < b->getId;
// 		}
// 	);

	 std::sort(particles.begin(), particles.end(), compare);


	}

	//TODO usar no mouse do rift
	//	void ParticleEmitter::update(DataMsg *_dataMsg) {
	//
	//		if (_dataMsg->getKindOp() == KindOp::START) {
	//
	//			Node::update(_dataMsg);
	//
	//			//inicializa objeto local
	//			initialize(10);
	//
	//		} else if (_dataMsg->getKindOp() == KindOp::DRAW3D) {
	//
	// 			glPushMatrix();
	////
	//// 			if (pPhysic != nullptr) {
	//// 			Object *pSource = (Object *)_dataMsg->getParam();
	//// 			pPhysic->ajusteMatrix(pSource->pPhysic);
	//// 			}
	////
	//			// salva flags de bit
	//			glPushAttrib(GL_ENABLE_BIT);
	//
	//			//preserva a cor original
	//			glPushAttrib(GL_CURRENT_BIT);
	//
	//
	//
	//// 	  float modelview[16];
	//// 	  int i,j;
	////
	//// 	  // save the current modelview matrix
	//// 	  glPushMatrix();
	////
	//// 	  // get the current modelview matrix
	//// 	  glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
	////
	//// 	  // undo all rotations
	//// 	  // beware all scaling is lost as well
	//// 	  for( i=0; i<3; i++ )
	//// 	  for( j=0; j<3; j++ ) {
	//// 	   if ( i==j )
	//// 	   modelview[i*4+j] = 1.0;
	//// 	   else
	//// 	   modelview[i*4+j] = 0.0;
	//// 	 }
	////
	//// 	  // set the modelview with no rotations
	//// 	  glLoadMatrixf(modelview);
	//
	//
	//
	//			setGL();
	//
	// 			render();
	//
	//
	//			//retorna paleta
	//			glPopAttrib();
	//			//retorna paleta
	//			glPopAttrib();
	//
	////
	//// 			Node::update(_dataMsg);
	////
	// 			glPopMatrix();
	//
	//// 	  glPopMatrix();
	//
	//		} else if (_dataMsg->getKindOp() == KindOp::IS_ALLOW_COLLIDE) {
	//
	//			//_dataMsg->setDone(true);
	//
	//		}
	//	}

}