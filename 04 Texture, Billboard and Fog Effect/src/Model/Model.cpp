//#include "Model/Model.hpp"
//
//#include <functional>
//
//Model::Model(Geometry* geo, Texture2D* text) : geometry(geo), texture(text), shininess(4.0f), color(glm::vec3(42 / 255.0f, 219 / 255.0f, 89 / 255.0f)) {
//
//}
//
//bool Model::operator==(const Model& other) const {
//    return (geometry->vao == other.geometry->vao && texture->id == other.texture->id);
//}
//
////
//namespace std {
//    template <>
//    struct hash<Model> {
//        std::size_t operator()(const Model& model) const {
//
//            std::size_t result = 17;
//            std::size_t sec = (std::hash<string>()("Hello"));
//            result = result * 8 + std::hash<unsigned int>()(model.geometry->vao ) ^ sec << 1;
//            result = result * 4 + std::hash<unsigned int>()(model.texture->id ) ^ sec >> 2;
//
//            // std::cout << res << std::endl;
//
//            return result;
//        }
//    };
//}