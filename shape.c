#include <GL/glew.h>
#include <err.h>
#include <stdio.h>

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
    printf("Failed to compile the shader : %s", message);
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
  glewExperimental = GL_TRUE;
  GLenum error = glewInit();
  if (error != GLEW_OK) {
    errx(1, "Unable to init GLEW");
  }
  const char *vertex_shader = "#version 450 core\n"
                              "layout (location = 0) in vec3 aPos;"
                              "layout (location = 1) in vec3 aColor;"
                              "out vec3 outcolor;"
                              "void main() {"
                              "   gl_Position = vec4(aPos, 1.0);"
                              "   outcolor = aColor;"
                              "}";

  const char *fragment_shader = "#version 450 core\n"
                                "out vec4 frag_colour;"
                                "in vec3 outcolor;"
                                "void main() {"
                                "  frag_colour = vec4(outcolor, 1.0f);"
                                "}";

  unsigned int shader_programme = CreateShader(vertex_shader, fragment_shader);
  glUseProgram(shader_programme);

  GLuint vbo, vao;
  glGenBuffers(1, &vbo);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 1440 / 5 /*sizeof(points)*/, points,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glClearColor(0.3, 0.3, 0.3, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 12 /*60*/);
}
