#ifndef VIDEO_DEVICE_H_
#define VIDEO_DEVICE_H_

#include "Video.h"

#ifndef WIN32
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

namespace Chimera
{

	class VideoDevice : public Video
	{
	public:
		VideoDevice(std::string _nome);
		VideoDevice(int _width, int _height, std::string _nome);

		virtual ~VideoDevice();

		void initGL();

		void initDraw();
		void endDraw();

		void getGeometry(int &_x, int &_y, int &_w, int &_h, int index);

		virtual void executeViewPerspective(Camera *pCamera, int _eye);
		virtual void executeViewOrto(int eye);
		virtual void reshape(int x, int y);
		virtual void toggleFullScreen();

	private:

		void initHardware();

		SDL_Rect rectangle;
		SDL_Window *window;
		SDL_GLContext context;

	};

}



#endif