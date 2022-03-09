#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/glm.hpp>

#include "Texture/Texture2D.hpp"

// TODO:: 建立一個 Material Manager 應該會好一點，不然 World 會有太多的 Material。
// 問題：每一個不同的顏色就要重新建立一個 Material，是不是不太優?
// 但一個物體可能會有很多種材質貼圖，但顏色只會有一種，所以說顏色是不是該跟隨著 Entity?

struct Material {
    Material();
    Material(glm::vec3 col, float shin = 4.0f, Texture2D* tex = nullptr);
    Material(Texture2D* tex);

    Texture2D* texture;

    glm::vec3 color;
    float shininess;
    bool mix;
};

#endif