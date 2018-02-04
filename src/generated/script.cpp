#include <script.h>
#include <json.hpp>
////// generated at 2018-02-04T14:09:14.784Z


void Script::setContents(std::string value) { 
Contents_field = value;
}
                                                        std::string Script::getContents() const {
return Contents_field;
}
void Script::setName(std::string value) { 
Name_field = value;
}
                                                        std::string Script::getName() const {
return Name_field;
}
void Script::setNs(std::string value) { 
auto error_reporter = [&](std::string msg) {
    std::vector<std::string>& vec = field_error_map[std::string("Ns")];
	printf("Pushing error %s\n", msg.c_str());
	vec.push_back(msg);};if(doesntContainNs(value, error_reporter))Ns_field = value;
}
                                                        std::string Script::getNs() const {
return Ns_field;
}
void Script::setScript_type(scriptTypes value) { 
Script_type_field = value;
}
                                                        scriptTypes Script::getScript_type() const {
return Script_type_field;
}
Script::Script() {
r.push_back(std::string(std::string("Id")));
r.push_back(std::string(std::string("contents")));
r.push_back(std::string(std::string("name")));
r.push_back(std::string(std::string("ns")));
r.push_back(std::string(std::string("script_type")));
}Script* toScript(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Script")) {
  return static_cast<Script*>(b);
}
else {
printf("\"toScript called with \"%s\"\n", b->type_identifier().c_str());
throw "";
}
}

std::string Script::toJSON() const
{
nlohmann::json j {
{"Id", getId()},
{"Contents", getContents()},
{"Name", getName()},
{"Ns", getNs()}
};
return j.dump();
}
void Script::fromJSON(const char* json_str)
{
json j = json::parse(json_str);
setId(j["Id"]);
setContents(j["Contents"]);
setName(j["Name"]);
setNs(j["Ns"]);
}

using nlohmann::json;

    void to_json(json& j, const Script& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, Script& c) {
        c.fromJSON(j.get<std::string>().c_str());
    }
    void to_json(json& j, const Script* c) {
      if(c)
        j = json::parse(c->toJSON());
      else
        j = json::parse("{\"error\": \"c is null\"}");
    }

    void to_json(json& j, const std::vector<Script*>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           j.push_back(*a);
         }
    }
}
    void to_json(json& j, const std::vector<std::vector<Script>>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           json j2;
           for(auto o = a->begin(); o != a->end(); o++) {
             j2.push_back(*o);
           }
           j.push_back(j2);
         }
    }
}

    void to_json(json& j, const std::vector<std::vector<Script*>>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           json j2;
           for(auto o = a->begin(); o != a->end(); o++) {
             j2.push_back(*o);
           }
           j.push_back(j2);
         }
    }
}

    void from_json(const json& j, Script* c) {
        c->fromJSON(j.get<std::string>().c_str());
}
