#pragma once
#include <iostream>
#include <stdexcept>

#define DIE_ON_NULL(VALUE, MSG) \
if (VALUE == NULL)  \
{ \
    std::cout << MSG << std::endl; \
    throw std::runtime_error(MSG); \
} \

struct Color
{
    float r;
    float g;
    float b;
    float a;
};