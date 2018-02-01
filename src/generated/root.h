#ifndef roote
#define roote

#include<propertierbase.h>

#include<json.hpp>

using nlohmann::json;

#include<boost/flyweight.hpp>
using namespace boost::flyweights;
#include<propertierbase.h>
#include<map>
#include<string>
#include<boost/flyweight.hpp>
#include<QString>
#include<either.h>
#include<cstring>
class Map;
class root: public Propertierbase {
 protected: std::map<std::string, std::map<std::string, Propertierbase*>>* registry = nullptr;
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
public: virtual std::vector<Propertierbase*> registryOf (std::string type) = 0;virtual void set(flyweight<std::string> propertyname, flyweight<std::string> value) {
if(propertyname == std::string("Id") ) { Id_field = value; return; } }virtual flyweight<std::string> get(flyweight<std::string> propertyname, bool *success, flyweight<std::string> type_helper) const {
if(propertyname == std::string("Id")) {
  *success = true;
  return Id_field;
} *success = false; flyweight<std::string> invalid_data; return invalid_data;
}
public: virtual std::string toJSON() const;
 virtual void fromJSON(const char* json);
public: root();

std::vector<flyweight<std::string>> r;
std::vector<flyweight<std::string>> names() { return r; }

virtual flyweight<std::string> type_identifier() { return flyweight<std::string>("root"); }
virtual int property_count() { return 1; }
virtual flyweight<std::string> type_name(flyweight<std::string> propertyname) const {
if(propertyname == std::string("Id")) return flyweight<std::string>("flyweight<std::string>");return flyweight<std::string>("");
}

};

root* toRoot(Propertierbase *b);

    void to_json(json& j, const root& c);
    void from_json(const json& j, root& c);
    void to_json(json& j, const root* c);
    void from_json(const json& j, root* c);
#endif