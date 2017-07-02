#include <tile.h>
///////// CPP FILE STARTS HERE, generated at #<date-time 2017-07-02 15:21:43.063 {10042E7323}>
int tile::getRotation() {
 return rotation_field; 
};
void tile::setRotation(int val) {
 rotation_field = val; 
}
int tile::getTileset() {
 return tileset_field; 
};
void tile::setTileset(int val) {
 tileset_field = val; 
}
int tile::getY() {
 return y_field; 
};
void tile::setY(int val) {
 y_field = val; 
}
int tile::getX() {
 return x_field; 
};
void tile::setX(int val) {
 x_field = val; 
}
tile::tile(){
r[0] = "rotation";
r[1] = "tileset";
r[2] = "y";
r[3] = "x";
}

const char* tile::type_name(const char* propertyname) {
if (strcmp(propertyname, "rotation") == 0) return "int";
if (strcmp(propertyname, "tileset") == 0) return "int";
if (strcmp(propertyname, "y") == 0) return "int";
if (strcmp(propertyname, "x") == 0) return "int";
return "";
}
