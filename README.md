# 2025 Computer Graphics Final Project

Author: 111703040 游宗諺
Author: 111703022 黃鈺盛

## Table of content

- [Reference](#reference)
- [Implementation](#implementation)
- [Build Instruction](#build-instructions)

## Reference

### Lighting Grid Hierarchy for Self-illuminating Explosions

- [site](https://www.cemyuksel.com/research/lgh/)
- year: 2017
- author: Can Yuksel, Cem Yuksel

## Implementation

we can use muiltiple virtual point lights(VPL) to achieve a lot of things:
1. mimic area light
2. custom color variation scene

BUT, it is very computational to consider every light in the scene one by one. Therefore, we implement:
1. advanced data structure, no-internal-space-partition-kd-tree, our self-created kd tree (not from the paper)
2. decomposite each VPL to its neighboring grid vertices
3. store those grid vertices to its corresponding level of kd-tree
4. implement range query in kd tree
5. implement radius query in kd tree
6. implement estimate lighting for a given point in the scene
7. implement muilti threading when tracing each color ray
8. implement other objects and scenes other than HW4's default scene
9. implement light attenuation so that estimate lighting can actually works

## Build Instructions

1. in `constant.h`, to set whether to use anti-aliasing
2. `cd build`
3. `make`
4. now you will see 3 exe file
    1. main (our light grid implement result)
    2. original (the original method, which is for loop to visit each VPL)
    3. single_thread_main (just... dont try this, it takes years)
5. all the result are in `images` folder