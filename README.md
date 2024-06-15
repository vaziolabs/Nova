# Nova
Nova Graphics Engine is built on top of Vulkan for streamlined Compute and Rendering purposes.

The goal of this library is to make vulkan easy to use, while offering usability regarding GPU Compute resources for users primarily using AMD, and non-CUDA based GPU's, and simultaneously providing a graphics library that provides the flexibility of engines like pygame, and  that don't require large amounts of boilerplate.

The vision for Nova is to be able to seamlessly create Machine Learning examples, or MVP-like prototypes, without having to compromise on quality, usability, or ease-of-use.

It shouldn't require 10,000 lines of boilerplate to use your GPU, nor to develop a proof of concept. This is an attempt to remediate that problem.

# TODOs
 - [ ] Object Loading
 - [ ] Camera Implementation
 - [ ] Shader Abstraction

# Example
You can find a Makefile (that may require some modifications to your system). You may be required to install the required dependencies to run this library. Currently we are running Vulkan with SDL, but there may be opportunity in the future to grow support for DX and Metal. You can rune `make` to create the executable example, and `./compute` to run the example.

You can also find an example implementation in the `main.cpp` and `NovaExample.*` wrapper implementation.
