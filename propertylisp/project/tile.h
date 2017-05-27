#include<cstring>
class tile {
public: 
  int x = 0;
  int y = 0;
protected: 
  int getTileset();

  void setTileset(int val);


  int tileset_field = 0;
  int getRotation();

  void setRotation(int val);


  int rotation_field = 0;
  const char* to_sexpr(int x, int y);
public: 
  tile() {
    r[0] = "rotation";
    r[1] = "tileset";
  }
  const char* type_name(const char* propertyname) {
    if (strcmp(propertyname, "rotation") == 0) return "int";
    if (strcmp(propertyname, "tileset") == 0) return "int";
    return "";
  }
  template<typename T>
  void set(const char* propertyname, T value) { 
    if(strcmp(propertyname, "rotation") == 0) rotation_field = value;
    if(strcmp(propertyname, "tileset") == 0) tileset_field = value;
  }
  template<typename T>
  T get(const char* propertyname, bool* success) {
    if(strcmp(propertyname, "rotation") == 0) {
      *success = true; 
      return rotation_field; 
    }
    if(strcmp(propertyname, "tileset") == 0) {
      *success = true; 
      return tileset_field; 
    }
    T t;
    *success = false; 
    return t; 
  }
  const char* r[2];
};

#ifdef trolo
class tile {
public:
  int x = 0;
  int y = 0;
protected:
  int tileset_field = 0;
  int rotation_field = 0;

  int getRotation() {
    return rotation_field;
  }
  void setRotation(int v) {
    rotation_field = v;
  }

  int getTileset() {
    return tileset_field;
  }
  void setTileset(int v) {
    tileset_field = v;
  }
  
  const char* to_sexpr (int x, int y);

public:
  const char** propertynames() {
    return {"tileset", "rotation"};
  }

  const char* type_name(const char* propertyname)
  {
    if(strcmp(propertyname, "tileset") == 0) return "int";
    else if(strcmp(propertyname, "rotation") == 0) return "int";
    return "";
  }

  template<typename T>
  void set(const char* propertyname, T value)
  {
    if(strcmp(propertyname, "tileset") == 0) tileset_field = value;
    else if(strcmp(propertyname, "rotation") == 0) rotation_field = value;
  }

  template<typename T>
  T get(const char* propertyname, bool* success)
  {
    if(strcmp(propertyname, "tileset") == 0) {
      *success = true;
      return tileset_field;
    }
    else if(strcmp(propertyname, "rotation") == 0) {
      *success = true;
      return rotation_field;
    }
    T t;
    *success = false;
    return t;
  }  
}

  #endif
