#include "../include/Scene.h"

int Scene::WindowWidth  = 800;
int Scene::WindowHeight = 600;

float Scene::AspectRatio = float(Scene::WindowWidth) / Scene::WindowHeight;

float Scene::Fov  = 60; //degrees
float Scene::RFov = Scene::Fov * 3.1415926535f/180.0f; //rads

float Scene::ZNear = 5.0f;
float Scene::ViewportWidth  = Scene::ZNear * tan(Scene::RFov/2);
float Scene::ViewportHeight = Scene::ViewportWidth / Scene::AspectRatio;

Scene::Scene()
{
    frameBuffer.create(WindowWidth, WindowHeight);
    timeCount = 0.0f;

    Sphere *sphere = new Sphere(glm::vec3(sin(timeCount) * 2.5f, 0.0f, 10.0f),  0.5f);
    primitives.push_back(sphere);

    Sphere *sphere2 = new Sphere(glm::vec3(0.0f, 5.0f, 10.0f),  0.5f);
    primitives.push_back(sphere2);

    Cube *cube = new Cube(glm::vec3(0.0f, 0.0f, 10.0f),  0.5f);
    primitives.push_back(cube);

    Light *light = new Light();
    light->type = LightType::Directional;
    light->range = 0.4f;
    light->intensity = 5.0f;
    light->center = glm::vec3(0.0f, 0.0f, 7.0f);
    light->dir = glm::vec3(0.0f, -1.0f, 0.0f);
    lights.push_back(light);

    depthBuffer = vector<float>(WindowWidth * WindowHeight);
    ClearDepthBuffer();
}

Scene::~Scene()
{
  for(Primitive *p : primitives) delete p;
  for(Light *l : lights) delete l;
}

void Scene::GetRayFromPixel(int pixelX, int pixelY, Ray &ray)
{
  float x =  ViewportWidth  * ( float(pixelX) / (WindowWidth /2) );
  float y = -ViewportHeight * ( float(pixelY) / (WindowHeight/2) );
  float z = ZNear;

  ray.dir = glm::vec3(x,y,z);
}

void Scene::ClearDepthBuffer()
{
    for(int i = 0; i < depthBuffer.size(); ++i) depthBuffer[i] = 999999999.0f;
}

void Scene::ClearFrameBuffer(sf::Color clearColor) {
  frameBuffer.create(WindowWidth, WindowHeight, clearColor);
}


float Scene::GetDepthAt(int pixelX, int pixelY)
{
    return depthBuffer[pixelX + WindowWidth/2 + (pixelY + WindowHeight/2) * WindowWidth];
}

void Scene::SetDepthAt(int pixelX, int pixelY, float depth)
{
    depthBuffer[pixelX + WindowWidth/2 + (pixelY + WindowHeight/2) * WindowWidth] = depth;
}

bool Scene::RayTrace(const Ray& ray, Intersection &intersection) const
{
  float minDist;
  Intersection inter;
  bool intersected = false;
  for(Primitive *primitive : primitives)
  {
      if(primitive->GetIntersection(ray, inter))
      {
        float dist = glm::length(ray.origin-inter.point);
        if (dist < minDist || !intersected) {
          minDist = dist;
          intersection = inter;
        }
        intersected = true;
      }
  }
  return intersected;
}



void Scene::Draw(sf::RenderWindow &window)
{
    ClearFrameBuffer(sf::Color::Blue);
    ClearDepthBuffer();

    timeCount += 0.005f;
    primitives[0]->center = glm::vec3(sin(timeCount) * 2.5f, -cos(timeCount) * 2.5f, 10.0f);
    primitives[1]->center = glm::vec3(-sin(timeCount * 2.0f) * 2.5f, cos(timeCount * 3.0f) * 2.5f, 10.0f);
    primitives[2]->center = glm::vec3(-sin(timeCount * 1.5f) * 2.5f, cos(timeCount * 2.0f) * 2.5f, 10.0f);

    for(int x = -WindowWidth/2; x < WindowWidth/2; ++x)
    {
        for(int y = -WindowHeight/2; y < WindowHeight/2; ++y)
        {
            Ray ray; GetRayFromPixel(x, y, ray);
            sf::Color pixelColor;
            Intersection intersection;
            if(RayTrace(ray, intersection))
            {
                SetDepthAt(x, y, intersection.point.z);
                pixelColor = sf::Color(255, 0, 0);
                for(Light *light : lights)
                {
                  pixelColor = light->LightIt(*this, pixelColor, intersection);
                }

                frameBuffer.setPixel(x + WindowWidth/2, y + WindowHeight/2, pixelColor);
            }
        }
    }

  sf::Texture texture; texture.loadFromImage(frameBuffer);

  sf::Sprite sprite; sprite.setTexture(texture);
  window.draw(sprite);
  window.display();
}
