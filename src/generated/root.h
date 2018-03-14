#ifndef roote
#define roote

#include<propertierbase.h>

#include<json.hpp>

using nlohmann::json;
#include<propertierbase.h>
#include<map>
#include<string>
#include<QString>
#include<either.h>
#include<cstring>
class Map;
class root: public Propertierbase {
 public: virtual void setRegistry(std::map<std::string, std::map<std::string, Propertierbase*>>* val);
virtual std::map<std::string, std::map<std::string, Propertierbase*>>* getRegistry() const;
std::map<std::string, std::map<std::string, Propertierbase*>>* Registry_field = nullptr;
public: virtual int registrySize () = 0;
public: virtual int typeRegistrySize (std::string type_name) = 0;
public: virtual void erase (std::string type, std::string id) = 0;
public: virtual void erase (std::string type, int id) = 0;
public: virtual Propertierbase* fetchRegister (std::string Type, std::string id) = 0;
public: virtual void doRegister (std::string Type, std::string id, Propertierbase* object) = 0;
public: virtual std::vector<Propertierbase*> registryToList (std::vector<std::string> filterTypes) = 0;
public: virtual either<scriptTypes, std::string> findNs (std::string ns) = 0;
public: virtual void saveNs (std::string ns, std::string content) = 0;
public: virtual bool containsNs (std::string ns) = 0;
public: virtual std::vector<Propertierbase*> registryOf (std::string type) = 0;virtual void set(std::string propertyname, std::string value) {
if(propertyname == std::string("Id") ) { setId(value); return; } }
virtual void set(std::string propertyname, std::map<std::string, std::map<std::string, Propertierbase*>>* value) {
if(propertyname == std::string("registry") ) { setRegistry(value); return; } }virtual std::string get(std::string propertyname, bool *success, std::string type_helper) const {
if(propertyname == std::string("Id")) {
  *success = true;
  return getId();
} *success = false; std::string invalid_data; return invalid_data;
}
virtual std::map<std::string, std::map<std::string, Propertierbase*>>* get(std::string propertyname, bool *success, std::map<std::string, std::map<std::string, Propertierbase*>>* type_helper) const {
if(propertyname == std::string("registry")) {
  *success = true;
  return getRegistry();
} *success = false; std::map<std::string, std::map<std::string, Propertierbase*>>* invalid_data; return invalid_data;
}
public: virtual std::string toJSON() const;
 virtual void fromJSON(const char* json);
public: root();

std::vector<std::string> r;
std::vector<std::string> names() { return r; }

virtual std::string type_identifier() { return std::string("root"); }
virtual int property_count() { return 2; }
virtual std::string type_name(std::string propertyname) const {
if(propertyname == std::string("Id")) return std::string("std::string");
if(propertyname == std::string("registry")) return std::string("std::map<std::string, std::map<std::string, Propertierbase>>");return std::string("");
}

};

root* toRoot(Propertierbase *b);

    void to_json(json& j, const root& c);
    void from_json(const json& j, root& c);
    void to_json(json& j, const root* c);
    void from_json(const json& j, root* c);
    void to_json(json& j, const std::vector<root*>* v);     void to_json(json& j, const std::vector<std::vector<root*>>* v);
    void to_json(json& j, const std::vector<std::vector<root>>* v);

void to_json(json& j, const std::map<std::string, std::map<std::string, Propertierbase*> >* m);
void from_json(const json& j, std::map<std::string, std::map<std::string, Propertierbase*> >* m);
#endif
