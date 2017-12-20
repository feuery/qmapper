#include <tilesetContainer.h>

#include <QDebug>

// https://gist.github.com/nkint/9089157#file-gistfile1-txt-L27
/*
  Askartele paskartele viritys joka lataa png:n levyltä QImageksi
  Jakaa sen 50 x 50 kokoisiksi QImageiksi QImage::copy()lläoo
  Tekee jokaisesta näistä opengltextuurin
  ja tekee tileseteistä 2d-taulukoita täynnä opengl-textuuri-kahvoja
*/


// Making this return a 2d array should be a low hanging fruit when optimizing
// I'm skipping it now 'cause I have no idea how this insane language did
// dynamically sized 2D arrays
void tilesetContainer::load_texture_splitted(QOpenGLFunctions_4_3_Core *f, const char *filename)
{
  QImage root_img;
  root_img.load(QString(filename));

  tiles_w = root_img.width() / 50;
  tiles_h = root_img.height() / 50;

  tiles = new obj**[tiles_w];

  // QVector<QVector<obj*>> results(array_w);
  
  for(int x = 0; x < tiles_w; x++) {
    tiles[x] = new obj*[tiles_h];
    
    for(int y = 0; y < tiles_h; y++) {
      QImage copy = root_img.copy(x * 50,
				  y * 50, 50, 50);
      obj *o = new obj(f, copy);
      o->position = glm::vec2(x * 50.0f, y * 50.0f);
      qDebug() << "Loading tiletexture at " << o;
      tiles[x][y] = o;
    }
  }
}

tilesetContainer::tilesetContainer(Renderer *r, const char *tilesetPath): Tileset(){
  auto fns = r->getGlFns();
  load_texture_splitted(fns, tilesetPath);
  r->freeCtx();
}

void tilesetContainer::render(QOpenGLFunctions_4_3_Core *f)
{
  for(int x = 0; x < tiles_w; x++) {
    for(int y = 0; y < tiles_h; y++) {
      obj* tile = tiles[x][y];
      if(tile) {
	tile->render(f);
      }
      else qDebug() << "Tile on " << tile;
      // else
      // 	puts("Tile ei ole validi :(");
    }
  }
}
