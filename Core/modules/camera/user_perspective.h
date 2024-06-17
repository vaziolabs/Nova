#include "./camera.h"
#include <glm/glm.hpp>

class UserPerspective {
    public:
        Camera *active_camera;

        void switchCamera();
        void update();
        void processEvents(SDL_Event& event);
    
    private:
        Camera first_person;
        Camera third_person_behind;
        Camera third_person_above;
};