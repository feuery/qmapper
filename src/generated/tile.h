#ifndef tile
#define tile
//// generated at #<date-time 2017-06-27 19:28:56.756 {10033DA103}>
#include<cstring>
#include<propertierbase.h>
class tile: public propertierbase 
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
tile() {
r[0] = "rotation";
r[1] = "tileset";
r[2] = "y";
r[3] = "x";
}
const char* type_name(const char* propertyname) {
if (strcmp(propertyname, "rotation") == 0) return "int";
if (strcmp(propertyname, "tileset") == 0) return "int";
if (strcmp(propertyname, "y") == 0) return "int";
if (strcmp(propertyname, "x") == 0) return "int";
return "";
}
const char** names() { return r; }
int property_count = 4;
virtual void set(const char* propertyname, int value) {
if(strcmp(propertyname, "rotation") == 0) rotation = value;
else if(strcmp(propertyname, "tileset") == 0) tileset = value;
else if(strcmp(propertyname, "y") == 0) y = value;
else if(strcmp(propertyname, "x") == 0) x = value;
}

virtual int get(const char* propertyname, bool *success, int* type_helper) {
if(strcmp(propertyname, "rotation") == 0) {

*success = true;
return rotation;
}else if(strcmp(propertyname, "tileset") == 0) {

*success = true;
return tileset;
}else if(strcmp(propertyname, "y") == 0) {

*success = true;
return y;
}else if(strcmp(propertyname, "x") == 0) {

*success = true;
return x;
}*success = false; int a; return a;
}
const char* r[4];
};
#endifurn y;
    }else if(strcmp(propertyname, "x") == 0) {

      *success = true;
      return x;
    }*success = false; int a; return a;
  }
  const char* r[4];
};
#endif
