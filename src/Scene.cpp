#include "../include/Scene.h"
#include "../include/PointLight.h"

int Scene::WindowWidth  = 800;
int Scene::WindowHeight = 600;

double Scene::AspectRatio = double(Scene::WindowWidth) / Scene::WindowHeight;

double Scene::Fov  = 60; //degrees
double Scene::RFov = Scene::Fov * 3.1415926535f/180.0; //rads

double Scene::ZNear = 5.0;
double Scene::ViewportWidth  = Scene::ZNear * tan(Scene::RFov/2);
double Scene::ViewportHeight = Scene::ViewportWidth / Scene::AspectRatio;

Scene::Scene()
{
    frameBuffer.create(WindowWidth, WindowHeight);
    timeCount = 0.0;

    Sphere *sphere = new Sphere(glm::dvec3(2.5f, 0.1, 10.0),  0.5f);
    sphere->material.diffuse = glm::vec3(0,1,1);
    primitives.push_back(sphere);

    Sphere *sphere2 = new Sphere(glm::dvec3(-2.5f, 0.0, 10.0),  0.5f);
    primitives.push_back(sphere2);

    Cube *cube = new Cube(glm::dvec3(0.0, 0.0, 10.0),  0.5f);
    primitives.push_back(cube);

    DirectionalLight *light = new DirectionalLight();
    light->intensity = 0.8f;
    light->color = glm::vec3(0, 0, 1);
    light->dir = glm::dvec3(1.0, 0.0, 0.0);
    //lights.push_back(light);

    DirectionalLight *light2 = new DirectionalLight();
    light2->intensity = .2f;
    light2->color = glm::vec3(0, 0, 1);
    light2->dir = glm::dvec3(0.0, -1.0, 0.0);
    //lights.push_back(light2);

    PointLight *light3 = new PointLight();
    light3->center = glm::dvec3(0.0,0.0,2.0);
    light3->color = glm::vec3(1, 1, 0);
    light3->range = 1.0;
    light3->intensity = 0.8;
    lights.push_back(light3);

    depthBuffer = vector<double>(WindowWidth * WindowHeight);
    ClearDepthBuffer();
}

Scene::~Scene()
{
  for(Primitive *p : primitives) delete p;
  for(Light *l : lights) delete l;
}

void Scene::GetRayFromPixel(int pixelX, int pixelY, Ray &ray)
{
  double x =  ViewportWidth  * ( double(pixelX) / (WindowWidth /2) );
  double y = -ViewportHeight * ( double(pixelY) / (WindowHeight/2) );
  double z = ZNear;

  ray.dir = glm::dvec3(x,y,z);
}

void Scene::ClearDepthBuffer()
{
    for(int i = 0; i < depthBuffer.size(); ++i) depthBuffer[i] = 999999999.0;
}

void Scene::ClearFrameBuffer(sf::Color clearColor) {
  frameBuffer.create(WindowWidth, WindowHeight, clearColor);
}


double Scene::GetDepthAt(int pixelX, int pixelY)
{
    return depthBuffer[pixelX + WindowWidth/2 + (pixelY + WindowHeight/2) * WindowWidth];
}

void Scene::SetDepthAt(int pixelX, int pixelY, double depth)
{
    depthBuffer[pixelX + WindowWidth/2 + (pixelY + WindowHeight/2) * WindowWidth] = depth;
}

bool Scene::RayTrace(const Ray& ray, Intersection &intersection) const
{
  double minDist;
  Intersection inter;
  bool intersected = false;
  for(Primitive *primitive : primitives)
  {
      if(primitive->GetIntersection(ray, inter))
      {
        double dist = glm::length(ray.origin - inter.point);
        if (dist < minDist || !intersected) {
          minDist = dist;
          intersection = inter;
        }
        intersected = true;
      }
  }
  return intersected;
}

sf::Color Scene::Vec3ToColor(glm::vec3 color)
{
  unsigned char r = (unsigned char) glm::clamp(color.r * 255.0f, 0.0f, 255.0f);
  unsigned char g = (unsigned char) glm::clamp(color.g * 255.0f, 0.0f, 255.0f);
  unsigned char b = (unsigned char) glm::clamp(color.b * 255.0f, 0.0f, 255.0f);
  return sf::Color(r,g,b);
}


void Scene::Draw(sf::RenderWindow &window)
{
    ClearFrameBuffer(sf::Color::Blue);
    ClearDepthBuffer();

    timeCount += 0.005f;
    primitives[0]->center = glm::dvec3(-2.5f, cos(timeCount*2.0) * 2.5f, 10.0 + cos(timeCount * 2.0) * 3.0);
    primitives[1]->center = glm::dvec3( 2.5f, cos(timeCount*2.0) * 2.5f, 10.0 + sin(timeCount * 4.0) * 2.5);
    primitives[2]->center = glm::dvec3(-sin(timeCount * 1.5f) * 2.5f, cos(timeCount * 2.0) * 2.5f, 10.0 + sin(timeCount * 3.0) * 2.0);

    for(int x = -WindowWidth/2; x < WindowWidth/2; ++x)
    {
        for(int y = -WindowHeight/2; y < WindowHeight/2; ++y)
        {
            Ray ray; GetRayFromPixel(x, y, ray);
            Intersection intersection;
            if(RayTrace(ray, intersection))
            {
                glm::vec3 pixelColor = glm::vec3(0);
                SetDepthAt(x, y, intersection.point.z);
                for(Light *light : lights)
                {
                  pixelColor = light->LightIt(*this, pixelColor, intersection);
                }
                frameBuffer.setPixel(x + WindowWidth/2, y + WindowHeight/2, Vec3ToColor(pixelColor));
            }
        }
    }

  for (int i = 0; i < WindowWidth; ++i) frameBuffer.setPixel(i,WindowHeight/2, sf::Color::Green);
  for (int i = 0; i < WindowHeight; ++i) frameBuffer.setPixel(WindowWidth/2,i, sf::Color::Green);
  sf::Texture texture; texture.loadFromImage(frameBuffer);

  sf::Sprite sprite; sprite.setTexture(texture);
  window.draw(sprite);
  window.display();
}
