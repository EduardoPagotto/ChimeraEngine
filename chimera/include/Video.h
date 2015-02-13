#ifndef VIDEO_H_
#define VIDEO_H_

#include <string>
#include "Camera.h"
#include "HUD.h"

namespace Chimera
{
	enum class KIND_DEVICE {
		SCREEN = 0,
		OVR_OCULUS
	};

	/**
	 * Class Video
	 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
	 *  @since 20130925
	 */
	class Video
	{
	public:

		Video(std::string _nome);
		virtual ~Video();

		virtual void initGL() = 0;
		virtual void initDraw() = 0;
		virtual void endDraw() = 0;
		virtual void getGeometry(int &_x, int &_y, int &_w, int &_h, int index) = 0;

		virtual void executeViewPerspective(Camera *pCamera, int _eye) = 0;
		virtual void executeViewOrto(int eyeIndex) = 0;

		std::string getNomeTela() const {
			return nomeTela;
		}

		void updateHud(HUD *pHud, int eye);

		void restoreMatrix();

		void setLight(bool _lightOn);
		void setMaterial(bool _materialOn);

		std::string getVersaoOpenGL();

		inline KIND_DEVICE getKindDevice() const {
			return kindDevice;
		}

	protected:
		std::string nomeTela;
		KIND_DEVICE kindDevice;

	};

}				/* namespace Chimera */
#endif				/* VIDEO_H_ */
// kate: indent-mode cstyle; indent-width 2; replace-tabs on; ;
