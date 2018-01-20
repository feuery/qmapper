#include <new_obj.h>
#include <script.h>

#include <QImage>

#include <editorController.h>

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
  Script *obj = toScript(editorController::instance->document.fetchRegister("Script", id));
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
  f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *text_w, *text_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
  f->glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(img);
  f->glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}

void obj::prepare(QOpenGLFunctions_4_3_Core *fns, GLfloat parentw, GLfloat parenth, editorController *ec)
{
  shader = createShader(fns, ec);
  VAO = getVAO(fns, parentw, parenth, shader);
  position = glm::vec2(0.0f, 0.0f);
  size = glm::vec2(static_cast<float>(text_w), static_cast<float>(text_h));
  rotate = 0.0f;
  color = glm::vec3(1.0f, 1.0f, 1.0f);
  opacity = 255;
}
  
void obj::prepare(QOpenGLFunctions_4_3_Core *fns, Renderer *r, editorController *ec)
{
  shader = createShader(fns, ec);
  VAO = getVAO(fns, r->width(), r->height(), shader);
  position = glm::vec2(0.0f, 0.0f);
  size = glm::vec2(static_cast<float>(text_w), static_cast<float>(text_h));
  rotate = 0.0f;
  color = glm::vec3(1.0f, 1.0f, 1.0f);
  opacity = 255;
}

const char* formatToStr(QImage::Format format)
{
  switch(format) {
  case QImage::Format_Invalid: return "Format_Invalid";
  case QImage::Format_Mono: return "Format_Mono";
  case QImage::Format_MonoLSB: return "Format_MonoLSB";
  case QImage::Format_Indexed8: return "Format_Indexed8";
  case QImage::Format_RGB32: return "Format_RGB32";
  case QImage::Format_ARGB32: return "Format_ARGB32";
  case QImage::Format_ARGB32_Premultiplied: return "Format_ARGB32_Premultiplied";
  case QImage::Format_RGB16: return "Format_RGB16";
  case QImage::Format_ARGB8565_Premultiplied: return "Format_ARGB8565_Premultiplied";
  case QImage::Format_RGB666: return "Format_RGB666";
  case QImage::Format_ARGB6666_Premultiplied: return "Format_ARGB6666_Premultiplied";
  case QImage::Format_RGB555: return "Format_RGB555";
  case QImage::Format_ARGB8555_Premultiplied: return "Format_ARGB8555_Premultiplied";
  case QImage::Format_RGB888: return "Format_RGB888";
  case QImage::Format_RGB444: return "Format_RGB444";
  case QImage::Format_ARGB4444_Premultiplied: return "Format_ARGB4444_Premultiplied";
  case QImage::Format_RGBX8888: return "Format_RGBX8888";
  case QImage::Format_RGBA8888: return "Format_RGBA8888";
  case QImage::Format_RGBA8888_Premultiplied: return "Format_RGBA8888_Premultiplied";
  case QImage::Format_BGR30: return "Format_BGR30";
  case QImage::Format_A2BGR30_Premultiplied: return "Format_A2BGR30_Premultiplied";
  case QImage::Format_RGB30: return "Format_RGB30";
  case QImage::Format_A2RGB30_Premultiplied: return "Format_A2RGB30_Premultiplied";
  case QImage::Format_Alpha8: return "Format_Alpha8";
  case QImage::Format_Grayscale8: return "Format_Grayscale8";
  }

  return "Not recognized";
}

obj::obj(Renderer *r, QOpenGLFunctions_4_3_Core *f, QImage img)
{
  auto ec = editorController::instance;
  text_w = text_h = 50.0f;
  prepare(f, r->width(), r->height(), ec);

  copy = img;

  qDebug() << "Format is " << formatToStr(img.format());

  f->glGenTextures(1, &texture);
  f->glBindTexture(GL_TEXTURE_2D, texture);
  
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_BORDER);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_BORDER);

  qDebug() << "Texture dimensions: " << text_w << text_h;
  
  f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, copy.width(), copy.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, copy.bits());
  f->glGenerateMipmap(GL_TEXTURE_2D);
  f->glBindTexture(GL_TEXTURE_2D, 0);

  qDebug() << "Texture generated";
}

obj::obj(Renderer *r, const char *texture_path,  bool skipTexture)
{
  auto ec = editorController::instance;
  auto fns = r->getGlFns();

  qDebug()<<"Preparing obj for parent" << r->name.c_str();
  
  prepare(fns, r, ec);
  
  texture = !skipTexture? load_texture(fns, texture_path, &text_w, &text_h): 0;

  r->freeCtx();
}

obj::~obj()
{

}

void obj::render(Renderer *parent) {
  render(parent->getGlFns());
  parent->freeCtx();
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

  auto op_loc = f->glGetUniformLocation(shader, "opacity");

  if(op_loc < 0) qDebug() << "No opacity location found";

  float op = (float)opacity / 255.0f;

  f->glUniform4f(op_loc, op, op, op, op);
  
  f->glActiveTexture(GL_TEXTURE0);
  f->glBindTexture(GL_TEXTURE_2D, texture);

  f->glBindVertexArray(VAO);
  f->glDrawArrays(GL_TRIANGLES, 0, 6);
  f->glBindVertexArray(0);

  GLenum err (f->glGetError());
 
  while(err!=GL_NO_ERROR) {
    std::string error;
 
    switch(err) {
    case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
    case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
    case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
    case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
    }
 
    qDebug() << "GL_" << error.c_str();
    err=f->glGetError();
  }
}

void obj::reload_shaders(QOpenGLFunctions_4_3_Core *f, flyweight<std::string> vertexId, flyweight<std::string> fragmentId)
{
  editorController *ec = editorController::instance;
  shader = createShader(f, toScript(ec->document.fetchRegister("Script", vertexId))->getContents().c_str(),
			toScript(ec->document.fetchRegister("Script", fragmentId))->getContents().c_str());
}

bool obj::load_new_texture(const char *path, Renderer *r)
{
  auto fns = r->getGlFns();
  texture = load_texture(fns, path, &text_w, &text_h); 
  r->freeCtx();
  return true;
}

obj* obj::make(Renderer *parent, QImage img) {
  int id = rand();
  for(Renderer *r: editorController::instance->renderers) {
    auto f = r->getGlFns();

    qDebug()<<"Preparing obj " << id << " for parent" << r->name.c_str();
    
    obj *o = new obj(parent, f, img);
    o->id = id;
    r->owned_objects[id] = o;
    r->freeCtx();
  }

  // These are always producing objs so this is a safe cast
  return static_cast<obj*>(parent->owned_objects[id]);
}

obj* obj::make(Renderer *rr, const char *texture_path, bool skipTexture) {
  int id = rand();
  for(Renderer *r: editorController::instance->renderers) {
    obj *o = new obj(r, texture_path, skipTexture);
    o->id = id;
    r->owned_objects[id] = o;
  }

  return static_cast<obj*>(rr->owned_objects[id]);
}

int obj::getRenderId() {
  return id;
}
