#ifndef roote
#define roote

#include<propertierbase.h>

#include<boost/flyweight.hpp>
using namespace boost::flyweights;
#include<propertierbase.h>
#include<map>
#include<string>
#include<boost/flyweight.hpp>
#include<QString>
#include<cstring>
class Map;
class root: public Propertierbase {
 public: std::map<flyweight<std::string>, Propertierbase*>* registry = nullptr;
public: virtual flyweight<std::string> indexOf (int row) = 0;
public: virtual int rowOf (flyweight<std::string> id) = 0;
public: virtual std::string findNs (std::string ns) = 0;
public: virtual void saveNs (std::string ns, std::string content) = 0;virtual void set(flyweight<std::string> propertyname, flyweight<std::string> value) {
if(propertyname == std::string("Id")) { Id_field = value; return; } }virtual flyweight<std::string> get(flyweight<std::string> propertyname, bool *success, flyweight<std::string> type_helper) {
if(propertyname == std::string("Id")) {
  *success = true;
  return Id_field;
} *success = false; flyweight<std::string> invalid_data; return invalid_data;
}
public: root();

std::vector<flyweight<std::string>> r;
std::vector<flyweight<std::string>> names() { return r; }

virtual flyweight<std::string> type_identifier() { return flyweight<std::string>("root"); }
virtual int property_count() { return 1; }
virtual flyweight<std::string> type_name(flyweight<std::string> propertyname) {
if(propertyname == std::string("Id")) return flyweight<std::string>("flyweight<std::string>");return flyweight<std::string>("");
}

};

root* toRoot(Propertierbase *b);
#endif