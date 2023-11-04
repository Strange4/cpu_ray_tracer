#include "camera/camera.h"
#include "sphere.h"
#include "hittable_list.h"
#include "utilities.h"
#include "material.h"

#include <iostream>
#include <chrono>
#include "png.hpp"

using namespace std::chrono;

hittable_list three_balls()
{

  hittable_list balls;

  shared_ptr<material> ground_material = make_shared<LambertialReflection>(color(0.8, 0.8, 0.0));
  shared_ptr<material> red_mate_material = make_shared<LambertialReflection>(color(0.98, 0.125, 0.01));
  shared_ptr<material> glass_like_material = make_shared<DielectricMaterial>(1.5);
  shared_ptr<material> silver_rough_material = make_shared<MetalReflection>(color(0.8, 0.8, 0.8), 1);
  shared_ptr<material> gold_smooth_material = make_shared<MetalReflection>(color(0.8, 0.6, 0.2));

  balls.add(make_shared<sphere>(-0.5, point3(-1, 0, 1), glass_like_material));
  balls.add(make_shared<sphere>(0.5, point3(0, 0, 1), red_mate_material));
  balls.add(make_shared<sphere>(0.5, point3(1, 0, 1), gold_smooth_material));

  // plain ground
  balls.add(make_shared<sphere>(100, point3(0, -100.5, 1), ground_material));

  return balls;
}

hittable_list random_ass_balls()
{
  hittable_list world;
  auto ground_material = make_shared<LambertialReflection>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(1000, point3(0, -1000, 0), ground_material));

  auto material_glass = make_shared<DielectricMaterial>(1.5);
  auto mate_material = make_shared<LambertialReflection>(color(0.98, 0.125, 0.01));
  auto metal_material = make_shared<MetalReflection>(color(0.7, 0.6, 0.5));

  auto big_glass_ball = make_shared<sphere>(1, point3(0, 1, 0), material_glass);
  world.add(big_glass_ball);
  auto big_mate_ball = make_shared<sphere>(1, point3(-4, 1, 0), mate_material);
  world.add(big_mate_ball);
  auto big_metal_ball = make_shared<sphere>(1, point3(4, 1, 0), metal_material);
  world.add(big_metal_ball);

  double small_ball_radius = 0.2;
  point3 small_ball_in_big_glass_ball_center = (big_glass_ball->center) - point3(0, small_ball_radius, 0);

  for (int x = -10; x < 11; x++)
  {
    for (int z = -10; z < 11; z++)
    {
      double random_material_chance = random_double();
      point3 ball_center = point3(x + 0.9 * random_double(), small_ball_radius, z + 0.9 * random_double());

      // make all the small balls don't go in the big glass ball
      if ((ball_center - small_ball_in_big_glass_ball_center).length() > 1)
      {

        shared_ptr<material> ball_material;
        if (random_material_chance < 0.8)
        {
          // 80% of the time give out a diffuse material
          auto color_tint = color::random() * color::random();
          ball_material = make_shared<LambertialReflection>(color_tint);
          world.add(make_shared<sphere>(small_ball_radius, ball_center, ball_material));
        }
        else if (random_material_chance < 0.95)
        {
          // 15% of the time make out a metal material
          auto color_tint = color::random(0.5, 1);
          auto roughness = random_double(0, 0.5);
          ball_material = make_shared<MetalReflection>(color_tint, roughness);
          world.add(make_shared<sphere>(small_ball_radius, ball_center, ball_material));
        }
        else
        {
          // 5% of the time make out a glass material
          ball_material = make_shared<DielectricMaterial>(1.5);
          world.add(make_shared<sphere>(small_ball_radius, ball_center, ball_material));
        }
      }
    }
  }
  return world;
}

int main()
{

  // all the objects
  hittable_list objects = random_ass_balls();

  // Render

  auto start = high_resolution_clock::now();
  Camera camera = Camera::create()
                      .camera_center(point3(0, 2, -3))
                      .view_point(point3(0, 0, 1))
                      .image_width(400)
                      .ray_bounce_limit(5)
                      .samples_per_pixel(5)
                      .image_name("test.png");
  camera.render(objects);

  auto end = high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<seconds>(end - start);
  std::cout << "\rRendering took: " << duration.count() << " seconds" << std::endl;
}