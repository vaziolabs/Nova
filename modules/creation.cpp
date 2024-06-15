#include "creation.h"

Existence* _essence = nullptr;

Existence::Existence() 
    {
        report(LOGGER::INFO, "Existence - Constructing Graphics ..");
        assert(_essence == nullptr);
        
        _application_name = "Compute Shaders";
        _window_extent = { 1600, 1200 };
        _actuality = nullptr;
    }

Existence::~Existence()
    {
        report(LOGGER::INFO, "Existence - The End is Nigh ..");

        if (_essence != nullptr) {
            delete _actuality;
            _essence = nullptr;
        }
    }

// Singleton to ensure only one instance of Existence is created.
Existence* Existence::manifest()
    {
        report(LOGGER::INFO, "Existence - Manifesting ..");

        if (_essence == nullptr) {
            Existence* essence = new Existence();
            _essence = essence->realize();
        }

        return _essence;
    }

// TODO: this needs to be handled by a singleton
Existence* Existence::realize() 
    {
        // Initialize the Graphics with Genesis 
        report(LOGGER::INFO, "Existence - Graphics Complete ..");

        _actuality = new Graphics(_application_name, _window_extent);
        _actuality->initialized = true;

        return this;
    }

void Existence::materialize() 
    {
       report(LOGGER::VERBOSE, "Existence - Materializing ..");
       // This is where we need to handle the real time calculations and state changes
       // TODO: Implement Menu and User Input Handling and Processing

        return;
    }

void Existence::actualize() 
    {
        report(LOGGER::INFO, "Existence - Actualizing ..");

        // Do we want to pull top level SDL logic out to here?
        _actuality->illuminate();
        //_actuality->illuminate(Existence::materialize);   
    }