#ifndef MATERIAL_H
#define MATERIAL_H

#include "glm/glm.hpp"
#include <SFML/Graphics.hpp>

class Material
{
public:
    Material();
    virtual ~Material();

    glm::vec3 ambient, diffuse, specular;

    double shininess;
    double roughness;
};

#endif
