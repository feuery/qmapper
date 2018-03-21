#ifndef Scripte
#define Scripte

#include<propertierbase.h>

#include<json.hpp>

using nlohmann::json;
#include<string>
#include<script-types.h>
#include<nsValidators.h>
#include<cstring>
;
class Script: public Propertierbase {
 public: virtual void setContents(std::string val);
virtual std::string getContents() const;
std::string Contents_field = "";
public: virtual void setName(std::string val);
virtual std::string getName() const;
std::string Name_field = "";
public: virtual void setNs(std::string val);
virtual std::string getNs() const;
std::string Ns_field = "user";
public: virtual void setScript_type(scriptTypes val);
virtual scriptTypes getScript_type() const;
scriptTypes Script_type_field = scheme;virtual void set(std::string propertyname, std::string value) {
if(propertyname == std::string("Id") ) { setId(value); return; }
if(propertyname == std::string("contents") ) { setContents(value); return; }
if(propertyname == std::string("name") ) { setName(value); return; }
auto error_reporter = [&](std::string msg) {
    std::vector<std::string>& vec = field_error_map[std::string("ns")];
	printf("Pushing error %s\n", msg.c_str());
	vec.push_back(msg);};if(propertyname == std::string("ns")  && doesntContainNs(value, error_reporter)) { setNs(value); return; } }
virtual void set(std::string propertyname, scriptTypes value) {
if(propertyname == std::string("script_type") ) { setScript_type(value); return; } }virtual std::string get(std::string propertyname, bool *success, std::string type_helper) const {
if(propertyname == std::string("Id")) {
  *success = true;
  return getId();
}
if(propertyname == std::string("contents")) {
  *success = true;
  return getContents();
}
if(propertyname == std::string("name")) {
  *success = true;
  return getName();
}
if(propertyname == std::string("ns")) {
  *success = true;
  return getNs();
} *success = false; std::string invalid_data; return invalid_data;
}
virtual scriptTypes get(std::string propertyname, bool *success, scriptTypes type_helper) const {
if(propertyname == std::string("script_type")) {
  *success = true;
  return getScript_type();
} *success = false; scriptTypes invalid_data; return invalid_data;
}
public: virtual std::string toJSON() const;
 virtual void fromJSON(const char* json);
public: Script();

std::vector<std::string> r;
std::vector<std::string> names() { return r; }

virtual std::string type_identifier() { return std::string("Script"); }
virtual int property_count() { return 5; }
virtual std::string type_name(std::string propertyname) const {
if(propertyname == std::string("Id")) return std::string("std::string");
if(propertyname == std::string("contents")) return std::string("std::string");
if(propertyname == std::string("name")) return std::string("std::string");
if(propertyname == std::string("ns")) return std::string("std::string");
if(propertyname == std::string("script_type")) return std::string("scriptTypes");return std::string("");
}

};

Script* toScript(Propertierbase *b);

    void to_json(json& j, const Script& c);
    void from_json(const json& j, Script& c);
    void to_json(json& j, const Script* c);
    void from_json(const json& j, Script* c);
    void to_json(json& j, const std::vector<Script*>* v);     void to_json(json& j, const std::vector<std::vector<Script*>>* v);
    void to_json(json& j, const std::vector<std::vector<Script>>* v);
void to_json(json& j, std::map<std::string, Script*>* m);
void from_json(const json& j, std::map<std::string, Script*>* m);

#else 
class Script;
#endif