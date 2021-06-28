//
// Created by berthelot Mickael on 08/05/2021.
//

#ifndef OPENGL_PROJECT_QUATERNION_H
#define OPENGL_PROJECT_QUATERNION_H

#include <cmath>
#include <vector>

#include "./glm/glm.hpp"
#include "./glm/vec3.hpp"
#include "./glm/vec4.hpp"

typedef struct Quaternion {
    Quaternion(const std::vector<float> &RotationAxis, const float &RotationAngle) {
        x = RotationAxis[0] * std::sin(RotationAngle / 2);
        y = RotationAxis[1] * std::sin(RotationAngle / 2);
        z = RotationAxis[2] * std::sin(RotationAngle / 2);
        w = std::cos(RotationAngle / 2);
    };

    void normalise();
    Quaternion getConjugate();
    void addition(const Quaternion *quat);
    glm::mat4 convertToMatrix();
    float produitsqualaire(const glm::mat4 *mat);
    Quaternion MatrixtoQuaternion(const glm::mat4 *src);

    float x;
    float y;
    float z;
    float w;

} Quaternion;


#endif //OPENGL_PROJECT_QUATERNION_H
