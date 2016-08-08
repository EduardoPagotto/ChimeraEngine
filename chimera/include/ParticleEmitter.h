#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include<vector>

#include "Particle.h"
# include "Draw.h"
# include "Texture.h"

namespace Chimera {
    
class ParticleEmitter : public Draw
{
public:
    ParticleEmitter (Node *_parent, std::string _name );
    virtual ~ParticleEmitter();
	virtual void init();
	virtual void accept(class NodeVisitor* v);
    virtual void renderExecute(bool _texture);
    virtual glm::vec3 getSizeBox();
    
    void setGL();
    void loadImage ( const char *_file );
    void SortParticles ( const glm::vec3 &posCamera );
    void setSizeBox(const glm::vec3 &_size);

	inline void setPosSource(const glm::vec3 &_val) {
		posSource = _val;
	}

	inline glm::vec3 getPosSource() const {
		return posSource;
	}

	inline void setMaxSeed(const int &_max) {
		maxSeed = _max;
	}

	inline int getMaxSeed() const {
		return maxSeed;
	}

	inline void setPosition(const glm::vec3 &_pos) {
		position = _pos;
	}

private:
	glm::vec3 position;
	glm::vec3 posSource;
    glm::vec3 sizeBox;
    
	int maxSeed;
    
    std::vector<Particle*> particles;
    std::vector<Color> coresPart;
};
}
#endif
