#ifndef Spritee
#define Spritee

#include<propertierbase.h>

#include<json.hpp>

using nlohmann::json;
#include<new_obj.h>
#include<renderer.h>
#include<string>
#include<cstring>
;
class Sprite: public Propertierbase {
 public: virtual void setX (int newX) = 0;
public: virtual int getX () = 0;
public: virtual void setY (int newY) = 0;
public: virtual int getY () = 0;
public: virtual void setAngle (float newangle) = 0;
public: virtual float getAngle () = 0;
public: virtual void setParentmapid(std::string val);
virtual std::string getParentmapid() const;
std::string Parentmapid_field = "";virtual void set(std::string propertyname, std::string value) {
if(propertyname == std::string("Id") ) { Id_field = value; return; }
if(propertyname == std::string("parentMapId") ) { Parentmapid_field = value; return; } }virtual std::string get(std::string propertyname, bool *success, std::string type_helper) const {
if(propertyname == std::string("Id")) {
  *success = true;
  return Id_field;
}
if(propertyname == std::string("parentMapId")) {
  *success = true;
  return Parentmapid_field;
} *success = false; std::string invalid_data; return invalid_data;
}
public: virtual std::string toJSON() const;
 virtual void fromJSON(const char* json);
public: Sprite();

std::vector<std::string> r;
std::vector<std::string> names() { return r; }

virtual std::string type_identifier() { return std::string("Sprite"); }
virtual int property_count() { return 2; }
virtual std::string type_name(std::string propertyname) const {
if(propertyname == std::string("Id")) return std::string("std::string");
if(propertyname == std::string("parentMapId")) return std::string("std::string");return std::string("");
}

};

Sprite* toSprite(Propertierbase *b);

    void to_json(json& j, const Sprite& c);
    void from_json(const json& j, Sprite& c);
    void to_json(json& j, const Sprite* c);
    void from_json(const json& j, Sprite* c);
    void to_json(json& j, const std::vector<Sprite*>* v);     void to_json(json& j, const std::vector<std::vector<Sprite*>>* v);
    void to_json(json& j, const std::vector<std::vector<Sprite>>* v);
#endif