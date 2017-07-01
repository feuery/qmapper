#include <Tile.h>
///////// CPP FILE STARTS HERE, generated at #<date-time 2017-07-01 22:22:12.955 {1002222693}>
int Tile::getRotation() {
 return rotation_field; 
};
void Tile::setRotation(int val) {
 rotation_field = val; 
}
int Tile::getTileset() {
 return tileset_field; 
};
void Tile::setTileset(int val) {
 tileset_field = val; 
}
int Tile::getY() {
 return y_field; 
};
void Tile::setY(int val) {
 y_field = val; 
}
int Tile::getX() {
 return x_field; 
};
void Tile::setX(int val) {
 x_field = val; 
}
Tile::Tile(){
r[0] = "rotation";
r[1] = "tileset";
r[2] = "y";
r[3] = "x";
}

const char* Tile::type_name(const char* propertyname) {
if (strcmp(propertyname, "rotation") == 0) return "int";
if (strcmp(propertyname, "tileset") == 0) return "int";
if (strcmp(propertyname, "y") == 0) return "int";
if (strcmp(propertyname, "x") == 0) return "int";
return "";
}
