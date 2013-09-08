Fore Directed Edge Bundling (FDEB)
====

Simple Force directed edge bundling algorithm for parallel coordinates implemented in OpenGL. 

I followed the paper "Force-Directed Edge Bundling for Graph Visualization" by Holten and Wijk for this. The paper can be found at http://www.win.tue.nl/~dholten/papers/forcebundles_eurovis.pdf This implementation has not been optimized so the running time is O(n^3). The most of the time is consumed in the computation of the forces. There is ample room for parallelization - I just didn't have time to do it yet. As for the data, I generated bunch of random lines and worked on them. 

Please feel free to fork the code and use it in your projects/programs.

fdeb.cpp - is the main file for force computations and OpenGL rendering.
fdeb.h - is the header file.

To build:
g++ fdeb.cpp -o app.out -lm -lGL -lGLU -lglut -Wall
