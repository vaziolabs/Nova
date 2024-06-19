#include "core.h"
#include <string>

struct NovaConfig {
    std::string name;
    VkExtent2D window_size;
    std::string debug_level;        
    std::string dimensions;         // 2D, 3D (Not Implemented)
    std::string camera_type;        // Fixed, FPS, Quaternion (Not Implemented)
    bool compute;                   // True, False (Not Implemented)
};