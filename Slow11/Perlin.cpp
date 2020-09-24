#include "Perlin.h"

Perlin::Perlin()
{
    
}

Perlin::~Perlin()
{
}

//assure that x and y is a multipe times of 0.1
float Perlin::GetHeight(float x, float y)
{
    XMFLOAT2 bottomleft = FakeRandomValue(x - 0.5, y - 0.5);
    XMFLOAT2 bottomright = FakeRandomValue(x + 0.5, y - 0.5);
    XMFLOAT2 topleft = FakeRandomValue(x - 0.5, y + 0.5);
    XMFLOAT2 topright = FakeRandomValue(x + 0.5, y + 0.5);
    return  bottomleft.x + bottomleft.y + bottomright.x + bottomright.y + topleft.x + topleft.y + topright.x + topright.y;
}

XMFLOAT2 Perlin::FakeRandomValue(float x, float y)
{
    float fakeX =  sinf(x * 1785 + y * 8051) + cosf(3021 * x + 3245 * y);
    float fakeY = sinf(x * 5685 + y * 8211) + cosf(3121 * x + 4145 * y);
    float base = sqrt(fakeX * fakeX + fakeY * fakeY);
    return XMFLOAT2(fakeX, fakeY);
}
