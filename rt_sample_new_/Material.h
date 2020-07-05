#ifndef RT_SAMPLE_NEW_MATERIAL_H
#define RT_SAMPLE_NEW_MATERIAL_H

#include "LiteMath.h"
#include "RayTracer.h"

using namespace HydraLiteMath;

class Material;

//Класс, хранящий данные для найденной точки пересечения
class SurfHit
{
public:
  bool   hit;      // наличие пересечения
  float3 hitPoint; // точка пересечения
  float3 normal;   // вектор нормали к поверхности в точке пересечения
  float  t;        // расстояние от начала луча до точки пересечения

  std::shared_ptr<Material> m_ptr; //материал объекта, с которым нашли пересечение

  SurfHit() : hit(false), hitPoint(), normal(), m_ptr(nullptr), t(-1) {};
  ~SurfHit() = default;

  SurfHit& operator= (const SurfHit &rhs);
};

class Material
{
public:
  Material(){};
  ~Material(){};

  virtual bool Scatter(const Ray &ray_in, const SurfHit &surf, float3 &attenuation, Ray &ray_out) = 0;
};

class IdealMirror : public Material
{
public:
  IdealMirror(const float3 &a_color) : Material(), color(a_color){};

  bool Scatter(const Ray &ray_in, const SurfHit &surf, float3 &attenuation, Ray &ray_out) override;

  float3 color;
};

class Defuse : public Material
{
public:
    Defuse(const float3& a_color) : Material(), color(a_color) {};

    bool Scatter(const Ray& ray_in, const SurfHit& surf, float3& attenuation, Ray& ray_out) override;

    float3 color;
};

class Light : public Material
{
public:
    Light(const float3& a_color) : Material(), color(a_color) {};

    bool Scatter(const Ray& ray_in, const SurfHit& surf, float3& attenuation, Ray& ray_out) override;

    float3 color;
};

#endif //RT_SAMPLE_NEW_MATERIAL_H
