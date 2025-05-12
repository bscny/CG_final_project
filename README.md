# 2025 Computer Graphics HW4

Author: 111703040 游宗諺

## Table of content

- [Build Instruction](#build-instructions)
- [Progress](#progress)
- [Summary to the modified code](#summary-to-the-modified-code)

## Build Instructions

1. in `main.cpp`, decide what final output to build
2. `cd build`
3. `make`
4. `./out`

## Progress

- Default function:
    - [x] multiple sphere
    - [x] nearest intersection point
    - [x] Recursive Ray Tracer with shadow
    - [x] Recursive Ray Tracer with shadow + reflection
    - [x] Recursive Ray Tracer with shadow + reflection + transmission
- Bonus:
    - [x] anti-aliasing
    - [x] plane
    - [x] tetrahedron
    - [ ] multi lights

## Summary to the Modified Code

1. `main.cpp`
    - color function implementation, the main recursive function
    - trace_shadow_ray function implementation, in order to cast shadows
2. `vec3`, `ray`
    - seperate .cpp and .h file from Hw1
3. `object.h` and `object.cpp` to implement polymorphism
4. `sphere.h` and `sphere.cpp`
5. `triangle.h` and `triangle.cpp`
6. `mesh.h` and `mesh.cpp`
    - this mostly uses `Triangle` to form some basic meshes, for example, tetrahedron