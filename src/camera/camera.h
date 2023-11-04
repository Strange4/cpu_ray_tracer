#ifndef CAMERA_H
#define CAMERA_H

#include "../hittable.h"
#include "../hittable_list.h"
#include "../material.h"
#include "../common.h"

#include <string>

class CameraBuilder;

class Camera
{

private:
  std::string image_name = "image.png";

  int samples_per_pixel = 10;

  int ray_bounce_limit = 10;

  double aspect_ratio = 16.0 / 9.0; // the aspect ratio for the image

  int fov = 90; // sets the horizontal fov;

  int image_width = 400; // in pixels

  int image_height;

  double viewport_width;

  point3 camera_center = point3(0, 0, 1);

  point3 view_point = point3();

  vec3 vup = vec3(0, 1, 0); // the vector determining where up is

  double defocus_angle = 0; // the cone angle determining the "size" of the blur. The bigger, the more blur

  double focus_distance = 10; // the distance from the camera where objects will be in focus

  vec3 pixel_delta_x;

  vec3 pixel_delta_y;

  point3 pixel_00_location;

  // the vectors representing the size and direction of the "lens" aperture
  vec3 defocus_disc_x;
  vec3 defocus_disc_y;

  color get_pixel_color(int x, int y, const hittable &hittable);
  color ray_color(const ray &r, const hittable &hittable, int max_depth);
  ray create_ray(int pixel_x, int pixel_y) const;
  vec3 pixel_sample_square() const;
  point3 defocus_disk_sample() const;

public:
  friend class CameraBuilder;
  static CameraBuilder create();
  void render(const hittable_list &objects);
};

class CameraBuilder
{
private:
  Camera camera;

public:
  CameraBuilder() : camera() {}
  CameraBuilder &image_width(int image_width);
  CameraBuilder &image_name(std::string image_name);
  CameraBuilder &fov(int fov_in_degrees);
  CameraBuilder &focus_distance(double focus_distance);
  CameraBuilder &camera_center(point3 camera_center);
  CameraBuilder &view_point(point3 view_point);
  CameraBuilder &defocus_angle(double angle_in_degrees);
  CameraBuilder &ray_bounce_limit(int limit);
  CameraBuilder &samples_per_pixel(int sample_count);
  operator Camera();
};

#endif