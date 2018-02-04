#include <resize_obj.h>
#include <json.hpp>
////// generated at 2018-02-03T18:35:42.793Z


void resize_data::setNew_width(int value) { 
New_width_field = value;
}
                                                        int resize_data::getNew_width() const {
return New_width_field;
}
void resize_data::setNew_height(int value) { 
New_height_field = value;
}
                                                        int resize_data::getNew_height() const {
return New_height_field;
}
void resize_data::setVertical_anchor(verticalAnchor value) { 
Vertical_anchor_field = value;
}
                                                        verticalAnchor resize_data::getVertical_anchor() const {
return Vertical_anchor_field;
}
void resize_data::setHorizontal_anchor(horizontalAnchor value) { 
Horizontal_anchor_field = value;
}
                                                        horizontalAnchor resize_data::getHorizontal_anchor() const {
return Horizontal_anchor_field;
}
resize_data::resize_data() {
r.push_back(std::string(std::string("Id")));
r.push_back(std::string(std::string("new_width")));
r.push_back(std::string(std::string("new_height")));
r.push_back(std::string(std::string("vertical_anchor")));
r.push_back(std::string(std::string("horizontal_anchor")));
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
nlohmann::json j {
{"Id", getId()},
{"New_width", getNew_width()},
{"New_height", getNew_height()}
};
return j.dump();
}
void resize_data::fromJSON(const char* json_str)
{
json j = json::parse(json_str);
setId(j["Id"]);
setNew_width(j["New_width"]);
setNew_height(j["New_height"]);
}

using nlohmann::json;

    void to_json(json& j, const resize_data& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, resize_data& c) {
        c.fromJSON(j.get<std::string>().c_str());
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
        c->fromJSON(j.get<std::string>().c_str());
}