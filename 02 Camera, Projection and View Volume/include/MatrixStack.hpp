#pragma once

#include <glm/glm.hpp>
#include <stack>

struct MatrixStack {
    MatrixStack();

    void Push();
    void Pop();
    void Save(const glm::mat4& matrix);
    bool IsEmpty() const;
    glm::mat4 Top() const;

private:
    std::stack<glm::mat4> stack;
};