#ifndef TEXTURECONTAINER_H
#define TEXTURECONTAINER_H

#include <texture.h>
#include <QString>

#include <SOIL/SOIL.h>
#include <qopengl.h>

class textureContainer: public Texture{
 public:
  textureContainer(QString path, QOpenGLFunctions_4_3_Core *f);

  void parent (root* p);
  root* parent ();

private:
  root *p;
  
};

#endif //TEXTURECONTAINER_H
