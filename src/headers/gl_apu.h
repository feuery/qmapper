#ifndef GL_APU_H
#define GL_APU_H

#include <qopengl.h>
#include <QOpenGLFunctions_4_3_Core>
#include <QtDebug>

struct Point { GLfloat x, y; };
struct Rect  { int w, h; };
struct Point3D { GLfloat x, y, z; };

bool operator==(Point const& l, Point const& r);
bool operator!=(Point const& l, Point const& r);
bool operator==(Point3D const& l, Point3D const& r);
bool operator!=(Point3D const& l, Point3D const& r);

inline QDebug operator<<(QDebug g, Point3D const& e)
{
  return g<<"["<<e.x<<","<<e.y<<","<<e.z<<"]";
}

GLuint generateRectangle (QOpenGLFunctions_4_3_Core *f);

GLuint loadTexture(QOpenGLFunctions_4_3_Core *f, const char* filename, int *text_w, int *text_h);
  
#endif //GL_APU_H
