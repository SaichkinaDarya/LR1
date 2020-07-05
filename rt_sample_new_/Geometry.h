#ifndef RT_SAMPLE_NEW_GEOMETRY_H
#define RT_SAMPLE_NEW_GEOMETRY_H

#include "LiteMath.h"
#include "RayTracer.h"
#include "Material.h"

using namespace HydraLiteMath;

//Базовый виртуальный класс для геометрического объекта, с которым может пересекаться луч
class GeoObject
{
public:
  virtual bool Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const = 0;

  std::shared_ptr<Material> m_ptr;

  explicit GeoObject(Material *a_m) : m_ptr(a_m) {} ;
  virtual ~GeoObject() = default;
};

//Бесконечная плоскость
class Plane : public GeoObject
{
public:
  Plane(const float3 a_point, const float3 a_normal, Material *a_m) : GeoObject(a_m), point(a_point), normal(a_normal) {}
  ~Plane() = default;

  bool Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const override;

private:
  float3 point;
  float3 normal;
};


class Sphere : public GeoObject
{
public:

  Sphere(const float3 &a_center, const float &a_r, Material *a_m): GeoObject(a_m), center(a_center), r(a_r), r_sq(a_r * a_r) {}
  ~Sphere() = default;

  bool Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const override;

private:
  float3 center;
  float r;
  float r_sq;

};

class Parallel : public GeoObject {
public:
    Parallel(const float3& a_min, const float3& a_max, Material* a_m) :
        GeoObject(a_m), t_min(a_min), t_max(a_max) {}
    ~Parallel() = default;

    bool Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const override;

private:
    float3 t_min;
    float3 t_max;
    float r_sq;
};

class Triangle : public GeoObject {
public:
    Triangle(const float3& a, const float3& b, const float3& c, Material* a_m) : a(a), b(b), c(c), GeoObject(a_m) {}
    ~Triangle() = default;

    bool Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const override;

private:
    float3 a;
    float3 b;
    float3 c;
};

class Disk : public GeoObject {
public:
    Disk(const float3& o, const float& r, const float3& n, Material* a_m) : o(o), r(r), n(n), GeoObject(a_m) {}
    ~Disk() = default;

    bool Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const override;

private:
    float3 o;
    float r;
    float3 n;
};

#endif //RT_SAMPLE_NEW_GEOMETRY_H
