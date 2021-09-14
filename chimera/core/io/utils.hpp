#ifndef __CHIMERA_UTILS__HPP
#define __CHIMERA_UTILS__HPP

#include <SDL2/SDL.h>
#include <string>

namespace Chimera::IO {

const int32_t EVENT_TOGGLE_FULL_SCREEN = 0x0001; // FullScreem
const int32_t EVENT_NEW_FPS = 0x2001;            // OFF_COLLIDE
const int32_t EVENT_COLLIDE_IS_ALLOW = 0x1000;   // IS_ALLOW_COLLIDE
const int32_t EVENT_COLLIDE_START = 0x1001;      // START_COLLIDE
const int32_t EVENT_COLLIDE_ON = 0x1002;         // ON_COLLIDE
const int32_t EVENT_COLLIDE_OFF = 0x1003;        // OFF_COLLIDE
/**
 * Enum KindOp
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
void utilSendEvent(const int32_t& user_event, void* _paramA, void* _paramB);

// GLuint shadeLoadProg(const char* programName, const char* vertex_file_path, const char* fragment_file_path);
std::string utilReadFile(const char* file_path);

} // namespace Chimera::IO

#endif
