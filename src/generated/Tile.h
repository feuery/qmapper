#ifndef Tile_inclguard
#define Tile_inclguard
//// generated at #<date-time 2017-07-01 22:22:12.955 {100221D863}>
#include<cstring>
#include<propertierbase.h>
class Tile: public Propertierbase 
 { 
public: 
int getX();

void setX(int val);


int x_field = 0;
int getY();

void setY(int val);


int y_field = 0;
int getTileset();

void setTileset(int val);


int tileset_field = 0;
int getRotation();

void setRotation(int val);


int rotation_field = 0;
public: 
Tile();
const char* type_name(const char* propertyname); 
virtual const char** names() { return r; }
int property_count = 4;
virtual void set(const char* propertyname, int value) {
if(strcmp(propertyname, "rotation") == 0) rotation_field = value;
else if(strcmp(propertyname, "tileset") == 0) tileset_field = value;
else if(strcmp(propertyname, "y") == 0) y_field = value;
else if(strcmp(propertyname, "x") == 0) x_field = value;
}

virtual int get(const char* propertyname, bool *success, int* type_helper) {
if(strcmp(propertyname, "rotation") == 0) {

*success = true;
return rotation_field;
}else if(strcmp(propertyname, "tileset") == 0) {

*success = true;
return tileset_field;
}else if(strcmp(propertyname, "y") == 0) {

*success = true;
return y_field;
}else if(strcmp(propertyname, "x") == 0) {

*success = true;
return x_field;
}*success = false; int a; return a;
}
const char* r[4];
};
#endif