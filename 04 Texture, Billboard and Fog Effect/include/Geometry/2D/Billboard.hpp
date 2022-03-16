#ifndef BILLBOARD_HPP
#define BILLBOARD_HPP

#include <memory>

#include "Geometry/Geometry.hpp"
#include "Camera.hpp"

enum BillboardMode: unsigned int {
    FIXEDY = 0,
    MOVEDY = 1,
};

struct Billboard : Geometry {
    Billboard();
    void UpdateBillboard(std::unique_ptr<Camera>& camera, BillboardMode mode = BillboardMode::FIXEDY);

protected:
    void GenerateVertices() override;

};

#endif
