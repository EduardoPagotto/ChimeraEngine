#ifndef IMAGE_H_
#define IMAGE_H_

#include "SDL2/SDL_image.h"

namespace Chimera {

class Image {
public:
	friend class Material;
	Image();
	~Image();
	int load(const char *_file);
private:
	SDL_Surface *pImage;
};

}
#endif