#include <GL/glew.h>
#include <err.h>
#include <stdio.h>

void draw_triangle(float points[]) {
  glewExperimental = GL_TRUE;
  GLenum error = glewInit();
  if (error != GLEW_OK) {
    errx(1, "Unable to init GLEW");
  }
  const char* vertex_shader =
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;"
            "layout (location = 1) in vec3 aColor;"
            "out vec3 outcolor;"
            "void main() {"
            "   gl_Position = vec4(aPos, 1.0);"
            "   outcolor = aColor;"
            "}";

    const char* fragment_shader =
            "#version 330 core\n"
            "out vec4 frag_colour;"
            "in vec3 outcolor;"
            "void main() {"
            "  frag_colour = vec4(outcolor, 1.0f);"
            "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    int sucess ;
    //char infolog[512];
    glGetShaderiv(vs,GL_COMPILE_STATUS,&sucess);
    if (!sucess)
    {
        printf("Shader didn't compile \n");
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs,GL_COMPILE_STATUS,&sucess);
    if (!sucess)
    {
        printf("Fragment didn't compile \n");
    }

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);
    glGetProgramiv(shader_programme,GL_LINK_STATUS,&sucess);
    if (!sucess)
    {
        printf("Shader program didn't compile \n");
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    GLuint vbo ,vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 1440/5/*sizeof(points)*/, points, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shader_programme);

    glClearColor (0.3, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 12/*60*/);
}