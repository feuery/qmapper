#include <tile.h>
////// generated at 2017-09-16T12:30:53.297Z

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
r[0] = "x";
r[1] = "y";
r[2] = "tileset";
r[3] = "rotation";
}