#include <iostream>
#include <vector>
#include <random>

#include "LiteMath.h"
#include "Geometry.h"
#include "Camera.h"
#include "LightSource.h"

using namespace HydraLiteMath;

struct Scene {
    std::vector<std::shared_ptr<GeoObject>> geoObjects;
    std::vector<std::shared_ptr<LightSource>> lightSources;
};

void RenderScene(uint32_t w, uint32_t h, uint32_t num_samples, const Scene& scene, const Camera& cam, const std::string& filename)
{
    auto background_color = float3(0.5f, 0.7f, 1.0f);
    auto film = std::make_unique<Film>(w, h, num_samples);
    auto tracer = std::make_unique<WhittedRT>(16, background_color);

    float invWidth = 1.0f / float(w);
    float invHeight = 1.0f / float(h);


    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            float3 pixel_color = float3(0.0f, 0.0f, 0.0f);

            for (int s = 0; s < num_samples; ++s)
            {
                Ray ray = cam.genRay(w, h, x, h - y); //генерируем луч из камеры через текущий пиксель

                pixel_color += tracer->TraceRay(ray, scene.geoObjects, scene.lightSources, 0); //трассируем луч и получаем цвет
            }

            pixel_color /= film->num_samples;      // усредняем полученные цвета
            pixel_color *= cam.getExposureTime();  // умножаем на время экспонирования сенсора - выдержка виртуальной камеры

            film->SetPixelColor(x, y, pixel_color); //записываем цвет на виртуальный сенсор
        }
    }

    film->SaveImagePPM(filename); //сохраняем картинку

}

void test_scene1()
{
    Scene scene;

    auto lightSource1 = std::make_shared<LightSource>(float3(0.0f, 12.0f, 0.0f), float3(1.0f, 1.0f, 0.0f));
    auto lightSource2 = std::make_shared<LightSource>(float3(2.0f, 5.0f, 0.0f), float3(1.0f, 0.0f, 0.0f));
    scene.lightSources.push_back(lightSource1);
    scene.lightSources.push_back(lightSource2);

    auto plane1 = std::make_shared<Plane>(float3(+0.0f, -1.0f, +0.0f), float3(0.0f, 1.0f, 0.0f), new Defuse(float3(0.0f, .80f, 0.20f)));
    auto sph = std::make_shared<Sphere>(float3(-6.5f, +7.0f, +1.65f), 1.5, new IdealMirror(float3(.80f, 0.80f, 0.16f)));
    auto sph2 = std::make_shared<Sphere>(float3(-2.0f, +2.5f, +0.0f), 3, new Defuse(float3(0.70f, 0.72f, 1.76f)));
    auto par = std::make_shared<Parallel>(float3(+2.0f, -7.0f, +4.0f), float3(+5.0f, +3.0f, +6.0f), new Defuse(float3(1.0f, 0.0f, 1.0f)));
    auto tr = std::make_shared<Triangle>(float3(+3.0f, 4.0f, +1.0f), float3(+6.0f, +9.0f, +3.0f), float3(-5.0f, +4.0f, +1.0f), new Defuse(float3(0.0f, 1.0f, 0.0f)));
    scene.geoObjects.push_back(plane1);
    scene.geoObjects.push_back(sph);
    scene.geoObjects.push_back(par);
    scene.geoObjects.push_back(tr);
    scene.geoObjects.push_back(sph2);


    float3 eye(0.0f, 2.0f, 20.0f);
    float3 lookat(0.0f, 2.0f, 0.0f);
    float3 up(0.0f, 1.0f, 0.0f);
    float field_of_view = 90.0f;
    unsigned int w = 640;
    unsigned int h = 480;
    Camera cam(eye, lookat, up, field_of_view, float(w) / float(h));

    RenderScene(w, h, 1, scene, cam, "test_scene1");
}


void random_scene(int n_objects)
{
    Scene scene;

    auto plane1 = std::make_shared<Plane>(float3(+0.0f, -1.0f, +0.0f), float3(0.0f, 1.0f, 0.0f), new IdealMirror(float3(0.3f, 0.3f, 0.3f)));
    scene.geoObjects.push_back(plane1);
    auto par = std::make_shared<Parallel>(float3(+0.0f, -1.0f, +0.0f), float3(+2.0f, +2.0f, +2.0f), new IdealMirror(float3(1.00f, 0.32f, 0.36f)));
    scene.geoObjects.push_back(par);
    int k = floor(sqrt(n_objects));

    std::random_device rand;
    std::mt19937 rng(rand());
    std::uniform_real_distribution<float> gen(0.0f, 1.0f);

    for (int i = 0; i < n_objects; ++i)
    {
        float3 center(-10.0f + 20.0f * gen(rng), 10.0f * gen(rng), -30.0f + 40.0f * gen(rng));
        auto sph = std::make_shared<Sphere>(center, 0.05f + 3.0f * gen(rng), new Defuse(float3(gen(rng), gen(rng), gen(rng))));
        scene.geoObjects.push_back(sph);
    }

    float3 eye(0.0f, 2.0f, 20.0f);
    float3 lookat(0.0f, 2.0f, 0.0f);
    float3 up(0.0f, 1.0f, 0.0f);
    float field_of_view = 90.0f;
    unsigned int w = 640;
    unsigned int h = 480;
    Camera cam(eye, lookat, up, field_of_view, float(w) / float(h));

    RenderScene(w, h, 1, scene, cam, "random_scene");
}

int main()
{
    test_scene1();
    //random_scene(100);

    return 0;
}

