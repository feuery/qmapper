#ifndef TILESETCONTAINER_H
#define TILESETCONTAINER_H

#include <tileset.h>
#include <new_obj.h>

class tilesetContainer: public Tileset, public obj {
public:

  tilesetContainer(Renderer *r, const char *tilesetPath);
};

#endif //TILESETCONTAINER_H
