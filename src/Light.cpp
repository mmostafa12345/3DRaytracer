#include "../include/Light.h"
#include "../include/Scene.h"
#include <iostream>
using namespace std;

Light::Light()
{
  intensity = 5.0;
  color = glm::vec3(255, 255, 255);
}

Light::~Light() {}
