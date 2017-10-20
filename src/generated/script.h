#ifndef Scripte
#define Scripte

#include<propertierbase.h>

#include<boost/flyweight.hpp>
using namespace boost::flyweights;
#include<string>
#include<script-types.h>
#include<nsValidators.h>
#include<cstring>
;
class Script: public Propertierbase {
 public: virtual void setContents(std::string val);
virtual std::string getContents();
std::string Contents_field = "";
public: virtual void setName(std::string val);
virtual std::string getName();
std::string Name_field = "";
public: virtual void setNs(std::string val);
virtual std::string getNs();
std::string Ns_field = "user";
public: virtual void setScript_type(scriptTypes val);
virtual scriptTypes getScript_type();
scriptTypes Script_type_field = scheme;virtual void set(flyweight<std::string> propertyname, flyweight<std::string> value) {
if(propertyname == std::string("Id") ) { Id_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, std::string value) {
if(propertyname == std::string("contents") ) { Contents_field = value; return; }
if(propertyname == std::string("name") ) { Name_field = value; return; }
auto error_reporter = [&](std::string msg) {
    std::vector<std::string>& vec = field_error_map[std::string("ns")];
	printf("Pushing error %s\n", msg.c_str());
	vec.push_back(msg);};if(propertyname == std::string("ns")  && doesntContainNs(value, error_reporter)) { Ns_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, scriptTypes value) {
if(propertyname == std::string("script_type") ) { Script_type_field = value; return; } }virtual flyweight<std::string> get(flyweight<std::string> propertyname, bool *success, flyweight<std::string> type_helper) {
if(propertyname == std::string("Id")) {
  *success = true;
  return Id_field;
} *success = false; flyweight<std::string> invalid_data; return invalid_data;
}
virtual std::string get(flyweight<std::string> propertyname, bool *success, std::string type_helper) {
if(propertyname == std::string("contents")) {
  *success = true;
  return Contents_field;
}
if(propertyname == std::string("name")) {
  *success = true;
  return Name_field;
}
if(propertyname == std::string("ns")) {
  *success = true;
  return Ns_field;
} *success = false; std::string invalid_data; return invalid_data;
}
virtual scriptTypes get(flyweight<std::string> propertyname, bool *success, scriptTypes type_helper) {
if(propertyname == std::string("script_type")) {
  *success = true;
  return Script_type_field;
} *success = false; scriptTypes invalid_data; return invalid_data;
}
public: Script();

std::vector<flyweight<std::string>> r;
std::vector<flyweight<std::string>> names() { return r; }

virtual flyweight<std::string> type_identifier() { return flyweight<std::string>("Script"); }
virtual int property_count() { return 5; }
virtual flyweight<std::string> type_name(flyweight<std::string> propertyname) {
if(propertyname == std::string("Id")) return flyweight<std::string>("flyweight<std::string>");
if(propertyname == std::string("contents")) return flyweight<std::string>("std::string");
if(propertyname == std::string("name")) return flyweight<std::string>("std::string");
if(propertyname == std::string("ns")) return flyweight<std::string>("std::string");
if(propertyname == std::string("script_type")) return flyweight<std::string>("scriptTypes");return flyweight<std::string>("");
}

};

Script* toScript(Propertierbase *b);
#endif