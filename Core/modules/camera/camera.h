#pragma once
#include <glm/glm.hpp>
#include <SDL2/SDL_events.h>

class Camera {
    glm::vec3 velocity;
    glm::vec3 position;
    float pitch {0.0f};
    float yaw {0.0f};
    float roll {0.0f};

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    glm::mat4 getRotationMatrix();

    void processEvents(SDL_Event& event);
    void update();
};