#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

#include "glew/include/glew.h"
#include "GLFW/include/glfw3.h"

#include <cstdio>
#include <cmath>
#include <iostream>
#include "./GLShader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "./stb-master/stb_image.h"

GLuint Dragon3DProgram = 0;

void Initialize(GLFWwindow* window)
{
    printf("Version OPENGL : %s\n", glGetString(GL_VERSION));
    printf("Vendor : %s\n", glGetString(GL_VENDOR));
    printf("Renderer : %s\n", glGetString(GL_RENDERER));
    printf("Version GLSL : %s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Erreur OpenGL, code %d\n", error);
    }

    Dragon3DProgram = CreateShaderProgram(
                            "./../Dragon3DLight.vs.glsl",
                            "./../Dragon3DLight.fs.glsl");

}

void Terminate()
{
    DestroyProgram(Dragon3DProgram);
}

int main()
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "Dragon", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLenum glewError = glewInit();
    if (glewError != GLEW_NO_ERROR)
    {
        glfwTerminate();
        return -1;
    }

    std::string objPath = "./../models/Cottage.obj";
    tinyobj::attrib_t attribs;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warm;
    std::string err;

//    bool ret = tinyobj::LoadObj(&attribs, &shapes, &materials, &warm, &err, "./../models/Cottage.obj", "", true, false);
    bool ret = tinyobj::LoadObj(&attribs, &shapes, &materials, &warm, &err, "./../models/suzanne.obj", nullptr, true, true);
    if (!warm.empty()) {
        std::cout<<warm<<std::endl;
    }

    if (!err.empty()) {
        std::cout<<err<<std::endl;
    }

    if (!ret) {
        exit(1);
    }

    std::vector<float> listData;

    int index = 0;

    for (auto & shape : shapes){

        int index_offset = 0;

        for (int j = 0; j < shape.mesh.num_face_vertices.size(); j++) {
            int fv = shape.mesh.num_face_vertices[j];

            for (int k = 0; k < fv; k++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + k];

                listData.push_back(attribs.vertices[3*idx.vertex_index+0]);
                listData.push_back(attribs.vertices[3*idx.vertex_index+1]);
                listData.push_back(attribs.vertices[3*idx.vertex_index+2]);

                if (!attribs.normals.empty()) {
//                    printf("normals");
                    listData.push_back(attribs.normals[3*idx.normal_index+0]);
                    listData.push_back(attribs.normals[3*idx.normal_index+1]);
                    listData.push_back(attribs.normals[3*idx.normal_index+2]);
                }

                if (!attribs.texcoords.empty()) {
//                    printf("text coords");
                    listData.push_back(attribs.texcoords[2*idx.texcoord_index+0]);
                    listData.push_back(attribs.texcoords[2*idx.texcoord_index+1]);
                }
            }
            index_offset += fv;
            index += fv;
        }
    }

    Initialize(window);

    int h, w, nrChannels;
    uint8_t *data = stbi_load("./../Textures/grass_grass_0070_03.jpg", &w, &h, &nrChannels, STBI_rgb_alpha);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        glViewport(0, 0, width, height);

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        static const int stride = sizeof(float) * 8;

        const GLint POSITION = glGetAttribLocation( Dragon3DProgram, "a_position");
        glEnableVertexAttribArray(POSITION);
        glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, &listData[0]);

        const GLint texAttrib = glGetAttribLocation(Dragon3DProgram,"a_texcoords");
        glEnableVertexAttribArray(texAttrib);
        glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, &listData[2]);

        glUseProgram(Dragon3DProgram);

        float time = glfwGetTime();
        const int timeLocation = glGetUniformLocation(Dragon3DProgram, "u_time");
        glUniform1f(timeLocation, time);

        float rotationMatrix[] = {
            cosf(time), 0.f, -sinf(time), 0.0f,
            0.0f, 1.0f, 0.0f, 0.f,
            sinf(time), 0.f, cosf(time), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        const int rotationLocation = glGetUniformLocation(
                    Dragon3DProgram,
                    "u_rotationMatrix"
        );

        glUniformMatrix4fv(rotationLocation,
                            1,
                            GL_TRUE,
                            rotationMatrix
                            );

        float translationMatrix[] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            cosf(time), 0.0f, -20.0f, 1.0f
        };

        const int translationLocation = glGetUniformLocation(
                    Dragon3DProgram,
                    "u_translationMatrix"
        );

        glUniformMatrix4fv(translationLocation,
                            1,
                            GL_FALSE,
                            translationMatrix
                            );

        // fov=45°, aspect-ratio=width/height, znear=0.1, zfar=1000.0
        float fov = 30.0f;
        float radianFov = fov * (float)(M_PI / 180.0);
        float aspect = (float)width / (float)height;
        float znear = 0.1f, zfar = 500.0f;
        float cot = 1.0f / tanf(radianFov / 2.0f);

        float projectionMatrix[] = {
            cot/aspect, 0.0f, 0.0f, 0.0f,
            0.0f, cot, 0.0f, 0.0f,
            0.0f, 0.0f, -(znear+zfar)/(zfar-znear), -1.0f,
            0.0f, 0.0f, -(2.0f*znear*zfar)/(zfar-znear), 0.0f
        };

        const int projectionLocation = glGetUniformLocation(
                    Dragon3DProgram,
                    "u_projectionMatrix"
        );

        glUniformMatrix4fv(projectionLocation,
                            1,
                            GL_FALSE,
                            projectionMatrix
                            );

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glDrawArrays(GL_TRIANGLES, listData[0], index);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    Terminate();
    glfwTerminate();
    return 0;
}
