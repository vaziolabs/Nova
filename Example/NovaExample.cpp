#include "NovaExample.h"

NovaExample* _essence = nullptr;

NovaExample::NovaExample() 
    {
        report(LOGGER::INFO, "NovaExample - Constructing Nova ..");
        assert(_essence == nullptr);
        
        _application_name = "Compute Shaders";
        _window_extent = { 1600, 1200 };
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

        if (_essence == nullptr) {
            NovaExample* essence = new NovaExample();
            _essence = essence->realize();
        }

        return _essence;
    }

// TODO: this needs to be handled by a singleton
NovaExample* NovaExample::realize() 
    {
        // Initialize the Graphics with Genesis 
        report(LOGGER::INFO, "NovaExample - Nova Complete ..");

        _actuality = new Nova(_application_name, _window_extent);
        _actuality->initialized = true;

        return this;
    }

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
