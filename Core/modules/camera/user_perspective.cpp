#include "./user_perspective.h"

void UserPerspective::switchCamera() 
    {
        if (active_camera == &first_person) {
            active_camera = &third_person_behind;
        } else if (active_camera == &third_person_behind) {
            active_camera = &third_person_above;
        } else {
            active_camera = &first_person;
        }
    }

void UserPerspective::update() { active_camera->update(); }
void UserPerspective::processEvents(SDL_Event& e) 
    { 
        active_camera->processEvents(e); 
        if (e.type == SDL_KEYDOWN) 
            {
                if (e.key.keysym.sym == SDLK_1) 
                    { switchCamera(); }
                if (e.key.keysym.sym == SDLK_2) 
                    { active_camera->setRearView(); }
            }
            
        if (e.type == SDL_KEYUP) 
            {
                if (e.key.keysym.sym == SDLK_2) 
                    { active_camera->setRearView(); }
            }
    }