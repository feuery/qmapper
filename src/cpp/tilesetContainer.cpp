#include <tilesetContainer.h>

#include <QDebug>
#include <gl_apu.h>

void tilesetContainer::render (QOpenGLFunctions_4_3_Core *f)
{
  qDebug() << "Rendering tileset";
}

bool tilesetContainer::load_texture(QString& path, Renderer* r)
{
  texture = r->load_texture(path.toStdString().c_str(), &texture_width_px, &texture_height_px);
  return texture > 0;
}
