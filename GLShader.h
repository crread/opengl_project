
#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <cstdlib>
#include <cstring>

GLuint LoadShader(const char* filename, GLenum type)
{
    GLuint shader = glCreateShader(type);

    if (shader != 0)
    {
        // toujours ouvrir en mode binaire (evite les conversions)

        FILE* file = fopen(filename, "rb");

        fseek(file, 0, SEEK_END);
        int length = (int)ftell(file);

        rewind(file);

        char* buffer = (char*)malloc(length + 1);

        fread(buffer, length, 1, file);
        buffer[length] = 0;

        glShaderSource(shader, 1, (const char**)&buffer, nullptr);
        glCompileShader(type);

        GLint compile_status = GL_TRUE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
        if (compile_status != GL_TRUE)
        {
            int log_size = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);

            if (log_size > 0)
            {
                char* log = (char*)malloc(log_size + 1);
                glGetShaderInfoLog(shader, log_size, &log_size, log);
                log[log_size] = 0;
                printf("[compilation error] in file \"%s\" :\n %s\n", filename, log);
                free(log);
            }

        }

        free(buffer);
        fclose(file);
    }
    return shader;
}

GLuint CreateProgram(GLuint vertex_shader, GLuint fragment_shader)
{
    GLuint program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    
    glLinkProgram(program);

    // TODO check link result
 
    GLint link_status = GL_TRUE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE)
    {
        int log_size = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
        if (log_size > 0)
        {
            char* log = (char*)malloc(log_size + 1);
            glGetProgramInfoLog(program, log_size, &log_size, log);
            log[log_size] = 0;
            printf("[link error] \"%s\" :\n %s\n", log);
            free(log);
        }
    }

    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);

    return program;
}

GLuint CreateShaderProgram(const char* vertex_filename, const char* fragment_filename)
{
    GLuint vertex_shader = LoadShader(vertex_filename, GL_VERTEX_SHADER);
    GLuint fragment_shader = LoadShader(fragment_filename, GL_FRAGMENT_SHADER);
    GLuint program = CreateProgram(vertex_shader, fragment_shader);
    return program;
}

void DestroyProgram(GLuint program)
{
    glDeleteProgram(program);
}

#endif // GL_SHADER_H
