#pragma once
#include <vector>
#include <cstdint>

typedef struct {
    uint8_t r, g, b, a;
} Color;

typedef struct {
    float r, g, b, a;
} ColorF;

typedef struct {
    int x, y;
} Vertex;

typedef struct {
    float x, y;
} VertexF;
