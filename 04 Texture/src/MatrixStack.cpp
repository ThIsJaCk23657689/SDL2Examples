#include "MatrixStack.hpp"

#include <iostream>

MatrixStack::MatrixStack() {
    stack.push(glm::mat4(1.0f));
}

void MatrixStack::Push() {
    stack.push(stack.top());
}

void MatrixStack::Pop() {
    if (IsEmpty()) {
        std::cerr << "The matrix stack is empty!!!" << std::endl;
        return;
    }
    stack.pop();
}

void MatrixStack::Save(const glm::mat4& matrix) {
    // 記得要先 Pop，這個進來的 matrix 必須搭配 glm 的 gtc 版本。
    // 會先把原本的 Top 的矩陣拿出來乘之後，再透過 Save 丟回堆疊。
    stack.pop();
    stack.push(matrix);
}

bool MatrixStack::IsEmpty() const {
    return stack.empty();
}

glm::mat4 MatrixStack::Top() const {
    return stack.top();
}