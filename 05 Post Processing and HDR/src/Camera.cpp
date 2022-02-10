#include "Camera.hpp"

#include "SDL.h"

Camera::Camera(glm::vec3 pos, bool is_prscpt) :
    Pitch(0.0f),
    Yaw(0.0f),
    Roll(0.0f),
    Position(pos),
    Velocity(glm::vec3(0.0f)),
    Acceleration(glm::vec3(0.0f)),
    Front(glm::vec3(0.0, 0.0, -1.0)),
    Right(glm::vec3(1.0, 0.0, 0.0)),
    Up(glm::vec3(0.0, 1.0, 0.0)),
    Target(pos + Front),
    MoveSpeed(150.0f),
    MouseSensitivity(0.1f),
    Zoom(45.0f),
    MouseControl(false),
    IsPerspective(is_prscpt),
    FollowTarget(false) {
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    UpdateCameraVectors();

    // Default Value
    frustum.near = 0.1f;
    frustum.far = 500.0f;
    UpdateProjectionParameters();
}

Camera::Camera(glm::vec3 pos, glm::vec3 target, bool is_prscpt) :
    Pitch(0.0f),
    Yaw(0.0f),
    Roll(0.0f),
    Position(pos),
    Velocity(glm::vec3(0.0f)),
    Acceleration(glm::vec3(0.0f)),
    Front(glm::vec3(0.0, 0.0, -1.0)),
    Right(glm::vec3(1.0, 0.0, 0.0)),
    Up(glm::vec3(0.0, 1.0, 0.0)),
    Target(target),
    Distance(0.0f),
    MoveSpeed(150.0f),
    MouseSensitivity(0.1f),
    Zoom(45.0f),
    MouseControl(false),
    IsPerspective(is_prscpt),
    FollowTarget(true) {
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    Distance = glm::length(Position - Target);
    UpdateCameraVectors();

    // Default Value
    frustum.near = 0.1f;
    frustum.far = 500.0f;
    UpdateProjectionParameters();
}

