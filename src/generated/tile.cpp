#include <tile.h>
////// generated at 2017-10-15T15:50:50.200Z


void Tile::setX(int value) { 
X_field = value;
}
                                                        int Tile::getX() {
return X_field;
}
void Tile::setY(int value) { 
Y_field = value;
}
                                                        int Tile::getY() {
return Y_field;
}
void Tile::setTileset(int value) { 
Tileset_field = value;
}
                                                        int Tile::getTileset() {
return Tileset_field;
}
void Tile::setRotation(int value) { 
Rotation_field = value;
}
                                                        int Tile::getRotation() {
return Rotation_field;
}
Tile::Tile() {
r.push_back(flyweight<std::string>(std::string("Id")));
r.push_back(flyweight<std::string>(std::string("x")));
r.push_back(flyweight<std::string>(std::string("y")));
r.push_back(flyweight<std::string>(std::string("tileset")));
r.push_back(flyweight<std::string>(std::string("rotation")));
}Tile* toTile(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Tile")) {
  return static_cast<Tile*>(b);
}
else {
printf("\"toTile called with \"%s\"\n", b->type_identifier().get().c_str());
throw "";
}
}
