#include "Texture.h"

#ifndef WIN32
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#include "windows.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>

namespace Chimera {

	Texture::Texture(std::string _id, std::string _name) : Node(EntityKind::TEXTURE, _id, _name) {

		texturaCarregada = false;
		pathFile = "";
		setFilter(TextureFilter::Nearest);
		textureList[0] = 0;
		textureList[1] = 0;
		textureList[2] = 0;

	}

	Texture::Texture(const Texture &_texture) : Node(_texture) {

		pathFile = _texture.pathFile;
		indiceFilter = _texture.indiceFilter;
		texturaCarregada = _texture.texturaCarregada;
		textureList[0] = _texture.textureList[0];
		textureList[1] = _texture.textureList[1];
		textureList[2] = _texture.textureList[2];

	}

	Texture::~Texture() {
		//TODO descobrir como remover textura do openGL quando textura deixar de existir
	}

	void Texture::setFilter(TextureFilter _filter) {

		switch (_filter) {
		case TextureFilter::Nearest:
			indiceFilter = 0;
			break;
		case TextureFilter::Linear:
			indiceFilter = 1;
			break;
		case TextureFilter::Mipmapped:
			indiceFilter = 2;
			break;

		}

	}

	TextureFilter Texture::getFilter() {

		TextureFilter retorno;

		switch (indiceFilter) {
		case 0:
			retorno = TextureFilter::Nearest;
			break;
		case 1:
			retorno = TextureFilter::Linear;
			break;
		case 2:
			retorno = TextureFilter::Mipmapped;
			break;

		}

		return retorno;

	}

	void Texture::render() {

		glBindTexture(GL_TEXTURE_2D, textureList[indiceFilter]);

	}

	void Texture::init() {

		if (texturaCarregada == false) {

			SDL_Surface *pImage = loadImage();

			/* Create The Texture */
			glGenTextures(3, (GLuint*)&textureList[0]);

			/* Load in texture 1 */
			glBindTexture(GL_TEXTURE_2D, textureList[0]); /* Typical Texture Generation Using Data From The Bitmap */

			/* Generate The Texture */
			glTexImage2D(GL_TEXTURE_2D, 0, 3, pImage->w,
				pImage->h, 0, GL_BGR_EXT,
				GL_UNSIGNED_BYTE, pImage->pixels);

			/* Nearest Filtering */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				GL_NEAREST);


			/* Load in texture 2 */
			glBindTexture(GL_TEXTURE_2D, textureList[1]); /* Typical Texture Generation Using Data From The Bitmap */

			/* Linear Filtering */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				GL_LINEAR);

			/* Generate The Texture */
			glTexImage2D(GL_TEXTURE_2D, 0, 3, pImage->w,
				pImage->h, 0, GL_BGR_EXT,
				GL_UNSIGNED_BYTE, pImage->pixels);

			/* Load in texture 3 */
			glBindTexture(GL_TEXTURE_2D, textureList[2]); /* Typical Texture Generation Using Data From The Bitmap */

			/* Mipmapped Filtering */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				GL_LINEAR);

			/* Generate The MipMapped Texture ( NEW ) */
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pImage->w,
				pImage->h, GL_BGR_EXT,
				GL_UNSIGNED_BYTE, pImage->pixels);

			SDL_FreeSurface(pImage);

			texturaCarregada = true;
		}

	}

	SDL_Surface *Texture::loadImage() {

		SDL_Surface *pImage = IMG_Load(pathFile.c_str());
		if (pImage != nullptr)
			return pImage;

		throw ExceptionChimera(ExceptionCode::READ, "Falha ao ler arquivo:" + pathFile);
	}

	void Texture::clone(Node **ppNode) {
		*ppNode = new Texture(*this);
		Node::clone(ppNode);
	}

	void Texture::update(DataMsg *dataMsg) {

		if (dataMsg->getKindOp() == KindOp::START)
			init();

		Node::update(dataMsg);
	}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
