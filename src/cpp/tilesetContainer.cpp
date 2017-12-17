#include <tilesetContainer.h>

#include <QDebug>

// https://gist.github.com/nkint/9089157#file-gistfile1-txt-L27
/*
  Askartele paskartele viritys joka lataa png:n levyltä QImageksi
  Jakaa sen 50 x 50 kokoisiksi QImageiksi QImage::copy()lläoo
  Tekee jokaisesta näistä opengltextuurin
  ja tekee tileseteistä 2d-taulukoita täynnä opengl-textuuri-kahvoja
*/

GLuint toTexture(QOpenGLFunctions_4_3_Core *f, QImage &img)
{
  int text_w = 50,
      text_h = 50;
  
  GLuint texture;
  f->glGenTextures(1, &texture);
  f->glBindTexture(GL_TEXTURE_2D, texture);
  
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_BORDER);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_BORDER);

  f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, text_w, text_h, 0, GL_RGB, GL_UNSIGNED_BYTE, img.constBits());
  f->glGenerateMipmap(GL_TEXTURE_2D);
  f->glBindTexture(GL_TEXTURE_2D, 0);

  assert(text_w == 50);
  assert(text_h == 50);

  return texture;
}

// Making this return a 2d array should be a low hanging fruit when optimizing
// I'm skipping it now 'cause I have no idea how this insane language did
// dynamically sized 2D arrays
QVector<QVector<GLuint>> load_texture_splitted(QOpenGLFunctions_4_3_Core *f, const char *filename)
{
  QImage root_img;
  root_img.load(QString(filename));

  int array_w = root_img.width() / 50;
  int array_h = root_img.height() / 50;

  QVector<QVector<GLuint>> results(array_w);
  
  for(int x = 0; x < array_w; x++) {
    QVector<GLuint> row (array_h);
    
    for(int y = 0; y < array_h; y++) {
      QImage copy = root_img.copy(x * 50,
				  y * 50, 50, 50);
      row.push_back(toTexture(f, copy));
    }
    results.push_back(row);
  }
  
  return results;
}

tilesetContainer::tilesetContainer(Renderer *r, const char *tilesetPath): Tileset(), obj(r, tilesetPath, true) {
  auto fns = r->getGlFns();
  textures = load_texture_splitted(fns, tilesetPath);
  r->freeCtx();
}

void tilesetContainer::render(QOpenGLFunctions_4_3_Core *f) {
  qDebug() << "Rendering a tileset";
}
