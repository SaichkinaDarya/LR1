cmake_minimum_required(VERSION 3.10)
project(rt_sample_new)

set(CMAKE_CXX_STANDARD 14)


set(SRC Geometry.cpp
        Camera.cpp
        Material.cpp
        RayTracer.cpp
        main.cpp
        LightSource.cpp)

add_executable(rt_sample_new ${SRC})

#find_package(OpenMP)
#if(OpenMP_CXX_FOUND)
#    target_link_libraries(rt_sample_new PUBLIC OpenMP::OpenMP_CXX)
#endif()