#ifndef RT_SAMPLE_NEW_RAY_H
#define RT_SAMPLE_NEW_RAY_H

#include "LiteMath.h"

using namespace HydraLiteMath;

class GeoObject;
class LightSource;

class Ray
{
public:

  float3 o;
  float3 d;

  Ray() : o(0.0f, 0.0f, 0.0f), d(0.0, 0.0, 1.0) {};
  Ray(const float3& a_o, const float3& a_d) : o(a_o), d(a_d) {};
  Ray(const Ray& ray) : o(ray.o), d(ray.d) {};

  Ray& operator= (const Ray& rhs) { if (this == &rhs) return (*this); o = rhs.o; d = rhs.d; return (*this); };

  ~Ray(){};
};


class SimpleRT
{
public:
  SimpleRT() = default;
  SimpleRT(const int &a_max_ray_depth, const float3 &a_bg_color) : max_ray_depth(a_max_ray_depth), bg_color(a_bg_color){};
  ~SimpleRT() = default;

  float3 TraceRay(const Ray &ray, const std::vector<std::shared_ptr<GeoObject>> &geo, const int &depth);

private:
  int max_ray_depth; // максимальная глубина трассировки
  float3 bg_color;   // цвет фона
};

class WhittedRT
{
public:
    WhittedRT() = default;
    WhittedRT(const int& a_max_ray_depth, const float3& a_bg_color) : max_ray_depth(a_max_ray_depth), bg_color(a_bg_color) {};
    ~WhittedRT() = default;

    float3 TraceRay(const Ray& ray, const std::vector<std::shared_ptr<GeoObject>>& geo, const std::vector<std::shared_ptr<LightSource>>& light, int depth);
    bool ShadowRay(const Ray& ray, const std::vector<std::shared_ptr<GeoObject>>& geo);

private:
    int max_ray_depth; // максимальная глубина трассировки
    float3 bg_color;   // цвет фона
};

#endif //RT_SAMPLE_NEW_RAY_H
