#ifndef Tilee
#define Tilee

#include<propertierbase.h>

#include<cstring>
;
class Tile: public Propertierbase {
 public: virtual void setX(int val);
virtual int getX();
int X_field = 0;
public: virtual void setY(int val);
virtual int getY();
int Y_field = 0;
public: virtual void setTileset(int val);
virtual int getTileset();
int Tileset_field = 0;
public: virtual void setRotation(int val);
virtual int getRotation();
int Rotation_field = 0;virtual void set(const char* propertyname, int value) {
if(strcmp(propertyname, "x") == 0) { X_field = value; return; }
if(strcmp(propertyname, "y") == 0) { Y_field = value; return; }
if(strcmp(propertyname, "tileset") == 0) { Tileset_field = value; return; }
if(strcmp(propertyname, "rotation") == 0) { Rotation_field = value; return; } }virtual int get(const char* propertyname, bool *success, int type_helper) {
if(strcmp(propertyname, "x") == 0) {
  *success = true;
  return X_field;
}
if(strcmp(propertyname, "y") == 0) {
  *success = true;
  return Y_field;
}
if(strcmp(propertyname, "tileset") == 0) {
  *success = true;
  return Tileset_field;
}
if(strcmp(propertyname, "rotation") == 0) {
  *success = true;
  return Rotation_field;
} *success = false; int invalid_data; return invalid_data;
}
public: Tile();

const char * r[4];
const char** names() { return r; }

virtual const char* type_identifier() { return "Tile"; }
virtual int property_count() { return 4; }
virtual const char* type_name(const char *propertyname) {
if(strcmp(propertyname, "x") == 0) return "int";
if(strcmp(propertyname, "y") == 0) return "int";
if(strcmp(propertyname, "tileset") == 0) return "int";
if(strcmp(propertyname, "rotation") == 0) return "int";return "";
}

};
#endif