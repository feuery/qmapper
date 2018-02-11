#ifndef animatedspritee
#define animatedspritee

#include<propertierbase.h>

#include<json.hpp>

using nlohmann::json;
#include<sprite.h>
#include<vector>
#include<cstring>
;
class animatedsprite: public Propertierbase {
 public: virtual void setParentmapid(std::string val);
virtual std::string getParentmapid() const;
std::string Parentmapid_field = "";
public: virtual void setName(std::string val);
virtual std::string getName() const;
std::string Name_field = "";
public: virtual void setCurrentframeid(int val);
virtual int getCurrentframeid() const;
int Currentframeid_field = 0;
public: virtual void setMsperframe(int val);
virtual int getMsperframe() const;
int Msperframe_field = 25;
public: virtual void setAnimationplaying(bool val);
virtual bool getAnimationplaying() const;
bool Animationplaying_field = true;
public: virtual void setX (int newX) = 0;
public: virtual int getX () = 0;
public: virtual void setY (int newY) = 0;
public: virtual int getY () = 0;
public: virtual int maxFrames () = 0;
public: virtual void advanceFrame () = 0;
public: virtual void setAngle (float newangle) = 0;
public: virtual float getAngle () = 0;
protected: std::vector<Sprite*>* sprites = nullptr;virtual void set(std::string propertyname, std::string value) {
if(propertyname == std::string("Id") ) { Id_field = value; return; }
if(propertyname == std::string("parentMapId") ) { Parentmapid_field = value; return; }
if(propertyname == std::string("name") ) { Name_field = value; return; } }
virtual void set(std::string propertyname, int value) {
if(propertyname == std::string("currentFrameId") ) { Currentframeid_field = value; return; }
if(propertyname == std::string("msPerFrame") ) { Msperframe_field = value; return; } }
virtual void set(std::string propertyname, bool value) {
if(propertyname == std::string("animationPlaying") ) { Animationplaying_field = value; return; } }virtual std::string get(std::string propertyname, bool *success, std::string type_helper) const {
if(propertyname == std::string("Id")) {
  *success = true;
  return Id_field;
}
if(propertyname == std::string("parentMapId")) {
  *success = true;
  return Parentmapid_field;
}
if(propertyname == std::string("name")) {
  *success = true;
  return Name_field;
} *success = false; std::string invalid_data; return invalid_data;
}
virtual int get(std::string propertyname, bool *success, int type_helper) const {
if(propertyname == std::string("currentFrameId")) {
  *success = true;
  return Currentframeid_field;
}
if(propertyname == std::string("msPerFrame")) {
  *success = true;
  return Msperframe_field;
} *success = false; int invalid_data; return invalid_data;
}
virtual bool get(std::string propertyname, bool *success, bool type_helper) const {
if(propertyname == std::string("animationPlaying")) {
  *success = true;
  return Animationplaying_field;
} *success = false; bool invalid_data; return invalid_data;
}
public: virtual std::string toJSON() const;
 virtual void fromJSON(const char* json);
public: animatedsprite();

std::vector<std::string> r;
std::vector<std::string> names() { return r; }

virtual std::string type_identifier() { return std::string("animatedsprite"); }
virtual int property_count() { return 6; }
virtual std::string type_name(std::string propertyname) const {
if(propertyname == std::string("Id")) return std::string("std::string");
if(propertyname == std::string("parentMapId")) return std::string("std::string");
if(propertyname == std::string("name")) return std::string("std::string");
if(propertyname == std::string("currentFrameId")) return std::string("int");
if(propertyname == std::string("msPerFrame")) return std::string("int");
if(propertyname == std::string("animationPlaying")) return std::string("bool");return std::string("");
}

};

animatedsprite* toAnimatedsprite(Propertierbase *b);

    void to_json(json& j, const animatedsprite& c);
    void from_json(const json& j, animatedsprite& c);
    void to_json(json& j, const animatedsprite* c);
    void from_json(const json& j, animatedsprite* c);
    void to_json(json& j, const std::vector<animatedsprite*>* v);     void to_json(json& j, const std::vector<std::vector<animatedsprite*>>* v);
    void to_json(json& j, const std::vector<std::vector<animatedsprite>>* v);
#endif