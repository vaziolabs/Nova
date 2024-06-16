#include "./camera.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

void Camera::update()
    {
        glm::mat4 c_rotation = getRotationMatrix();
        position += glm::vec3(c_rotation * glm::vec4(velocity * 0.5f, 1.0f));
    }

void Camera::processEvents(SDL_Event& e)
    {
        if (e.type == SDL_KEYDOWN) 
            {
                switch (e.key.keysym.sym) 
                    {
                        case SDLK_w: velocity.z = -1.0f; break;
                        case SDLK_s: velocity.z = 1.0f; break;
                        case SDLK_a: velocity.x = -1.0f; break;
                        case SDLK_d: velocity.x = 1.0f; break;
                        case SDLK_q: roll -= 1.0f; break;
                        case SDLK_e: roll += 1.0f; break;
                    }
            }
        else if (e.type == SDL_KEYUP) 
            {
                switch (e.key.keysym.sym) 
                    {
                        case SDLK_w: velocity.z = 0.0f; break;
                        case SDLK_s: velocity.z = 0.0f; break;
                        case SDLK_a: velocity.x = 0.0f; break;
                        case SDLK_d: velocity.x = 0.0f; break;
                        case SDLK_q: velocity.y = 0.0f; break;
                        case SDLK_e: velocity.y = 0.0f; break;
                    }
            }

        if (e.type == SDL_MOUSEMOTION) 
            {
                yaw += (float)e.motion.xrel / 200.f;
                pitch -= (float)e.motion.yrel / 200.f;
            }
    }