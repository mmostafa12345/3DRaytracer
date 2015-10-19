#include "../include/DirectionalLight.h"
#include "../include/Scene.h"
#include <iostream>

using namespace std;


DirectionalLight::DirectionalLight()
{
  dir = glm::dvec3(0.0, 0.0, 1.0);
}

DirectionalLight::~DirectionalLight() {}

glm::vec3 DirectionalLight::LightIt(const Scene& scene, glm::vec3 pixelColor, const Intersection& intersection)
{
  glm::vec3 color = glm::vec3(0.0f);
  glm::vec3 ambient, diffuse, specular;
  ambient = intersection.material->ambient;
  diffuse = glm::vec3(0.0f);
  specular = glm::vec3(0.0f);

  Ray ray;
  ray.origin = intersection.point - this->dir * 9999.9;
  ray.dir = this->dir;

  Intersection lightInter = intersection;
  scene.RayTrace(ray, lightInter);

  double epsilon = 0.0001;
  double d1 = glm::length(intersection.point - ray.origin);
  double d2 = glm::length(lightInter.point - ray.origin);
  bool isInShadow = abs(d2 - d1) > epsilon;
  if(!isInShadow)
  {
    double f = glm::dot(-this->dir, intersection.normal);
    f*= intensity;
    f = glm::clamp(f, 0.0, 1.0);
    diffuse = intersection.material->diffuse * float(f);
  }

  color = (ambient + diffuse + specular) * this->color;
  return pixelColor + color;
}
