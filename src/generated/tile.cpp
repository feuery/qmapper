#include <tile.h>
////// generated at 2017-09-24T08:53:33.661Z


void Tile::setX(int val) {
X_field = val;
}
                                                        int Tile::getX() {
return X_field;
}
void Tile::setY(int val) {
Y_field = val;
}
                                                        int Tile::getY() {
return Y_field;
}
void Tile::setTileset(int val) {
Tileset_field = val;
}
                                                        int Tile::getTileset() {
return Tileset_field;
}
void Tile::setRotation(int val) {
Rotation_field = val;
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
