#include <new_obj.h>
#include <script.h>

GLuint getVAO(QOpenGLFunctions_4_3_Core *f, GLfloat window_w, GLfloat window_h, GLuint shader)
{
  GLuint VAO, VBO;
  //Create vertexes to be used in 2d rendering.
  GLfloat vertices[] = {
    // Pos      // Tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
  };

  //Init VAO and VBO
  f->glGenVertexArrays(1, &VAO);
  f->glGenBuffers(1, &VBO);

  f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  f->glBindVertexArray(VAO);
  f->glEnableVertexAttribArray(0);
  f->glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
  f->glBindBuffer(GL_ARRAY_BUFFER, 0);
  f->glBindVertexArray(0);

  //Create orho-projection from window dimensions.
  glm::mat4 projection = glm::ortho(0.0f, window_w, window_h, 0.0f, -1.0f, 1.0f);

  //Move ortho projection to 2dshader
  f->glUseProgram(shader);
  f->glUniform1i(f->glGetUniformLocation(shader, "image"), 0);

  f->glUniformMatrix4fv(f->glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
  return VAO;
}

GLuint CreateShaderProgram(QOpenGLFunctions_4_3_Core *f, GLuint vertex_shader, GLuint fragment_shader) {

  GLuint shaderProgram = f->glCreateProgram();
  f->glAttachShader(shaderProgram, vertex_shader);
  f->glAttachShader(shaderProgram, fragment_shader);
  f->glLinkProgram(shaderProgram);
  GLint success;
  GLchar infolog[512];
  f->glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if(!success) {
    f->glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
    printf("Linking shader program failed: %s\n", infolog);
    return -1;
  }

  return shaderProgram;
}

GLuint createShader(QOpenGLFunctions_4_3_Core *f, const char *v_c_smells, const char *fr_c_smells) {
  const char* const* v = &v_c_smells;
  const char* const* fr = &fr_c_smells;
  GLuint vertexShader = f->glCreateShader(GL_VERTEX_SHADER);
  f->glShaderSource(vertexShader, 1, v, NULL);
  f->glCompileShader(vertexShader);

  GLint success;
  GLchar infoLog[512];
  f->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if(!success) {
    f->glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("Loading vertex shader failed: %s\n", infoLog);
  }


  GLuint fragmentShader = f->glCreateShader(GL_FRAGMENT_SHADER);
  f->glShaderSource(fragmentShader, 1, v, NULL);
  f->glCompileShader(fragmentShader);

  f->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if(!success) {
    f->glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("Loading fragment shader (%s) failed: %s\n", fr, infoLog);
  }

  GLuint p = CreateShaderProgram(f, vertexShader, fragmentShader);
  
  f->glDeleteShader(vertexShader);
  f->glDeleteShader(fragmentShader);

  return p;
}

std::string getScript(flyweight<std::string> id) {
  Script *obj = toScript((*editorController::instance->document.registry)[id]);
  if(!obj) {
    puts("Obj is null");
    throw "";
  }
  else {
    return obj->getContents();
  }
}

GLuint createShader(QOpenGLFunctions_4_3_Core *f, editorController *ec)
{
  // fml and c's string handling
  auto v_c_smells = getScript(ec->indexOfStdVertexShader),
    fr_c_smells = getScript(ec->indexOfStdFragmentShader);

  const char *v_lol = v_c_smells.c_str();
  const char *f_lol = fr_c_smells.c_str();
  const char* const* v = &v_lol;
  const char* const* fr = &f_lol;

  GLuint vertexShader = f->glCreateShader(GL_VERTEX_SHADER);
  f->glShaderSource(vertexShader, 1, v, NULL);
  f->glCompileShader(vertexShader);

  GLint success;
  GLchar infoLog[512];
  f->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if(!success) {
    f->glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("Loading vertex shader failed: %s\n", infoLog);
  }


  GLuint fragmentShader = f->glCreateShader(GL_FRAGMENT_SHADER);
  f->glShaderSource(fragmentShader, 1, fr, NULL);
  f->glCompileShader(fragmentShader);

  f->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if(!success) {
    f->glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("Loading fragment shader (%s) failed: %s\n", fr_c_smells, infoLog);
  }

  GLuint p = CreateShaderProgram(f, vertexShader, fragmentShader);
  
  f->glDeleteShader(vertexShader);
  f->glDeleteShader(fragmentShader);

  return p;
}

GLuint load_texture(QOpenGLFunctions_4_3_Core *f, const char *filename, int *text_w, int *text_h) {
  GLuint texture;
  f->glGenTextures(1, &texture);
  f->glBindTexture(GL_TEXTURE_2D, texture);
  
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned char* img = SOIL_load_image(filename, text_w, text_h, 0, SOIL_LOAD_RGB);  
  f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *text_w, *text_h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
  f->glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(img);
  f->glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}

obj::obj(Renderer *r, editorController *ec, const char *texture_path)
{
  auto fns = r->getGlFns();
  shader = createShader(fns, ec);  
  VAO = getVAO(fns, r->width(), r->height(), shader);
  texture = load_texture(fns, texture_path, &text_w, &text_h);

  position = glm::vec2(0.0f, 0.0f);
  size = glm::vec2(static_cast<float>(text_w), static_cast<float>(text_h));
  rotate = 0.0f;
  color = glm::vec3(1.0f, 1.0f, 1.0f);

  r->freeCtx();
}

obj::~obj()
{

}

void obj::render(QOpenGLFunctions_4_3_Core *f)
{
  f->glUseProgram(shader);
  glm::mat4 model;
  model = glm::translate(model, glm::vec3(position, 0.0f));

  //Move pivot point to center
  model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));

  //Rotate if needed
  model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));

  //Move object
  model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

  //Scale object
  model = glm::scale(model, glm::vec3(size, 1.0f));

  f->glUniformMatrix4fv(f->glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
  f->glUniform3f(f->glGetUniformLocation(shader, "spriteColor"), color.x, color.y, color.z);
  f->glActiveTexture(GL_TEXTURE0);
  f->glBindTexture(GL_TEXTURE_2D, texture);

  f->glBindVertexArray(VAO);
  f->glDrawArrays(GL_TRIANGLES, 0, 6);
  f->glBindVertexArray(0);
}

void obj::reload_shaders(QOpenGLFunctions_4_3_Core *f, flyweight<std::string> vertexId, flyweight<std::string> fragmentId)
{
  editorController *ec = editorController::instance;
  shader = createShader(f, toScript((*ec->document.registry)[vertexId])->getContents().c_str(),
			toScript((*ec->document.registry)[fragmentId])->getContents().c_str());
}

bool obj::load_new_texture(const char *path, Renderer *r)
{
  auto fns = r->getGlFns();
  texture = load_texture(fns, path, &text_w, &text_h);
  r->freeCtx();
  return true;
}
