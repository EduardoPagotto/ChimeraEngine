#include "ParticleEmitter.h"
#include "ExceptionChimera.h"
#include "Object.h"
#include "Camera.h"
#include <algorithm>

namespace Chimera {
namespace Graph {
    
ParticleEmitter::ParticleEmitter ( std::string _name ) : Node ( EntityKind::PARTICLE_EMITTER, _name ) {

    coresPart.push_back ( Color ( 1.0f, 0.5f, 0.5f ) );
    coresPart.push_back ( Color ( 1.0f, 0.75f, 0.5f ) );
    coresPart.push_back ( Color ( 1.0f, 1.0f, 0.5f ) );
    coresPart.push_back ( Color ( 0.75f, 1.0f, 0.5f ) );
    coresPart.push_back ( Color ( 0.5f, 1.0f, 0.5f ) );
    coresPart.push_back ( Color ( 0.5f, 1.0f, 0.75f ) );
    coresPart.push_back ( Color ( 0.5f, 1.0f, 1.0f ) );
    coresPart.push_back ( Color ( 0.5f, 0.75f, 1.0f ) );
    coresPart.push_back ( Color ( 0.75f, 0.5f, 1.0f ) );
    coresPart.push_back ( Color ( 1.0f, 0.5f, 1.0f ) );
    coresPart.push_back ( Color ( 1.0f, 0.5f, 0.75f ) );

    pTexture = nullptr;
}

ParticleEmitter::~ParticleEmitter() {

    if ( pTexture != nullptr ) {
        delete pTexture;
    }

}

void ParticleEmitter::initialize ( int _max ) {

    particles.reserve ( _max );

    // Reset all the particles
    for ( int loop = 0; loop < _max; loop++ ) {
        int indiceCor = loop % coresPart.size();                 // (loop + 1) / (_max / coresPart.size());

        btVector3 direcao ( ( float ) ( ( rand() % 50 ) - 26.0f ) * 10.0f,
                            ( float ) ( ( rand() % 50 ) - 25.0f ) * 10.0f,
                            ( float ) ( ( rand() % 50 ) - 25.0f ) * 10.0f );   // yi e zi repetiam no original


        Particle *pParticle = new Particle ( btVector3 ( 5.0, 5.0, 5.0 ), direcao, btVector3 ( 0.0, 0.0, 0.0 ), coresPart[indiceCor], 10.0f );
        particles.push_back ( pParticle );
    }
}

void ParticleEmitter::update ( DataMsg *_dataMsg ) {

    if ( _dataMsg->getKindOp() == KindOp::START ) {

        Node::update ( _dataMsg );
        initialize ( 150 );

        pTexture->init();

    } else if ( _dataMsg->getKindOp() == KindOp::DRAW ) {

        glPushMatrix();

        glPushAttrib ( GL_ENABLE_BIT );
        glPushAttrib ( GL_CURRENT_BIT );

        setGL();

        Object *pSource = ( Object * ) _dataMsg->getParam();

        // desloca objeto em relacao ao que contem a camera
        btVector3 l_vec = pSource->getPosition();
        glTranslatef ( -l_vec.x(), -l_vec.y(), -l_vec.z() );

        // armazena a camera para uso no ordenador //TODO: Otimizar criando lista de cameras no Object e indicando qual esta ativa neste momento
        Graph::Camera *pCam = ( Graph::Camera* ) pSource->findChildByKind ( EntityKind::CAMERA, 0 );
        if ( pCam !=  nullptr ) {
            SortParticles ( pCam->getPosition() );
        }

        render();

        glPopAttrib();
        glPopAttrib();

        Node::update ( _dataMsg );

        glPopMatrix();
    } else if ( _dataMsg->getKindOp() == KindOp::IS_ALLOW_COLLIDE ) {

    }
}

void ParticleEmitter::setGL() {

    // Enable smooth shading
    glShadeModel ( GL_SMOOTH );

    // Set the background black
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Depth buffer setup
    //glClearDepth(1.0f);

    // Enables Depth Testing
    //glDisable(GL_DEPTH_TEST);

    // Enable Blending
    glEnable ( GL_BLEND );

    // Type Of Blending To Perform
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE );
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Really Nice Perspective Calculations
    glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    // Really Nice Point Smoothing
    glHint ( GL_POINT_SMOOTH_HINT, GL_NICEST );

    // Enable Texture Mapping
    glEnable ( GL_TEXTURE_2D );

    glDisable ( GL_LIGHTING );

}

void ParticleEmitter::loadImage ( const char *_file ) {

    pTexture = new Texture ( "testeZ1", std::string ( _file ) );

}

void ParticleEmitter::render() {

    // Select Our Texture
    pTexture->begin();

    for ( unsigned loop = 0; loop < particles.size(); loop++ ) {
        Particle *pParticle = particles[loop];
        pParticle->render();
    }

    pTexture->end();

}

// estrutura de comparacao
// TODO Otimizar
struct ParticleCompare {
    const btVector3 *pPosicao;
    bool operator() ( Particle* i,Particle* j ) {

        bool retorno;
        float d1 = pPosicao->distance ( i->position );
        float d2 = pPosicao->distance ( j->position );
        retorno = ( d2 < d1 );

        return retorno;

    }
};


void ParticleEmitter::SortParticles ( const btVector3 &posCamera ) {

//// Lambda nao carrega o this
// 		std::sort(particles.begin(), particles.end(), [](Particle* a, Particle* b){
//
// 			bool ret;
// 			float d1 = posCamera.distance(a->position);
// 			float d2 = posCamera.distance(b->position);
//
// 			ret = d1 < d2;
//
// 			return ret;
// 		});


    static ParticleCompare comparador;
    comparador.pPosicao = &posCamera;
    std::sort ( particles.begin(), particles.end(), comparador );

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
}