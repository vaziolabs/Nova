#include "NovaExample.h"
#include "../Core/components/debug_level.h"


NovaExample* _essence = nullptr;

NovaExample::NovaExample() 
    {
        report(LOGGER::INFO, "NovaExample - Constructing Nova ..");
        assert(_essence == nullptr);
        
        _config = { 
                "Nova Example", 
                { 1600, 1200 }, 
                "development"
            };
        _actuality = nullptr;
    }

NovaExample::~NovaExample()
    {
        report(LOGGER::INFO, "NovaExample - The End is Nigh ..");

        if (_essence != nullptr) {
            delete _actuality;
            _essence = nullptr;
        }
    }

// Singleton to ensure only one instance of NovaExample is created.
NovaExample* NovaExample::manifest()
    {
        report(LOGGER::INFO, "NovaExample - Manifesting ..");

        // This is not required, but it is a good practice to ensure only one instance is created.. 
        // This is particularly useful when we get to the point of using cloud level instantiation and want to pass
        // the same instance to multiple clients (not users but services).
        // Really this level of wrapping is redundant, but it's nice for static analysis and code review.
        if (_essence == nullptr) {
            NovaExample* essence = new NovaExample();
            _essence = essence->realize();
        }

        return _essence;
    }

// TODO: this could be handled by a singleton too, but at that level we would need to refactor the above
// and abstract or embed this 'singleton' into the Nova class. .. We will do this once we are properly using the 'materialize' function.
NovaExample* NovaExample::realize() 
    {
        // Initialize the Graphics with Genesis 
        report(LOGGER::INFO, "NovaExample - Nova Complete ..");

        _actuality = new Nova(_config);
        _actuality->initialized = true;

        return this;
    }

// This is something we want to be able to define on the parent level for the game engine, 
// but also to be able to 'embed' and pass through into the main game loop.
// This essentially runs inside of our render loop.
void NovaExample::materialize() 
    {
       report(LOGGER::VERBOSE, "NovaExample - Materializing ..");
       // This is where we need to handle the real time calculations and state changes
       // TODO: Implement Menu and User Input Handling and Processing

        return;
    }

void NovaExample::actualize() 
    {
        report(LOGGER::INFO, "NovaExample - Actualizing ..");

        // Do we want to pull top level SDL logic out to here?
        _actuality->illuminate();
        //_actuality->illuminate(NovaExample::materialize);   
    }
