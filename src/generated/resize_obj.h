#ifndef resize_datae
#define resize_datae

#include<propertierbase.h>

#include<json.hpp>

using nlohmann::json;
#include<resize-enums.h>
#include<cstring>
;
class resize_data: public Propertierbase {
 public: virtual void setNew_width(int val);
virtual int getNew_width() const;
int New_width_field = 0;
public: virtual void setNew_height(int val);
virtual int getNew_height() const;
int New_height_field = 0;
public: virtual void setVertical_anchor(verticalAnchor val);
virtual verticalAnchor getVertical_anchor() const;
verticalAnchor Vertical_anchor_field = TOP;
public: virtual void setHorizontal_anchor(horizontalAnchor val);
virtual horizontalAnchor getHorizontal_anchor() const;
horizontalAnchor Horizontal_anchor_field = LEFT;virtual void set(std::string propertyname, std::string value) {
if(propertyname == std::string("Id") ) { setId(value); return; } }
virtual void set(std::string propertyname, int value) {
if(propertyname == std::string("new_width") ) { setNew_width(value); return; }
if(propertyname == std::string("new_height") ) { setNew_height(value); return; } }
virtual void set(std::string propertyname, verticalAnchor value) {
if(propertyname == std::string("vertical_anchor") ) { setVertical_anchor(value); return; } }
virtual void set(std::string propertyname, horizontalAnchor value) {
if(propertyname == std::string("horizontal_anchor") ) { setHorizontal_anchor(value); return; } }virtual std::string get(std::string propertyname, bool *success, std::string type_helper) const {
if(propertyname == std::string("Id")) {
  *success = true;
  return getId();
} *success = false; std::string invalid_data; return invalid_data;
}
virtual int get(std::string propertyname, bool *success, int type_helper) const {
if(propertyname == std::string("new_width")) {
  *success = true;
  return getNew_width();
}
if(propertyname == std::string("new_height")) {
  *success = true;
  return getNew_height();
} *success = false; int invalid_data; return invalid_data;
}
virtual verticalAnchor get(std::string propertyname, bool *success, verticalAnchor type_helper) const {
if(propertyname == std::string("vertical_anchor")) {
  *success = true;
  return getVertical_anchor();
} *success = false; verticalAnchor invalid_data; return invalid_data;
}
virtual horizontalAnchor get(std::string propertyname, bool *success, horizontalAnchor type_helper) const {
if(propertyname == std::string("horizontal_anchor")) {
  *success = true;
  return getHorizontal_anchor();
} *success = false; horizontalAnchor invalid_data; return invalid_data;
}
public: virtual std::string toJSON() const;
 virtual void fromJSON(const char* json);
public: resize_data();

std::vector<std::string> r;
std::vector<std::string> names() { return r; }

virtual std::string type_identifier() { return std::string("resize_data"); }
virtual int property_count() { return 5; }
virtual std::string type_name(std::string propertyname) const {
if(propertyname == std::string("Id")) return std::string("std::string");
if(propertyname == std::string("new_width")) return std::string("int");
if(propertyname == std::string("new_height")) return std::string("int");
if(propertyname == std::string("vertical_anchor")) return std::string("verticalAnchor");
if(propertyname == std::string("horizontal_anchor")) return std::string("horizontalAnchor");return std::string("");
}

};

resize_data* toResize_data(Propertierbase *b);

    void to_json(json& j, const resize_data& c);
    void from_json(const json& j, resize_data& c);
    void to_json(json& j, const resize_data* c);
    void from_json(const json& j, resize_data* c);
    void to_json(json& j, const std::vector<resize_data*>* v);     void to_json(json& j, const std::vector<std::vector<resize_data*>>* v);
    void to_json(json& j, const std::vector<std::vector<resize_data>>* v);
#endif