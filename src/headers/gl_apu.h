#ifndef GL_APU_H
#define GL_APU_H

#include <qopengl.h>
#include <QOpenGLFunctions_4_3_Core>

struct Point { GLfloat x, y; };
struct Rect  { int w, h; };

GLuint generateRectangle (QOpenGLFunctions_4_3_Core *f, Point p1, Point p2, Point p3, Point p4);
  
#endif //GL_APU_H
