#include <animatedSprite.h>
#include <animatedspriteContainer.h>
#include <json.hpp>
////// generated at 2018-03-13T16:43:45.016Z


void animatedsprite::setParentmapid(std::string value) { 
Parentmapid_field = value;
for(auto fn: event_map["Parentmapid"]) { 
  fn.second(this->getId());
}
}
                                                        std::string animatedsprite::getParentmapid() const {
return Parentmapid_field;
}
void animatedsprite::setName(std::string value) { 
Name_field = value;
for(auto fn: event_map["Name"]) { 
  fn.second(this->getId());
}
}
                                                        std::string animatedsprite::getName() const {
return Name_field;
}
void animatedsprite::setCurrentframeid(int value) { 
Currentframeid_field = value;
for(auto fn: event_map["Currentframeid"]) { 
  fn.second(this->getId());
}
}
                                                        int animatedsprite::getCurrentframeid() const {
return Currentframeid_field;
}
void animatedsprite::setMsperframe(int value) { 
Msperframe_field = value;
for(auto fn: event_map["Msperframe"]) { 
  fn.second(this->getId());
}
}
                                                        int animatedsprite::getMsperframe() const {
return Msperframe_field;
}
void animatedsprite::setX(int value) { 
X_field = value;
for(auto fn: event_map["X"]) { 
  fn.second(this->getId());
}
}
                                                        int animatedsprite::getX() const {
return X_field;
}
void animatedsprite::setY(int value) { 
Y_field = value;
for(auto fn: event_map["Y"]) { 
  fn.second(this->getId());
}
}
                                                        int animatedsprite::getY() const {
return Y_field;
}
void animatedsprite::setAnimationplaying(bool value) { 
Animationplaying_field = value;
for(auto fn: event_map["Animationplaying"]) { 
  fn.second(this->getId());
}
}
                                                        bool animatedsprite::getAnimationplaying() const {
return Animationplaying_field;
}
void animatedsprite::setAngle(float value) { 
Angle_field = value;
for(auto fn: event_map["Angle"]) { 
  fn.second(this->getId());
}
}
                                                        float animatedsprite::getAngle() const {
return Angle_field;
}
animatedsprite::animatedsprite() {
r.push_back(std::string(std::string("Id")));
event_map["Id"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("parentMapId")));
event_map["parentMapId"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("name")));
event_map["name"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("currentFrameId")));
event_map["currentFrameId"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("msPerFrame")));
event_map["msPerFrame"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("animationPlaying")));
event_map["animationPlaying"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("x")));
event_map["x"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("y")));
event_map["y"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("angle")));
event_map["angle"] = std::unordered_map<int, FUN>();
}animatedsprite* toAnimatedsprite(Propertierbase *b)
 {
if(b->type_identifier() == std::string("animatedsprite")) {
  return static_cast<animatedsprite*>(b);
}
else {
printf("\"toAnimatedsprite called with \"%s\"\n", b->type_identifier().c_str());
throw "";
}
}

std::string animatedsprite::toJSON() const
{
nlohmann::json j {
{"Id", getId()},
{"Parentmapid", getParentmapid()},
{"Name", getName()},
{"Currentframeid", getCurrentframeid()},
{"Msperframe", getMsperframe()},
{"Animationplaying", getAnimationplaying()},
{"X", getX()},
{"Y", getY()},
{"Angle", getAngle()}
};
return j.dump();
}
void animatedsprite::fromJSON(const char* json_str)
{
json j = json::parse(json_str);
setId(j["Id"]);
setParentmapid(j["Parentmapid"]);
setName(j["Name"]);
setCurrentframeid(j["Currentframeid"]);
setMsperframe(j["Msperframe"]);
setAnimationplaying(j["Animationplaying"]);
setX(j["X"]);
setY(j["Y"]);
setAngle(j["Angle"]);
}

using nlohmann::json;

    void to_json(json& j, const animatedsprite& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, animatedsprite& c) {
        c.fromJSON(j.get<std::string>().c_str());
    }
    void to_json(json& j, const animatedsprite* c) {
      if(c)
        j = json::parse(c->toJSON());
      else
        j = json::parse("{\"error\": \"c is null\"}");
    }

    void to_json(json& j, const std::vector<animatedsprite*>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           j.push_back(*a);
         }
    }
}
    void to_json(json& j, const std::vector<std::vector<animatedsprite>>* v) {
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

    void to_json(json& j, const std::vector<std::vector<animatedsprite*>>* v) {
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

    void from_json(const json& j, animatedsprite* c) {
        c->fromJSON(j.get<std::string>().c_str());
}
