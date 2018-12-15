// #ifndef __CHIMERA_CAMERA_MAN__HPP
// #define __CHIMERA_CAMERA_MAN__HPP

// #include "Camera.hpp"

// namespace Chimera {

// class CameraMan : public Camera {

//   public:
//     CameraMan(std::string _name);
//     CameraMan(const Camera& _camera);
//     virtual ~CameraMan();

//     virtual void init();
//     virtual void render();

//     void pitch(float _theta);
//     void yaw(float _theta);
//     void roll(float _theta);
//     void walk(float _delta, bool _wall[4]);
//     void strafe(float _delta, bool _wall[4]);
//     void fly(float _delta, bool _wall[4]);
//     void reset();

//     void updateMove();

//   private:
//     glm::vec3 along;
//     glm::vec3 up;
//     glm::vec3 forward;
// };
// } // namespace Chimera
// #endif
