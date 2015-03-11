#ifndef TEXTURE_H_
#define TEXTURE_H_

#ifdef WIN32
#include "SDL_image.h"
#else
#include "SDL2/SDL_image.h"
#endif
#include "ExceptionChimera.h"

#include "Node.h"

namespace Chimera {

	enum class TextureFilter {
		Nearest,
		Linear,
		Mipmapped
	};


	class Texture : public Node {
	public:

		Texture(std::string _id, std::string _name);
		Texture(const Texture &_texture);

		virtual ~Texture();

		virtual void update(DataMsg *dataMsg);

		virtual void clone(Node **ppNode);

		void render();
		void init();

		SDL_Surface *loadImage();

		void setPathFile(const std::string &_pathFile) {
			pathFile = _pathFile;
		}

		void setFilter(TextureFilter _filter);

		TextureFilter getFilter();

	private:
		std::string pathFile;

		int textureList[3];
		//int texture;

		int indiceFilter;
		bool texturaCarregada;
	};

}
#endif
