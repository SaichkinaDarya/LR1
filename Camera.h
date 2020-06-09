#ifndef RT_SAMPLE_NEW_CAMERA_H
#define RT_SAMPLE_NEW_CAMERA_H

#include "LiteMath.h"
#include "RayTracer.h"

using namespace HydraLiteMath;

class Camera
{
public:
  Camera();
  Camera(float3 a_eye, float3 a_look_at, float3 a_up, float a_vfov, float a_aspect);


  void ComputeUVW();
  Ray genRay(float w, float h, float x, float y) const;

  const float3 &getEye() const     { return eye;  }
  void setEye(const float3 &a_eye) { eye = a_eye; };

  const float3 &getLookAt() const         { return look_at; }
  void setLookAt(const float3 &a_look_at) { look_at = a_look_at; }

  const float3 &getUp() const    {return up;}
  void setUp(const float3 &a_up) {up = a_up;}

  float getExposureTime() const     { return exposure_time;}
  void setExposureTime(float a_exp) { exposure_time = a_exp;}


protected:
  float3 eye;          //положение камеры
  float3 look_at;      //направление куда смотрит камера
  float3 up;           //направление вверх
  float3 u, v, w;      //базисные векторы системы координат камеры
  float exposure_time; //время открытия затвора камеры - выдержка
  float fov;
};


class Film
{
public:

  unsigned int h;
  unsigned int w;

  int   num_samples; // число сэмплов на пиксель
  float gamma;       //г амма для гамма-коррекции
  float inv_gamma;

  Film();
  Film(unsigned int a_w, unsigned int a_h, int a_num_samples, float a_gamma = 2.2f);
  ~Film();

  void SaveImagePPM (const std::string &filename);
  void SetPixelColor(int x, int y, HydraLiteMath::float3 col);

private:
  std::vector<HydraLiteMath::float3> pixels;
};



#endif //RT_SAMPLE_NEW_CAMERA_H
