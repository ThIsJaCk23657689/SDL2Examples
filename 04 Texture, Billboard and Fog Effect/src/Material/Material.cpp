#include "Material/Material.hpp"

Material::Material() : color({ 0.8, 0.8, 0.8 }), shininess(4.0f), texture(nullptr), mix(false) {}

Material::Material(glm::vec3 col, float shin, Texture2D* tex) : color(col), shininess(shin), texture(tex), mix(false) {}

Material::Material(Texture2D* tex) : color({ 0.8, 0.8, 0.8 }), shininess(4.0f), texture(tex), mix(false) {}