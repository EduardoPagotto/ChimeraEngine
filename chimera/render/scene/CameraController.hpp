#pragma once
#include "chimera/core/ScriptableEntity.hpp"
#include "chimera/core/visible/ICamera.hpp"

namespace Chimera {

class CameraControllerFPS : public ScriptableEntity {
  public:
    CameraControllerFPS() = default;
    void onCreate() override;
    void onDestroy() override;
    void onUpdate(const double& ts) override;

    const glm::vec3 getPosition() const { return camera->getPosition(); }
    const glm::vec3 getFront() const { return front; }
    const glm::vec3 getUp() const { return up; }

  private:
    void updateEye();
    void updateVectors();
    void update(const double& ts);
    void processCameraRotation(double xOffset, double yOffset, bool constrainPitch);
    void processCameraMovement(glm::vec3& direction, float deltaTime);
    void invertPitch();

    float pitch, yaw, movementSpeed;

    glm::vec3 up, front, worldUp, right;

    ICamera* camera = nullptr;
    EyeView* eyeView = nullptr;
};

class CameraControllerOrbit : public ScriptableEntity {
  public:
    CameraControllerOrbit() = default;
    void onCreate() override;
    void onDestroy() override;
    void onUpdate(const double& ts) override;

    // const glm::vec3 getPosition() const { return camera->getPosition(); }
    // const glm::vec3 getFront() const { return front; }
    // const glm::vec3 getUp() const { return up; }

  private:
    void updateEye();
    void updateVectors();
    void update(const double& ts);

    void processCameraRotation(const int& xOffset, const int& yOffset, bool constrainPitch = true);
    void processDistance(const int& _mz);

    // void processCameraRotation(double xOffset, double yOffset, bool constrainPitch);
    // void processCameraMovement(glm::vec3& direction, float deltaTime);
    void invertPitch();

    float pitch, yaw, distance, min, max; //, movementSpeed;

    glm::vec3 up, front; //, worldUp, right;

    ICamera* camera = nullptr;
    EyeView* eyeView = nullptr;
};

} // namespace Chimera
