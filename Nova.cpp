#include "Nova.h"

Nova* _essence = nullptr;

Nova::Nova() 
    {
        report(LOGGER::INFO, "Nova - Constructing Graphics ..");
        assert(_essence == nullptr);
        
        _application_name = "Compute Shaders";
        _window_extent = { 1600, 1200 };
        _actuality = nullptr;
    }

Nova::~Nova()
    {
        report(LOGGER::INFO, "Nova - The End is Nigh ..");

        if (_essence != nullptr) {
            delete _actuality;
            _essence = nullptr;
        }
    }

// Singleton to ensure only one instance of Nova is created.
Nova* Nova::manifest()
    {
        report(LOGGER::INFO, "Nova - Manifesting ..");

        if (_essence == nullptr) {
            Nova* essence = new Nova();
            _essence = essence->realize();
        }

        return _essence;
    }

// TODO: this needs to be handled by a singleton
Nova* Nova::realize() 
    {
        // Initialize the Graphics with Genesis 
        report(LOGGER::INFO, "Nova - Graphics Complete ..");

        _actuality = new Graphics(_application_name, _window_extent);
        _actuality->initialized = true;

        return this;
    }

void Nova::materialize() 
    {
       report(LOGGER::VERBOSE, "Nova - Materializing ..");
       // This is where we need to handle the real time calculations and state changes
       // TODO: Implement Menu and User Input Handling and Processing

        return;
    }

void Nova::actualize() 
    {
        report(LOGGER::INFO, "Nova - Actualizing ..");

        // Do we want to pull top level SDL logic out to here?
        _actuality->illuminate();
        //_actuality->illuminate(Nova::materialize);   
    }
