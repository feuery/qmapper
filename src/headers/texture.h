#ifndef TEXTURE_H
#define TEXTURE_H

#include <SOIL/SOIL.h>
#include <QtDebug>
#include <qopengl.h>

class Texture{
public:
  int w, h;
  GLuint texture;
  
  Texture(const char* filename)
  {
    qDebug() << "Initialized texture class with " << filename << ", please don't do that yet!";
  }

  ~Texture()
  {
    /* glDeleteTextures(1, &texture); */
    qDebug() << "Deleted texture class, please don't do that yet!";
  }

  // void render();
  
private:

};

#endif //TEXTURE_H