void Camera::UpdateCameraVectors() {
    // 更新攝影機的三軸方向向量：前、右、上，注意攝影機朝向負 z 軸
    glm::vec4 front = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);

    if (FollowTarget) {
        // 代表此相機的旋轉是依據他的 target 的，所以先求出半徑（球形座標相機）
        glm::vec4 radius = glm::vec4(0.0f, 0.0f, Distance, 1.0f);

        // 這邊 FollowTarget 我只專門用給那三個正交攝影機，設計專門跟隨著第一人稱視角的攝影機，所以說這邊我就不考慮 roll 了
        glm::mat4 rotateMatrix = glm::mat4(1.0f);
        rotateMatrix = glm::rotate(rotateMatrix, glm::radians(-Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateMatrix = glm::rotate(rotateMatrix, glm::radians(Pitch), glm::vec3(1.0f, 0.0f, 0.0f));

        // 攝影機會跟隨著目標移動
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(Target.x, Target.y, Target.z));

        glm::vec4 pos = translation * rotateMatrix * radius;

        // 求出位置
        Position = glm::vec3(pos.x, pos.y, pos.z);

        // Gram-Schmidt Orthogonalization 正交化求攝影機三軸
        Front = glm::normalize(Target - Position);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    } else {
        // 利用 rotation 矩陣來旋轉攝影機的【前】向量，pitch 為垂直旋轉、yaw 為水平旋轉、roll 為側滾旋轉，單位是角度
        // 因為 rotation 是逆時針旋轉(角度為正時)，而因為攝影機朝向負 z 軸，滑鼠的相對座標（螢幕坐標系）往右是正（向左轉），往左是負（向右轉）
        // 所以 Yaw 必須轉為負值，或者是將旋轉軸反過來也可以，另外旋轉的順序也有關係，一般而言是 YXZ 的順序
        // 而萬向鎖就是只要 Pitch 旋轉是 ±90°，因為 Pitch 旋轉會影響到 Roll，所以此時 Yaw 以及 Roll 旋轉將會是一樣的效果（失去一個旋轉自由度）
        glm::mat4 rotateMatrix = glm::mat4(1.0f);
        rotateMatrix = glm::rotate(rotateMatrix, glm::radians(-Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateMatrix = glm::rotate(rotateMatrix, glm::radians(Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateMatrix = glm::rotate(rotateMatrix, glm::radians(Roll), glm::vec3(0.0f, 0.0f, -1.0f));

        // 記住攝影機的初始【前】向量永遠面向世界座標的 -z 軸。
        front = rotateMatrix * front;

        // 將 front 當作旋轉軸，並且 Roll 為度數，去旋轉 WorldUp 即可。
        WorldUp = glm::rotate(glm::mat4(1.0f), glm::radians(Roll), glm::vec3(front.x, front.y, front.z)) *
            glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

        // Gram-Schmidt Orthogonalization 正交化求攝影機三軸
        Front = glm::normalize(glm::vec3(front.x, front.y, front.z));
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
}

void Camera::UpdateTargetPosition(glm::vec3 target) {
    Target = target;
    UpdateCameraVectors();
}

float Camera::AspectRatio() {
    return static_cast<float>(viewport.width) / static_cast<float>(viewport.height);
}

glm::mat4 Camera::View() {
    // 等價於 gluLookAt()
    // glm::mat4 view = glm::lookAt(Position, Position + Front, WorldUp);

    // Calculate by yourself (Notice the zaxis faces to positive z)
    glm::vec3 zaxis = glm::normalize(-Front);
    glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(WorldUp), zaxis));
    glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));

    // 記得 OpenGL 是 Column-Major
    glm::mat4 translation = glm::mat4(1.0f);
    translation[3][0] = -Position.x;
    translation[3][1] = -Position.y;
    translation[3][2] = -Position.z;

    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = xaxis.x;
    rotation[1][0] = xaxis.y;
    rotation[2][0] = xaxis.z;
    rotation[0][1] = yaxis.x;
    rotation[1][1] = yaxis.y;
    rotation[2][1] = yaxis.z;
    rotation[0][2] = zaxis.x;
    rotation[1][2] = zaxis.y;
    rotation[2][2] = zaxis.z;

    glm::mat4 view = rotation * translation;

    return view;
}

void Camera::ProcessKeyboard() {
    // SDL2 鍵盤控制移動建議使用這個 SDL_GetKeyboardState()，才不會覺得卡卡頓頓的
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_Q]) {
        Roll -= 1.0;
    }
    if (state[SDL_SCANCODE_E]) {
        Roll += 1.0;
    }

//    // 一般 Yaw 跟 Pitch 都是用滑鼠控制即可，除非你真的很喜歡鍵盤控制...
//    if (state[SDL_SCANCODE_Y]) {
//        Yaw -= 1.0;
//    }
//    if (state[SDL_SCANCODE_U]) {
//        Yaw += 1.0;
//    }
//
//    if (state[SDL_SCANCODE_H]) {
//        Pitch -= 1.0;
//    }
//    if (state[SDL_SCANCODE_J]) {
//        Pitch += 1.0;
//    }

    if (state[SDL_SCANCODE_W]) {
        Acceleration += Front * MoveSpeed;
    }
    if (state[SDL_SCANCODE_S]) {
        Acceleration -= Front * MoveSpeed;
    }
    if (state[SDL_SCANCODE_D]) {
        Acceleration += Right * MoveSpeed;
    }
    if (state[SDL_SCANCODE_A]) {
        Acceleration -= Right * MoveSpeed;
    }
    if (state[SDL_SCANCODE_SPACE]) {
        Acceleration.y += MoveSpeed * 2.0f;
    }
    if (state[SDL_SCANCODE_LSHIFT]) {
        Acceleration.y -= MoveSpeed * 2.0f;
    }
}

