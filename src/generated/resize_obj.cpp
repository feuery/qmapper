#include <resize_obj.h>
#include <json.hpp>






////// generated at 2018-04-08T18:07:57.146Z


void resize_data::setNew_width(int value) { 
New_width_field = value;
for(auto fn: event_map["New_width"]) { 
  fn.second(this->getId());
}
}
                                                        int resize_data::getNew_width() const {
return New_width_field;
}
void resize_data::setNew_height(int value) { 
New_height_field = value;
for(auto fn: event_map["New_height"]) { 
  fn.second(this->getId());
}
}
                                                        int resize_data::getNew_height() const {
return New_height_field;
}
void resize_data::setVertical_anchor(verticalAnchor value) { 
Vertical_anchor_field = value;
for(auto fn: event_map["Vertical_anchor"]) { 
  fn.second(this->getId());
}
}
                                                        verticalAnchor resize_data::getVertical_anchor() const {
return Vertical_anchor_field;
}
void resize_data::setHorizontal_anchor(horizontalAnchor value) { 
Horizontal_anchor_field = value;
for(auto fn: event_map["Horizontal_anchor"]) { 
  fn.second(this->getId());
}
}
                                                        horizontalAnchor resize_data::getHorizontal_anchor() const {
return Horizontal_anchor_field;
}
Propertierbase* resize_data::copy() {
  resize_data* t = new resize_data;
t->setId(getId());
t->setNew_width(getNew_width());
t->setNew_height(getNew_height());
t->setVertical_anchor(getVertical_anchor());
t->setHorizontal_anchor(getHorizontal_anchor()); 
return t;
}resize_data::resize_data() {
r.push_back(std::string(std::string("Id")));
event_map["Id"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("new_width")));
event_map["new_width"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("new_height")));
event_map["new_height"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("vertical_anchor")));
event_map["vertical_anchor"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("horizontal_anchor")));
event_map["horizontal_anchor"] = std::unordered_map<int, FUN>();
}resize_data* toResize_data(Propertierbase *b)
 {
if(b->type_identifier() == std::string("resize_data")) {
  return static_cast<resize_data*>(b);
}
else {
printf("\"toResize_data called with \"%s\"\n", b->type_identifier().c_str());
throw "";
}
}

std::string resize_data::toJSON() const
{
nlohmann::json j;
auto G__58 = getId();
 j["Id"] = G__58;

auto G__59 = getNew_width();
 j["New_width"] = G__59;

auto G__60 = getNew_height();
 j["New_height"] = G__60;

;
return j.dump();
}
void resize_data::fromJSON(const char* json_str)
{
json j = json::parse(json_str);
setId(j["Id"].get<std::string>());
setNew_width(j["New_width"]);
setNew_height(j["New_height"]);
}

using nlohmann::json;

    void to_json(json& j, const resize_data& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, resize_data& c) {
        c.fromJSON(j.dump().c_str());
    }
    void to_json(json& j, const resize_data* c) {
      if(c)
        j = json::parse(c->toJSON());
      else
        j = json::parse("{\"error\": \"c is null\"}");
    }

    void to_json(json& j, const std::vector<resize_data*>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           j.push_back(*a);
         }
    }
}
    void to_json(json& j, const std::vector<std::vector<resize_data>>* v) {
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

    void to_json(json& j, const std::vector<std::vector<resize_data*>>* v) {
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

    void from_json(const json& j, resize_data* c) {
        c->fromJSON(j.dump().c_str());
}

void to_json(json& j, std::map<std::string, resize_data*>* m) {
  for(auto b = m->begin(); b != m->end(); m++) {
    json j2;
    to_json(j2, b->second);
    
    j[b->first] = j2;
  }
}
void from_json(const json& j, std::map<std::string, resize_data*>* m)
{
  for(auto b = j.begin(); b != j.end(); b++) {
    resize_data *r = new resize_data;
    from_json(*b, r);
    (*m)[b.key()] = r;
  }
}