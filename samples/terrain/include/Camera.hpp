#ifndef __GAME_TERRAIN_CAMERA__HPP
#define __GAME_TERRAIN_CAMERA__HPP

#include "Frustum.hpp"

namespace ChimeraNew {

class Camera {

  public:
    Camera();
    ~Camera();
    void Look(const glm::vec3& _position, const glm::vec3& _reference, bool RotateAroundReference = false);
    void Move(const glm::vec3& Movement);
    glm::vec3 OnKeys(unsigned short Keys, float FrameTime);
    void OnMouseMove(int dx, int dy);
    void OnMouseWheel(float zDelta);
    void SetPerspective(float fovy, float aspect, float n, float f);

    glm::vec3 X, Y, Z, position, reference;

    glm::mat4 ViewMatrix;
    glm::mat4 ViewMatrixInverse;
    glm::mat4 ProjectionMatrix;
    glm::mat4 ProjectionMatrixInverse;
    glm::mat4 ViewProjectionMatrix;
    glm::mat4 ViewProjectionMatrixInverse;

    Chimera::Frustum frustum;

  private:
    void CalculateViewMatrix();
};

} // namespace ChimeraNew

#endif