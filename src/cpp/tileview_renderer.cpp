#include <tileview_renderer.h>

#include <editorController.h>
#include <guile_fn.h>

static GLuint createShader(QOpenGLFunctions_4_3_Core *f)
{
  editorController *ec = editorController::instance;
  static cl_object stdFrag = makefn("qmapper.root:root-contents-stdfragment");
  static cl_object stdVertex = makefn("qmapper.root:root-contents-stdvertex");
  static cl_object tileViewFrag = makefn("qmapper.root:root-contents-stdtileviewfragshader");

  std::string vertex_source = (ecl_string_to_string(cl_funcall(2, stdVertex, ec->document.getValue()))),
    fragmentSource (ecl_string_to_string(cl_funcall(2, tileViewFrag, ec->document.getValue())));

  const char* vertex_haisee = vertex_source.c_str();
  const char* const* v = &vertex_haisee;

  const char* fragment_haisee = fragmentSource.c_str();
  const char* const* frag = &fragment_haisee;
    

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
  f->glShaderSource(fragmentShader, 1, frag, NULL);
  f->glCompileShader(fragmentShader);

  f->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if(!success) {
    f->glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("Loading fragment shader (%s) failed: %s\n", frag, infoLog);
  }

  GLuint p = CreateShaderProgram(f, vertexShader, fragmentShader);
  
  f->glDeleteShader(vertexShader);
  f->glDeleteShader(fragmentShader);

  return p;  
}

tileview_renderer::tileview_renderer(QWidget *parent): Renderer(parent), o(nullptr)
{
  // size = glm::vec2(50.0f, 50.0f);
  // position = glm::vec2(0.0f, 0.0f);
  // printf("tileview::owned_objects.size() => %d\n", owned_objects.size());
}

void tileview_renderer::initializeGL()
{
    auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();

    if(!f) return;

    // shader = createShader(f);
    // VAO = getVAO(f, shader);
    // doProjection(f, 50.0f, 50.0f, shader);

    f->glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
    f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  
}

// GLuint tileview_renderer::getTexture(std::string id) {
//   auto res = getOwnObject(id);
//   if(!res) return -1;
//   obj *o = static_cast<obj*>(res);
//   return o->texture;
// }

void tileview_renderer::paintGL()
{
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
  // puts("paintGLing tileview");
  if(!f || !o ) { // puts("can't paintGL");
    return; }
  // puts("Really paintGLling");

  // texture = getTexture(tileid);
  
  o->position.x = 0;
  o->position.y = 0;
  assert(o->size.x == 50.0 && o->size.y == 50.0);
  o->render();

  // GLfloat rotate = 0.0f;

  // f->glClear(GL_COLOR_BUFFER_BIT);

  // f->glUseProgram(shader);
  // glm::mat4 model;
  // model = glm::translate(model, glm::vec3(position, 0.0f));

  // //Move pivot point to center
  // model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));

  // //Rotate if needed
  // model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));

  // //Move object
  // model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

  // //Scale object
  // model = glm::scale(model, glm::vec3(size, 1.0f));

  // f->glUniformMatrix4fv(f->glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
  // // f->glUniform3f(f->glGetUniformLocation(shader, "spriteColor"), 0.0f, 1.0f, 0.0f);

  // f->glActiveTexture(GL_TEXTURE0);
  // f->glBindTexture(GL_TEXTURE_2D, texture);

  // f->glBindVertexArray(VAO);
  // f->glDrawArrays(GL_TRIANGLES, 0, 6);

  // f->glBindVertexArray(0);
}

void tileview_renderer::setSelectedTile(obj *selectedTile)
{
  o = selectedTile;
}
