/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description: Trying to Vulkan the Matrix
 *
 *        Version:  0.1.0
 *        Created:  2024-03-25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Richard Isaac
 *   Organization:  Big Stick Studios
 *
 * =====================================================================================
 */
#include <iostream>
#include "./NovaExample.h"

int main(int argc, char* argv[]) {
    NovaExample* creation = NovaExample::manifest();    // Instantiate the World
    creation->actualize();                          // Run the Simulation
    delete creation;                                // Destroy the World

    return 0;
}