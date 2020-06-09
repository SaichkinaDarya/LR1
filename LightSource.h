#pragma once
#include "LiteMath.h"
#include "RayTracer.h"

class LightSource {
public:
	explicit LightSource(const float3 position, const float3 color) : position(position), color(color) {};
	virtual ~LightSource() = default;

	float3 position;
	float3 color;
};