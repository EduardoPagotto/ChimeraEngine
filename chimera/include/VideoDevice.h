#ifndef VIDEO_DEVICE_H_
#define VIDEO_DEVICE_H_

#include "Video.h"

namespace Chimera
{
	class VideoDevice : public Video
	{
	public:
		VideoDevice(std::string _nome);
		VideoDevice(int _width, int _height, std::string _nome);
		virtual ~VideoDevice();

		virtual void initDraw();
		virtual void endDraw();
		virtual void executeViewPerspective(Camera *pCamera, int _eye);
		virtual void executeViewOrto(int eye);
		virtual void reshape(int x, int y);
		virtual void toggleFullScreen();

	private:
		bool fullscreenStatus;
		SDL_Rect winGeometryPrev;
	};

}

#endif