void Camera::ProcessMouseMovement(bool constrain) {
    // 當 SDL_SetRelativeMouseMode() 為 TRUE 時，鼠標會不見
    // 並且使用的是【相對位置】，一般來說就是與上一幀滑鼠位置的相對位置
    // 就不用還要自己去計算，會方便許多！
    if (!MouseControl) {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        return;
    }

    int xoffset, yoffset;
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_GetRelativeMouseState(&xoffset, &yoffset);

    Yaw += xoffset * MouseSensitivity;
    Pitch += -yoffset * MouseSensitivity;

    if (Yaw >= 360.0f) {
        Yaw = 0.0f;
    }
    if (Yaw <= -360.0f) {
        Yaw = 0.0f;
    }

    if (Roll >= 360.0f) {
        Roll = 0.0f;
    }
    if (Roll <= -360.0f) {
        Roll = 0.0f;
    }

    // 限制 Pitch 不能為 ±90°，不然會產生萬向鎖問題（Yaw 跟 Pitch 合併在一起，此時 Yaw 跟 Roll 會是一樣的作用）。
    if (constrain) {
        if (this->Pitch > 89.0f) {
            this->Pitch = 89.0f;
        }
        if (this->Pitch < -89.0f) {
            this->Pitch = -89.0f;
        }
    }

    // 更新攝影機三軸座標
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    if (Zoom >= 1.0f && Zoom <= 90.0f) {
        Zoom -= yoffset;
    }
    if (Zoom < 1.0f) {
        Zoom = 1.0f;
    }
    if (Zoom > 90.0f) {
        Zoom = 90.0f;
    }
}

void Camera::ToggleMouseControl() {
    MouseControl = !MouseControl;
}

void Camera::Update(float dt) {
    UpdateCameraVectors();

    if (!FollowTarget) {
        Velocity += Acceleration * dt;
        Position += Velocity * dt;

        Acceleration = glm::vec3(0.0f);
        Velocity *= 0.95f;
    }
}

glm::mat4 Camera::Projection() {
    UpdateProjectionParameters();
    if (IsPerspective) {
        return Perspective();
    } else {
        return Orthogonal();
    }
}

glm::mat4 Camera::Orthogonal() {
    glm::mat4 proj = glm::ortho(frustum.left, frustum.right, frustum.bottom, frustum.top, frustum.near, frustum.far);



    return proj;
}

glm::mat4 Camera::Perspective() {
    glm::mat4 proj = glm::mat4(1.0f);
    // 等價於 glm::perspective()
    proj = glm::perspective(glm::radians(Zoom), AspectRatio(), frustum.near, frustum.far);

    proj[0][0] = 1 / (glm::tan(glm::radians(Zoom) / 2) * AspectRatio());
    proj[1][0] = 0;
    proj[2][0] = 0;
    proj[3][0] = 0;

    proj[0][1] = 0;
    proj[1][1] = 1 / (glm::tan(glm::radians(Zoom) / 2));
    proj[2][1] = 0;
    proj[3][1] = 0;

    proj[0][2] = 0;
    proj[1][2] = 0;
    proj[2][2] = -(frustum.far + frustum.near) / (frustum.far - frustum.near);
    proj[3][2] = (-2 * frustum.far * frustum.near) / (frustum.far - frustum.near);

    proj[0][3] = 0;
    proj[1][3] = 0;
    proj[2][3] = -1;
    proj[3][3] = 0;

    return proj;
}

void Camera::UpdateProjectionParameters() {
    // 計算 ViewVolume 使用
    if (IsPerspective) {
        frustum.top = tan(glm::radians(Zoom / 2.0f)) * frustum.near;
        frustum.right = AspectRatio() * frustum.top;
        frustum.bottom = -frustum.top;
        frustum.left = -frustum.right;
    } else {
        frustum.top = tan(glm::radians(Zoom / 2.0f)) * frustum.near * 1000.0f;
        frustum.right = AspectRatio() * frustum.top;
        frustum.bottom = -frustum.top;
        frustum.left = -frustum.right;
    }
}