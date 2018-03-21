#include <sprite.h>
#include <spriteContainer.h>
#include <json.hpp>







////// generated at 2018-03-21T17:37:13.716Z


void Sprite::setParentmapid(std::string value) { 
Parentmapid_field = value;
for(auto fn: event_map["Parentmapid"]) { 
  fn.second(this->getId());
}
}
                                                        std::string Sprite::getParentmapid() const {
return Parentmapid_field;
}
void Sprite::setName(std::string value) { 
Name_field = value;
for(auto fn: event_map["Name"]) { 
  fn.second(this->getId());
}
}
                                                        std::string Sprite::getName() const {
return Name_field;
}
void Sprite::setX(int value) { 
X_field = value;
for(auto fn: event_map["X"]) { 
  fn.second(this->getId());
}
}
                                                        int Sprite::getX() const {
return X_field;
}
void Sprite::setY(int value) { 
Y_field = value;
for(auto fn: event_map["Y"]) { 
  fn.second(this->getId());
}
}
                                                        int Sprite::getY() const {
return Y_field;
}
void Sprite::setAngle(float value) { 
Angle_field = value;
for(auto fn: event_map["Angle"]) { 
  fn.second(this->getId());
}
}
                                                        float Sprite::getAngle() const {
return Angle_field;
}
Sprite::Sprite() {
r.push_back(std::string(std::string("Id")));
event_map["Id"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("x")));
event_map["x"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("y")));
event_map["y"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("angle")));
event_map["angle"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("parentMapId")));
event_map["parentMapId"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("name")));
event_map["name"] = std::unordered_map<int, FUN>();
}Sprite* toSprite(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Sprite")) {
  return static_cast<Sprite*>(b);
}
else {
printf("\"toSprite called with \"%s\"\n", b->type_identifier().c_str());
throw "";
}
}

std::string Sprite::toJSON() const
{
nlohmann::json j;
auto G__5406 = getId();
 j["Id"] = G__5406;

auto G__5407 = getX();
 j["X"] = G__5407;

auto G__5408 = getY();
 j["Y"] = G__5408;

auto G__5409 = getAngle();
 j["Angle"] = G__5409;

auto G__5410 = getParentmapid();
 j["Parentmapid"] = G__5410;

auto G__5411 = getName();
 j["Name"] = G__5411;

;
return j.dump();
}
void Sprite::fromJSON(const char* json_str)
{
json j = json::parse(json_str);
setId(j["Id"]);
setX(j["X"]);
setY(j["Y"]);
setAngle(j["Angle"]);
setParentmapid(j["Parentmapid"]);
setName(j["Name"]);
}

using nlohmann::json;

    void to_json(json& j, const Sprite& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, Sprite& c) {
        c.fromJSON(j.get<std::string>().c_str());
    }
    void to_json(json& j, const Sprite* c) {
      if(c)
        j = json::parse(c->toJSON());
      else
        j = json::parse("{\"error\": \"c is null\"}");
    }

    void to_json(json& j, const std::vector<Sprite*>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           j.push_back(*a);
         }
    }
}
    void to_json(json& j, const std::vector<std::vector<Sprite>>* v) {
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

    void to_json(json& j, const std::vector<std::vector<Sprite*>>* v) {
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

    void from_json(const json& j, Sprite* c) {
        c->fromJSON(j.get<std::string>().c_str());
}

void to_json(json& j, std::map<std::string, Sprite*>* m) {
  for(auto b = m->begin(); b != m->end(); m++) {
    json j2;
    to_json(j2, b->second);
    
    j[b->first] = j2;
  }
}
void from_json(const json& j, std::map<std::string, Sprite*>* m)
{
  for(auto b = j.begin(); b != j.end(); b++) {
    json j2 = b.value();
    Sprite *r = new Spritecontainer;
    from_json(j2, r);
    (*m)[b.key()] = r;
  }
}