#include <new_obj.h>
#include <script.h>

#include <QImage>

#include <editorController.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

void doProjection(QOpenGLFunctions_4_3_Core *f, GLfloat window_w, GLfloat window_h, GLuint shader)
{
  //Create orho-projection from window dimensions.
  glm::mat4 projection = glm::ortho(0.0f, window_w, window_h, 0.0f, -1.0f, 1.0f);

  // qDebug() << "Making an ortho projection with dimensions " << window_w << window_h ;

  //Move ortho projection to 2dshader
  f->glUseProgram(shader);
  f->glUniform1i(f->glGetUniformLocation(shader, "image"), 0);
  f->glUniform1i(f->glGetUniformLocation(shader, "subTile"), 1);

  f->glUniformMatrix4fv(f->glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
  f->glUseProgram(0);

}

GLuint getVAO(QOpenGLFunctions_4_3_Core *f, GLuint shader)
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

std::string getScript(std::string id) {
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
    printf("Loading fragment shader (%s) failed: %s\n", fr_c_smells.c_str(), infoLog);
  }

  GLuint p = CreateShaderProgram(f, vertexShader, fragmentShader);
  
  f->glDeleteShader(vertexShader);
  f->glDeleteShader(fragmentShader);

  return p;
}

GLuint obj::load_texture(QOpenGLFunctions_4_3_Core *f, const char *filename, int *text_w, int *text_h) {

  qi_copy.load(QString(filename));
  *text_w = qi_copy.width();
  *text_h = qi_copy.height();
  
  GLuint texture;
  f->glGenTextures(1, &texture);
  f->glBindTexture(GL_TEXTURE_2D, texture);
  
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, qi_copy.width(), qi_copy.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, qi_copy.bits());
  f->glGenerateMipmap(GL_TEXTURE_2D);
  f->glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}

void obj::prepare(QOpenGLFunctions_4_3_Core *fns, GLfloat parentw, GLfloat parenth, editorController *ec)
{
  shader = createShader(fns, ec);  
  VAO = getVAO(fns, shader);
  doProjection(fns, parentw, parenth, shader);
  qDebug() << "Created VAO " << VAO;
  position = glm::vec2(0.0f, 0.0f);
  qDebug()<<"Text size: " << text_w << " * " << text_h;
  size = glm::vec2(static_cast<float>(text_w), static_cast<float>(text_h));
  rotate = 0.0f;
  color = glm::vec3(1.0f, 1.0f, 1.0f);
  opacity = 255;
}
  
void obj::prepare(QOpenGLFunctions_4_3_Core *fns, Renderer *r, editorController *ec)
{
  shader = createShader(fns, ec);
  VAO = getVAO(fns, shader);
  doProjection(fns, r->width(), r->height(), shader);
  position = glm::vec2(0.0f, 0.0f);
  qDebug()<<"Text size: " << text_w << " * " << text_h;
  size = glm::vec2(static_cast<float>(text_w), static_cast<float>(text_h));
  rotate = 0.0f;
  color = glm::vec3(1.0f, 1.0f, 1.0f);
  opacity = 255;
}

obj::obj(Renderer *r, QOpenGLFunctions_4_3_Core *f, QImage img): parent(r)
{
  auto ec = editorController::instance;
  text_w = img.width();
  text_h = img.height();
  prepare(f, r->width(), r->height(), ec);

  qi_copy = img;

  f->glGenTextures(1, &texture);
  f->glBindTexture(GL_TEXTURE_2D, texture);
  
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, qi_copy.width(), qi_copy.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, qi_copy.bits());
  f->glGenerateMipmap(GL_TEXTURE_2D);
  f->glBindTexture(GL_TEXTURE_2D, 0);
}

obj::obj(Renderer *r, const char *texture_path,  bool skipTexture): parent(r)
{
  auto ec = editorController::instance;
  auto fns = r->getGlFns();

  texture = !skipTexture? load_texture(fns, texture_path, &text_w, &text_h): 0;
  if(skipTexture)
    qDebug() << "skipTexture is false. Object might not render";
  prepare(fns, r, ec);

  r->freeCtx();
}

obj::~obj()
{

}

void obj::render(Renderer *parent) {
  render();
}

void obj::render(QOpenGLFunctions_4_3_Core *f)
{
  render();
}
void obj::render()
{
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
  f->glUseProgram(shader);
  // qDebug() << "Position " << position.x << ", " << position.y;
  glm::mat4 model;
  model = glm::translate(model, glm::vec3(position, 0.0f));

  //Move pivot point to center
  // qDebug() << "Size: " << size.x << " * " << size.y;
  model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));

  //Rotate if needed
  // qDebug() << "Rotate: " << rotate;
  model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));

  //Move object
  model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

  //Scale object
  model = glm::scale(model, glm::vec3(size, 1.0f));

  f->glUniformMatrix4fv(f->glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
  f->glUniform3f(f->glGetUniformLocation(shader, "spriteColor"), color.x, color.y, color.z);

  auto op_loc = f->glGetUniformLocation(shader, "opacity");

  if(op_loc < 0) {
    qDebug() << "No opacity location found";
    throw "";
  }
  // qDebug() << "Opacity" << opacity;
  float op = (float)opacity / 255.0f;

  f->glUniform4f(op_loc, op, op, op, op);
  

  if(subObj) {
    f->glActiveTexture(GL_TEXTURE1);
    f->glBindTexture(GL_TEXTURE_2D, subObj->texture);
    f->glUniform1i(f->glGetUniformLocation(shader, "subTileBound"), 1);
  }
  
  f->glActiveTexture(GL_TEXTURE0);
  f->glBindTexture(GL_TEXTURE_2D, texture);

  f->glBindVertexArray(VAO);
  f->glDrawArrays(GL_TRIANGLES, 0, 6);
  f->glBindVertexArray(0);

  if(subObj) {
    f->glUniform1i(f->glGetUniformLocation(shader, "subTileBound"), 0);
  }

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

obj* obj::make(Renderer *parent, QImage img, std::string id) {

  for(Renderer *r: editorController::instance->renderers) {
    auto f = r->getGlFns();

    qDebug()<<"Preparing obj " << id.c_str() << " for parent" << r->name.c_str();
    
    obj *o = new obj(r, f, img);
    o->id = id;
    o->qi_copied = true;
    r->setOwnObject(id, o);
    r->freeCtx();
  }

  // These are always producing objs so this is a safe cast
  if(parent)
    return static_cast<obj*>(parent->getOwnObject(id));
  else return nullptr;
}

std::string obj::make(QImage img, std::string id) {
  make(nullptr, img, id);
  return id;  
}

obj* obj::make(Renderer *rr, const char *texture_path, bool skipTexture, std::string id) {
  for(Renderer *r: editorController::instance->renderers) {
    obj *o = new obj(r, texture_path, skipTexture);
    o->id = id;
    r->setOwnObject(id, o);
    o->text_path = texture_path;
    o->skipTexture = skipTexture;
  }

  return rr? static_cast<obj*>(rr->getOwnObject(id)): static_cast<obj*>(editorController::instance->renderers.at(0)->getOwnObject(id));
}

std::string obj::getRenderId() const {
  return id;
}

Renderable* obj::copyRenderable() {
  if(qi_copied) 
    return make(parent, qi_copy);
  else return make(parent, text_path, skipTexture);
}
