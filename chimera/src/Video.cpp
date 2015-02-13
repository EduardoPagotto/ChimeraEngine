#include "Video.h"

#include "ExceptionChimera.h"

#ifdef WIN32
#include "windows.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>

namespace Chimera {

	Video::Video(std::string _nome) : nomeTela(_nome) {

	}

	Video::~Video() {

	}

	void  Video::updateHud(HUD *pHud, int eye) {

		if (pHud->isOn() == true) {

			executeViewOrto(eye);

			pHud->update();

			restoreMatrix();
		}
	}

	void Video::restoreMatrix() {

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

	}

	std::string Video::getVersaoOpenGL() {

		std::string retorno = "";
		const char *version = (const char *)glGetString(GL_VERSION);

		if (version != nullptr)
			retorno.append(version);
		else {
			//Check for error
			GLenum error = glGetError();
			throw ExceptionChimera(ExceptionCode::READ, std::string((const char *)gluErrorString(error)));
		}

		return retorno;
	}

	void Video::setLight(bool _lightOn) {
		if (_lightOn == true)
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);
	}

	void Video::setMaterial(bool _materialOn) {

		if (_materialOn == true) {
			glDisable(GL_COLOR_MATERIAL);
			//glColorMaterial ( GL_FRONT, GL_DIFFUSE ); //TODO verificar necessidade
		}
		else {
			glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT, GL_DIFFUSE);  //TODO verificar necessidade
		}
	}

} /* namespace Chimera */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4;


