#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ChimeraNew {

// FIXME: ver equivalencia em GLM::
void GetXY(const glm::vec3& Z, glm::vec3& X, glm::vec3& Y) {
    if (Z.y == -1.0f) {
        X = glm::vec3(1.0f, 0.0f, 0.0f);
        Y = glm::vec3(0.0f, 0.0f, 1.0f);
    } else if (Z.y > -1.0f && Z.y < 0.0f) {
        X = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), Z);
        Y = glm::cross(Z, X);
    } else if (Z.y == 0.0f) {
        Y = glm::vec3(0.0f, 1.0f, 0.0f);
        X = glm::cross(Y, Z);
    } else if (Z.y > 0.0f && Z.y < 1.0f) {
        X = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), Z);
        Y = glm::cross(Z, X);
    } else if (Z.y == 1.0f) {
        X = glm::vec3(1.0f, 0.0f, 0.0f);
        Y = glm::vec3(0.0f, 0.0f, -1.0f);
    }
}

Camera::Camera() {
    X = glm::vec3(1.0f, 0.0f, 0.0f);
    Y = glm::vec3(0.0f, 1.0f, 0.0f);
    Z = glm::vec3(0.0f, 0.0f, 1.0f);

    position = glm::vec3(0.0f, 0.0f, 5.0f);
    reference = glm::vec3(0.0f, 0.0f, 0.0f);

    CalculateViewMatrix();
}

Camera::~Camera() {}

void Camera::Look(const glm::vec3& _position, const glm::vec3& _reference, bool RotateAroundReference) {
    this->position = _position;
    this->reference = _reference;

    Z = normalize(_position - _reference);

    GetXY(Z, X, Y);

    if (!RotateAroundReference) {
        this->reference = this->position - Z * 0.05f;
    }

    CalculateViewMatrix();
}

void Camera::Move(const glm::vec3& Movement) {
    position += Movement;
    reference += Movement;

    CalculateViewMatrix();
}

glm::vec3 Camera::OnKeys(unsigned short Keys, float FrameTime) {
    float Speed = 5.0f;

    if (Keys & 0x40)
        Speed *= 2.0f;
    if (Keys & 0x80)
        Speed *= 0.5f;

    float Distance = Speed * FrameTime;

    glm::vec3 Up(0.0f, 1.0f, 0.0f);
    glm::vec3 Right = X;
    glm::vec3 Forward = cross(Up, Right);

    Up *= Distance;
    Right *= Distance;
    Forward *= Distance;

    glm::vec3 Movement;

    if (Keys & 0x01)
        Movement += Forward;
    if (Keys & 0x02)
        Movement -= Forward;
    if (Keys & 0x04)
        Movement -= Right;
    if (Keys & 0x08)
        Movement += Right;
    if (Keys & 0x10)
        Movement += Up;
    if (Keys & 0x20)
        Movement -= Up;

    return Movement;
}

void Camera::OnMouseMove(int dx, int dy) {
    float Sensitivity = 0.25f;

    position -= reference;

    if (dx != 0) {
        float DeltaX = (float)dx * Sensitivity;

        // X = rotate(X, DeltaX, glm::vec3(0.0f, 1.0f, 0.0f));
        // Y = rotate(Y, DeltaX, glm::vec3(0.0f, 1.0f, 0.0f));
        // Z = rotate(Z, DeltaX, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    if (dy != 0) {
        float DeltaY = (float)dy * Sensitivity;

        // Y = rotate(Y, DeltaY, X);
        // Z = rotate(Z, DeltaY, X);

        if (Y.y < 0.0f) {
            Z = glm::vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
            Y = cross(Z, X);
        }
    }

    position = reference + Z * length(position);

    CalculateViewMatrix();
}

void Camera::OnMouseWheel(float zDelta) {
    position -= reference;

    if (zDelta < 0 && length(position) < 500.0f) {
        position += position * 0.1f;
    }

    if (zDelta > 0 && length(position) > 0.05f) {
        position -= position * 0.1f;
    }

    position += reference;

    CalculateViewMatrix();
}

void Camera::SetPerspective(float fovy, float aspect, float n, float f) {
    ProjectionMatrix = glm::perspective(fovy, aspect, n, f);
    ProjectionMatrixInverse = glm::inverse(ProjectionMatrix);
    ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
    ViewProjectionMatrixInverse = ViewMatrixInverse * ProjectionMatrixInverse;

    frustum.set(ViewProjectionMatrixInverse);
}

void Camera::CalculateViewMatrix() {
    ViewMatrix = glm::mat4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, position),
                           -dot(Y, position), -dot(Z, position), 1.0f);
    ViewMatrixInverse = glm::inverse(ViewMatrix);
    ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
    ViewProjectionMatrixInverse = ViewMatrixInverse * ProjectionMatrixInverse;

    frustum.set(ViewProjectionMatrixInverse);
}

} // namespace ChimeraNew