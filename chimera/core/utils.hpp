#ifndef __CHIMERA_UTILS__HPP
#define __CHIMERA_UTILS__HPP

#include "chimera/core/OpenGLDefs.hpp"

namespace Chimera {

/**
 * Enum KindOp
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
enum class KindOp { IS_ALLOW_COLLIDE, START_COLLIDE, ON_COLLIDE, OFF_COLLIDE, VIDEO_TOGGLE_FULL_SCREEN };

void eventsSend(KindOp _kindOf, void* _paramA, void* _paramB);

GLuint shadeLoadProg(const char* programName, const char* vertex_file_path, const char* fragment_file_path);

} // namespace Chimera

#endif
