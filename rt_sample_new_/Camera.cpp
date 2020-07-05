#include <iostream>
#include <fstream>
#include "Camera.h"
#include <random>

void Camera::ComputeUVW()
{
  w = eye - look_at;
  w = normalize(w);

  u = cross(up, w);
  u = normalize(u);

  v = cross(u, w);
  //v = normalize(v);
}

Camera::Camera()
{
  eye = float3(0.0f, 0.0f, 20.0f);
  look_at = float3(0.0f, 0.0f, -1.0f);
  up = float3(0.0f, 1.0f, 0.0f);
  exposure_time = 1.0f;
  ComputeUVW();
}

Camera::Camera(float3 a_eye, float3 a_look_at, float3 a_up, float a_vfov, float a_aspect): eye(a_eye), look_at(a_look_at), up(a_up), fov(a_vfov)
{
  ComputeUVW();
  exposure_time = 1.0f;
}


Ray Camera::genRay(float w, float h, float x, float y) const
{
  float3 origin = eye;
  float3 dir;
  dir.x = x + 0.5f - (w / 2.0f);  // сейчас луч генерируется всегда через центр пикселя
  dir.y = y + 0.5f - (h / 2.0f);  // чтобы повысить качество картинки, нужно выбирать случайное положение внутри пикселя
  dir.z = -w / tan(fov / 2.0f);

  return Ray(origin, normalize(dir));
}

///////////////////////////////////////////////////

Film::Film()
{
  h = 480;
  w = 640;
  num_samples = 16;
  gamma = 1.0f;
  inv_gamma = 1.0f;
  pixels.assign(w * h, HydraLiteMath::float3(0.0f, 0.0f, 0.0f));
}

Film::Film(unsigned int a_w, unsigned int a_h, int a_num_samples, float a_gamma): h(a_h), w(a_w), num_samples(a_num_samples), gamma(a_gamma)
{
  inv_gamma = 1 / gamma;
  pixels.assign(w * h, HydraLiteMath::float3(0.0f, 0.0f, 0.0f));
}


Film::~Film()
{
  pixels.clear();
}

void Film::SaveImagePPM(const std::string &filename)
{
  std::string path;

  if (filename.empty())
    path = "./untitled.ppm";
  else
    path = "./" + filename + ".ppm";

  std::cout << path.c_str() << std::endl;

  std::ofstream ofs(path, std::ios::out | std::ios::binary);

  ofs << "P6\n" << w << " " << h << "\n255\n";

  for (unsigned i = 0; i < w * h; ++i)
  {
    ofs << (unsigned char)(std::min(float(1), pixels.at(i).x) * 255) <<
           (unsigned char)(std::min(float(1), pixels.at(i).y) * 255) <<
           (unsigned char)(std::min(float(1), pixels.at(i).z) * 255);
  }

  ofs.close();
}

void Film::SetPixelColor(int x, int y, HydraLiteMath::float3 col)
{
  pixels.at(y * w + x) = col;
}