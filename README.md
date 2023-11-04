# An Implementation of a Ray Tracer in C++

This implementation only uses the basic ideas of ray tracing and some multithreading to speed the calculations

![Final render](./previous%20renders/2k_random_balls.png)

# Insipirations

Heavily inspired by [Ray tracing in one weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) 

# How to run?

### 1. Install libraries

You will need [png++-0.2.9](https://download.savannah.nongnu.org/releases/pngpp/) which is a thin wrapper around `libpng` which you also need to install.

```bash
# This is to install libpng
sudo apt-get install libpng-dev
```

[BS_thread_pool](https://github.com/bshoshany/thread-pool#getting-started) is also needed for this demo


```bash
# If you have vcpkg and don't want to break your head
./vcpkg install bshoshany-thread-pool
```

### 2. Run with cmake
```bash
 # if you don't have vcpkg then don't add the last part
cmake -S . -B build/"
cmake --build build/ --config Release
./build/ray_tracer.exe
```


# Media of progress

Spheres, low quality, uses normals as color
![spheres with normal as the color](./previous%20renders/4_balls_normal_color.png)

Testing with a reflective surface

![reflective surface sphere](./previous%20renders/1080p_reflective_closeup.png)

Different roughness

![rough metal sphere](./previous%20renders/1080p_rough_metal.png)

A glass ball with mirrored light

![glass sphere](./previous%20renders/glass_ball.png)

Testing with camera angle and focus blur

![focus camera sphere](./previous%20renders/unfocused_balls.png)

Final render

![Final render](./previous%20renders/2k_random_balls.png)
