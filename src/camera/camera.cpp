#include "../common.h"
#include "camera.h"
#include "BS_thread_pool.hpp"
#include <chrono>

#include <string>

CameraBuilder &CameraBuilder::ray_bounce_limit(int limit)
{
    camera.ray_bounce_limit = limit;
    return *this;
}

CameraBuilder &CameraBuilder::samples_per_pixel(int sample_count)
{
    camera.samples_per_pixel = sample_count;
    return *this;
}

CameraBuilder &CameraBuilder::fov(int fov_in_degrees)
{
    camera.fov = fov_in_degrees;
    return *this;
}

CameraBuilder &CameraBuilder::focus_distance(double focus_distance)
{
    camera.focus_distance = focus_distance;
    return *this;
}

CameraBuilder &CameraBuilder::defocus_angle(double defocus_angle)
{
    camera.defocus_angle = defocus_angle;
    return *this;
}

CameraBuilder &CameraBuilder::view_point(point3 view_point)
{
    camera.view_point = view_point;
    return *this;
}

CameraBuilder &CameraBuilder::camera_center(point3 camera_center)
{
    camera.camera_center = camera_center;
    return *this;
}

CameraBuilder &CameraBuilder::image_width(int _image_width)
{
    camera.image_width = _image_width;
    return *this;
}

CameraBuilder &CameraBuilder::image_name(std::string _image_name)
{
    camera.image_name = _image_name;
    return *this;
}

CameraBuilder::operator Camera()
{

    // unit vectors denoting the depth, width, and height vectors respectively
    vec3 forward_direction = unit_vector(camera.view_point - camera.camera_center);
    vec3 camera_side_direction = unit_vector(cross(camera.vup, forward_direction));
    vec3 camera_up_direction = cross(forward_direction, camera_side_direction);

    // setting the viewport dimentions
    camera.image_height = static_cast<int>(camera.image_width / camera.aspect_ratio);
    double viewport_width = camera.focus_distance * tan(degrees_to_radians(camera.fov) / 2) * 2;
    double viewport_height = viewport_width / (double(camera.image_width) / double(camera.image_height));

    // determining the vectors for the x and y of the viewport
    vec3 viewport_x = camera_side_direction * viewport_width;
    vec3 viewport_y = -camera_up_direction * viewport_height;

    // calculate the distance between pixels to know how often to send rays
    camera.pixel_delta_x = viewport_x / camera.image_width;
    camera.pixel_delta_y = viewport_y / camera.image_height;

    point3 viewport_top_left = camera.camera_center + (camera.focus_distance * forward_direction) - viewport_x / 2 - viewport_y / 2;
    camera.pixel_00_location = viewport_top_left;

    // setting the size of the defocus disck
    double defocus_radius = camera.focus_distance * tan(degrees_to_radians(camera.defocus_angle) / 2);
    camera.defocus_disc_x = camera_side_direction * defocus_radius;
    camera.defocus_disc_y = camera_up_direction * defocus_radius;

    return camera;
}

color Camera::ray_color(const ray &r, const hittable &hittable, int max_depth)
{
    if (max_depth == 0)
    {
        return color(0, 0, 0);
    }
    hit_record hit_rec;
    interval from_camera(0.001, infinity);
    if (hittable.hit(r, from_camera, hit_rec))
    {
        ray out_ray;
        color color_tint;
        if (hit_rec.mat->scatter(r, hit_rec, color_tint, out_ray))
        {
            return color_tint * ray_color(out_ray, hittable, max_depth - 1);
        }
        return color(0, 0, 0);
    }

    vec3 direction = unit_vector(r.direction());
    double a = 0.5 * (direction.y() + 1.0);

    color from_color = color(1, 1, 1);
    color to_color = color(0.5, 0.7, 1);
    return from_color + (to_color - from_color) * a;
}

vec3 Camera::pixel_sample_square() const
{
    // Returns a random point in the square surrounding a pixel at the origin.
    auto px = -0.5 + random_double();
    auto py = -0.5 + random_double();
    return (px * pixel_delta_x) + (py * pixel_delta_y);
}

point3 Camera::defocus_disk_sample() const
{
    vec3 p = random_in_unit_disk();
    return camera_center + (p.x() * defocus_disc_x) + (p.y() * defocus_disc_y);
}

ray Camera::create_ray(int pixel_x, int pixel_y) const
{
    auto pixel_center = pixel_00_location + (pixel_x * pixel_delta_x) + (pixel_y * pixel_delta_y);
    auto pixel_sample = pixel_center + pixel_sample_square();

    auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
}

color Camera::get_pixel_color(int x, int y, const hittable &objects)
{
    color pixel_color(0, 0, 0);
    for (int sample = 0; sample < samples_per_pixel; ++sample)
    {
        ray r = create_ray(x, y);
        pixel_color += ray_color(r, objects, Camera::ray_bounce_limit);
    }
    return pixel_color * (1.0 / samples_per_pixel);
}

void Camera::render(const hittable_list &objects)
{
    // create an image to set the output
    png::image<png::rgb_pixel> image(image_width, image_height);

    BS::thread_pool pool;
    BS::multi_future<std::vector<color>> futures;
    for (int y = 0; y < image_height; ++y)
    {

        std::cout << "\rScanlines remaining...: " << (image_height - y) << ' ' << std::flush;
        futures.push_back(pool.submit([this, y, objects]
                                      {
                                            std::vector<color> line;
                                          for (int x = 0; x < image_width; ++x)
                                          {
                                              line.push_back(get_pixel_color(x, y, objects));
                                          } 
                                          return line; }));
    }
    std::cout << "\rWaiting for pixels to be done" << std::flush;
    double initial_tasks = pool.get_tasks_total();
    auto start_time = std::chrono::high_resolution_clock::now();
    while (pool.get_tasks_total() != 0)
    {
        double current_tasks = pool.get_tasks_total();
        auto current_time = std::chrono::high_resolution_clock::now();
        double milliseconds_average = double(std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count()) / (initial_tasks - current_tasks);
        std::cout << '\r' << pool.get_tasks_total() << " tasks total, " << pool.get_tasks_running() << " tasks running, " << pool.get_tasks_queued() << " tasks queued. "
                  << "Average time per task: " << milliseconds_average << "ms, "
                  << "Remaining time: " << int((milliseconds_average * current_tasks) / 1000) << "s.         " << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    std::vector<std::vector<color>> lines = futures.get();
    for (int y = 0; y < image_height; y++)
    {
        std::vector<color> line = lines.at(y);
        for (int x = 0; x < image_width; x++)
        {
            write_color(x, y, image, line.at(x));
        }
    }

    image.write(image_name);
    std::cout << "\rDone.                 \n";
}

CameraBuilder Camera::create() { return CameraBuilder(); }