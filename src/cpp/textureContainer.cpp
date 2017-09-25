#include <textureContainer.h>
#include <QDebug>

textureContainer::textureContainer(QString path, QOpenGLFunctions_4_3_Core *f) {
  f->glGenTextures(1, &texture_handle);
  f->glBindTexture(GL_TEXTURE_2D, texture_handle);

  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned char *img = SOIL_load_image(path.toStdString().c_str(), &texture_width, &texture_height, 0, SOIL_LOAD_AUTO);
  if(img) {
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    f->glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(img);
    f->glBindTexture(GL_TEXTURE_2D, 0);
    qDebug() << "Loading texture from " << path << " succeeded";
  }
  else qDebug() << "Loading texture from " << path << " failed";
}

void textureContainer::parent(root *p) {
  this->p = p;
}

root* textureContainer::parent() {
  return p;
}
