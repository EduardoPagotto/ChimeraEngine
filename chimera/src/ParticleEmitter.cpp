#include "ParticleEmitter.h"
#include "ExceptionChimera.h"
#include "Coord.h"
#include "Camera.h"
#include "NodeVisitor.h"
#include <algorithm>

namespace Chimera {
    
ParticleEmitter::ParticleEmitter (Node* _parent, std::string _name ) : Draw (_parent, EntityKind::PARTICLE_SYSTEM, _name ) {

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

void ParticleEmitter::init() {
	
	particles.reserve(maxSeed);

	// Reset all the particles
	for (int loop = 0; loop < maxSeed; loop++) {

		int indiceCor = loop % coresPart.size();                 // (loop + 1) / (_max / coresPart.size());

		glm::vec3 direcao(  (float)((rand() % 50) - 26.0f) * 10.0f,
                            (float)((rand() % 50) - 25.0f) * 10.0f,
                            (float)((rand() % 50) - 25.0f) * 10.0f);   // yi e zi repetiam no original

		Particle *pParticle = new Particle(&posSource, direcao, glm::vec3(0.0, 0.0, 0.0), coresPart[indiceCor], 10.0f);
		particles.push_back(pParticle);
	}

	pTexture->init();

}

void ParticleEmitter::setSizeBox ( const glm::vec3& _size ) {
    sizeBox = _size;
}

glm::vec3 ParticleEmitter::getSizeBox() {
    return sizeBox;
}

void ParticleEmitter::accept(NodeVisitor * v) {
	v->visit(this);
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

void ParticleEmitter::renderExecute(bool _texture) {

    glPushAttrib ( GL_ENABLE_BIT );
    glPushAttrib ( GL_CURRENT_BIT );

    setGL();

    // desloca objeto em relacao ao que contem a camera
    glTranslatef ( -position.x , -position.y , -position.z );

    // armazena a camera para uso no ordenador //TODO: Otimizar criando lista de cameras no Object e indicando qual esta ativa neste momento
    Camera *pCam = (Camera*)Node::findNodeBySeq(EntityKind::CAMERA, 0);//FIXME!!!!!! //pSource->findChildByKind ( EntityKind::CAMERA, 0 );
    if ( pCam !=  nullptr ) {
        SortParticles ( pCam->getPosition() );
    }
    
    // Select Our Texture
    pTexture->apply();

    for ( unsigned loop = 0; loop < particles.size(); loop++ ) {
        Particle *pParticle = particles[loop];
        pParticle->render();
    }
    
    glPopAttrib();
    glPopAttrib();
       
}

// estrutura de comparacao
// TODO Otimizar
struct ParticleCompare {
    const glm::vec3 *pPosicao;
    bool operator() ( Particle* i,Particle* j ) {

        bool retorno;
                
        float d1 = glm::distance(*pPosicao , i->position); //pPosicao->distance ( i->position );
        float d2 = glm::distance(*pPosicao , j->position); //pPosicao->distance ( j->position );
        retorno = ( d2 < d1 );

        return retorno;
    }
};


void ParticleEmitter::SortParticles ( const glm::vec3 &posCamera ) {

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
}