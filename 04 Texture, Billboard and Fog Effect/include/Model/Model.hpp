#ifndef MODEL_HPP
#define MODEL_HPP

#include "Geometry/Geometry.hpp";
#include "Texture/Texture2D.hpp"

struct Model {
    Model(Geometry* const geo, Texture2D* text = nullptr);

    Geometry *geometry;
    Texture2D *texture;
};

#endif
