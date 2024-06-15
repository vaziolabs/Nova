#pragma once
#include "../Nova.h"
#include <string>

class NovaExample {
    public:
        NovaExample();
        ~NovaExample();

        static NovaExample* manifest();   // Singleton

        static void materialize();      // Draw
        void actualize();               // Run
    
    private:
        std::string _application_name;
        VkExtent2D _window_extent;
        Nova* _actuality;

        NovaExample* realize();            // Init
};