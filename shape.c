#include <GL/glew.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static unsigned int CompileShader(unsigned int type, const char *source) {
  unsigned int id = glCreateShader(type);
  glShaderSource(id, 1, &source, NULL);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char message[length];
    glGetShaderInfoLog(id, length, &length, message);
    errx(1, "%s", message);
  }

  return id;
}

static unsigned int CreateShader(const char *vertexShader,
                                 const char *fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

void draw_triangle(float points[]) {
  //glewExperimental = GL_TRUE;

  GLenum error = glewInit();
  if (error != GLEW_OK) {
    errx(1, "Unable to init GLEW");
  }

  //printf("%s\n", glGetString(GL_VERSION));

  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  unsigned int indexes[] = {
          0,1,4,
          0,4,9,
          9,4,5,
          4,8,5,
          4,1,8,
          8,1,10,
          8,10,3,
          5,8,3,
          5,3,2,
          2,3,7,
          7,3,10,
          7,10,6,
          7,6,11,
          11,6,0,
          0,6,1,
          6,10,1,
          9,11,0,
          9,2,11,
          9,5,2,
          7,11,2
  };

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  unsigned int vb;
  glGenBuffers(1, &vb);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(float), points,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),(void *)(3 * sizeof(float)));

  unsigned int ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 60 * sizeof(unsigned int), indexes,
               GL_STATIC_DRAW);

  const char *vertex_shader = "#version 450 core\n"
                              "layout (location = 0) in vec4 position;"
                              "layout (location = 1) in vec3 aColor;"
                              "out vec3 outcolor;"
                              "void main() {"
                              "   gl_Position = position;"
                              "   outcolor = aColor;"
                              "}";

  const char *fragment_shader = "#version 450 core\n"
                                "out vec4 frag_color;"
                                "in vec3 outcolor;"
                                "void main() {"
                                "  frag_color = vec4(outcolor, 0.0f);"
                                "}";

  unsigned int shader_programme = CreateShader(vertex_shader, fragment_shader);
  glUseProgram(shader_programme);

  //int location = glGetUniformLocation(shader_programme, "u_Color");
  //glUniform4f(location, 0.1f, 0.3f, 0.8f, 1.0f);

  glClearColor(0.0, 0.5, 0.3, 0.5);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, NULL);
}
