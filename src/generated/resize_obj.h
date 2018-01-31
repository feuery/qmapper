#ifndef resize_datae
#define resize_datae

#include<propertierbase.h>

#include<boost/flyweight.hpp>
using namespace boost::flyweights;
#include<resize-enums.h>
#include<cstring>
;
class resize_data: public Propertierbase {
 public: virtual void setNew_width(int val);
virtual int getNew_width();
int New_width_field = 0;
public: virtual void setNew_height(int val);
virtual int getNew_height();
int New_height_field = 0;
public: virtual void setVertical_anchor(verticalAnchor val);
virtual verticalAnchor getVertical_anchor();
verticalAnchor Vertical_anchor_field = TOP;
public: virtual void setHorizontal_anchor(horizontalAnchor val);
virtual horizontalAnchor getHorizontal_anchor();
horizontalAnchor Horizontal_anchor_field = LEFT;virtual void set(flyweight<std::string> propertyname, flyweight<std::string> value) {
if(propertyname == std::string("Id") ) { Id_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, int value) {
if(propertyname == std::string("new_width") ) { New_width_field = value; return; }
if(propertyname == std::string("new_height") ) { New_height_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, verticalAnchor value) {
if(propertyname == std::string("vertical_anchor") ) { Vertical_anchor_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, horizontalAnchor value) {
if(propertyname == std::string("horizontal_anchor") ) { Horizontal_anchor_field = value; return; } }virtual flyweight<std::string> get(flyweight<std::string> propertyname, bool *success, flyweight<std::string> type_helper) {
if(propertyname == std::string("Id")) {
  *success = true;
  return Id_field;
} *success = false; flyweight<std::string> invalid_data; return invalid_data;
}
virtual int get(flyweight<std::string> propertyname, bool *success, int type_helper) {
if(propertyname == std::string("new_width")) {
  *success = true;
  return New_width_field;
}
if(propertyname == std::string("new_height")) {
  *success = true;
  return New_height_field;
} *success = false; int invalid_data; return invalid_data;
}
virtual verticalAnchor get(flyweight<std::string> propertyname, bool *success, verticalAnchor type_helper) {
if(propertyname == std::string("vertical_anchor")) {
  *success = true;
  return Vertical_anchor_field;
} *success = false; verticalAnchor invalid_data; return invalid_data;
}
virtual horizontalAnchor get(flyweight<std::string> propertyname, bool *success, horizontalAnchor type_helper) {
if(propertyname == std::string("horizontal_anchor")) {
  *success = true;
  return Horizontal_anchor_field;
} *success = false; horizontalAnchor invalid_data; return invalid_data;
}
public: resize_data();

std::vector<flyweight<std::string>> r;
std::vector<flyweight<std::string>> names() { return r; }

virtual flyweight<std::string> type_identifier() { return flyweight<std::string>("resize_data"); }
virtual int property_count() { return 5; }
virtual flyweight<std::string> type_name(flyweight<std::string> propertyname) {
if(propertyname == std::string("Id")) return flyweight<std::string>("flyweight<std::string>");
if(propertyname == std::string("new_width")) return flyweight<std::string>("int");
if(propertyname == std::string("new_height")) return flyweight<std::string>("int");
if(propertyname == std::string("vertical_anchor")) return flyweight<std::string>("verticalAnchor");
if(propertyname == std::string("horizontal_anchor")) return flyweight<std::string>("horizontalAnchor");return flyweight<std::string>("");
}

};

resize_data* toResize_data(Propertierbase *b);
#endif