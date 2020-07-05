#include "Geometry.h"


bool Plane::Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const
{
  surf.t = dot((point - ray.o), normal) / dot(ray.d, normal);

  if(surf.t > t_min && surf.t < t_max)
  {
    surf.hit      = true;
    surf.hitPoint = ray.o + surf.t * ray.d;
    surf.normal   = normal;
    surf.m_ptr    = m_ptr;
    return true;
  }

  return false;
}

/////////////////////////////////////////


//Для пересечения со сферой подставляем луч в уравнение сферы и решаем получившееся квадратное уравнение
//Решения этого уравнения - точки пересечения луча со сферой
bool Sphere::Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const
{

  float3 k = ray.o - center;

  float a = dot(ray.d, ray.d);
  float b = dot(2 * k, ray.d);
  float c = dot(k, k) - r_sq;

  float d = b * b - 4 * a * c;

  if (d < 0) return false;

  surf.t = (-b - sqrt(d)) / 2 * a;

  if (surf.t > t_min && surf.t < t_max)
  {
    surf.hit      = true;
    surf.hitPoint = ray.o + surf.t * ray.d;
    surf.normal   = normalize(surf.hitPoint - center);
    surf.m_ptr    = m_ptr;
    return true;
  }

  surf.t = (-b + sqrt(d)) / 2 * a;
  if (surf.t > t_min && surf.t < t_max)
  {
    surf.hit      = true;
    surf.hitPoint = ray.o + surf.t * ray.d;
    surf.normal   = normalize(surf.hitPoint - center);
    surf.m_ptr    = m_ptr;
    return true;
  }

  return false;
}

bool Parallel::Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const
{
	float dirfracX = 1.0f / ray.d.x;
	float dirfracY = 1.0f / ray.d.y;
	float dirfracZ = 1.0f / ray.d.z;
	float x1 = (t_min.x - ray.o.x) * dirfracX;
	float x2 = (t_max.x - ray.o.x) * dirfracX;
	float y1 = (t_min.y - ray.o.y) * dirfracY;
	float y2 = (t_max.y - ray.o.y) * dirfracY;
	float z1 = (t_min.z - ray.o.z) * dirfracZ;
	float z2 = (t_max.z - ray.o.z) * dirfracZ;

	float tMin = max(max(min(x1, x2), min(y1, y2)), min(z1, z2));
	float tMax = min(min(max(x1, x2), max(y1, y2)), max(z1, z2));


	surf.t = tMin;
	if (tMin < tMax && tMax > 0 && surf.t > tmin && surf.t < tmax) {
		surf.hit = true;
		surf.hitPoint = ray.o + surf.t * ray.d;
		surf.normal = normalize(surf.hitPoint);
		surf.m_ptr = m_ptr;
		return true;
	}
	return false;
}

bool Triangle::Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const
{
	float3 E1 = b - a;
	float3 E2 = c - a;
	float3 T = ray.o - a;
	float3 D = ray.d;
	float3 P = cross(D, E2);
	float3 Q = cross(T, E1);
	float det = dot(E1, P);

	if (det < tmin && det > tmax) {
		return false;
	}

	float invDet = 1 / det;
	float u = dot(T, P) * invDet;

	if (u < 0 || u > 1) {
		return false;
	}

	float v = dot(ray.d, Q) * invDet;

	if (v < 0 || u + v > 1) {
		return false;
	}

	surf.t = dot(E2, Q) * invDet;
	if (surf.t > tmin && surf.t < tmax) {
		surf.hit = true;
		surf.hitPoint = float3(surf.t, u, v);
		surf.normal = cross((b - a), (c - a));
		surf.m_ptr = m_ptr;
		return true;
	}
	return false;
}

bool Disk::Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const
{
	float denom = dot(n, ray.d);
	float t;
	if (denom > tmin && denom < tmax) {
		float3 p0l0 = o - ray.o;
		t = dot(p0l0, n) / denom;
		if (t >= 0) {
			float3 p = ray.o + ray.d * t;
			float3 v = p - o;
			float d2 = dot(v, v);
			if (sqrtf(d2) <= r) {
				surf.t = t;
				surf.hit = true;
				surf.hitPoint = ray.o + t * ray.d;
				surf.normal = normalize(surf.hitPoint);
				surf.m_ptr = m_ptr;
				return true;
			}
		}
	}

	return false;

}