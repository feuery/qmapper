#include<propertierbase.h>
#include <QDebug>
#include <root.h>
#include <map.h>
#include <script.h>
#include <tileset.h>

Propertierbase::Propertierbase() 
{

}

Propertierbase::~Propertierbase() 
{

}

void Propertierbase::set(std::string propertyname, bool value) 
{

}

bool Propertierbase::get(std::string propertyname, bool *success, bool type_helper) const
{

}
void Propertierbase::set(std::string propertyname, std::map<std::string, Layer*>* value) 
{

}

std::map<std::string, Layer*>* Propertierbase::get(std::string propertyname, bool *success, std::map<std::string, Layer*>* type_helper) const
{

}
void Propertierbase::set(std::string propertyname, std::map<std::string, Script*>* value) 
{

}

std::map<std::string, Script*>* Propertierbase::get(std::string propertyname, bool *success, std::map<std::string, Script*>* type_helper) const
{

}
void Propertierbase::set(std::string propertyname, Script* value) 
{

}

Script* Propertierbase::get(std::string propertyname, bool *success, Script* type_helper) const
{

}
void Propertierbase::set(std::string propertyname, std::string value) 
{

}

std::string Propertierbase::get(std::string propertyname, bool *success, std::string type_helper) const
{

}
void Propertierbase::set(std::string propertyname, scriptTypes value) 
{

}

scriptTypes Propertierbase::get(std::string propertyname, bool *success, scriptTypes type_helper) const
{

}
void Propertierbase::set(std::string propertyname, verticalAnchor value) 
{

}

verticalAnchor Propertierbase::get(std::string propertyname, bool *success, verticalAnchor type_helper) const
{

}
void Propertierbase::set(std::string propertyname, std::vector<std::vector<Tile*>>* value) 
{

}

std::vector<std::vector<Tile*>>* Propertierbase::get(std::string propertyname, bool *success, std::vector<std::vector<Tile*>>* type_helper) const
{

}
void Propertierbase::set(std::string propertyname, std::map<std::string, animatedsprite*>* value) 
{

}

std::map<std::string, animatedsprite*>* Propertierbase::get(std::string propertyname, bool *success, std::map<std::string, animatedsprite*>* type_helper) const
{

}
void Propertierbase::set(std::string propertyname, int value) 
{

}

int Propertierbase::get(std::string propertyname, bool *success, int type_helper) const
{

}
void Propertierbase::set(std::string propertyname, std::map<std::string, Sprite*>* value) 
{

}

std::map<std::string, Sprite*>* Propertierbase::get(std::string propertyname, bool *success, std::map<std::string, Sprite*>* type_helper) const
{

}
void Propertierbase::set(std::string propertyname, horizontalAnchor value) 
{

}

horizontalAnchor Propertierbase::get(std::string propertyname, bool *success, horizontalAnchor type_helper) const
{

}
void Propertierbase::set(std::string propertyname, float value) 
{

}

float Propertierbase::get(std::string propertyname, bool *success, float type_helper) const
{

}
void Propertierbase::set(std::string propertyname, std::map<std::string, Map*>* value) 
{

}

std::map<std::string, Map*>* Propertierbase::get(std::string propertyname, bool *success, std::map<std::string, Map*>* type_helper) const
{

}
void Propertierbase::set(std::string propertyname, std::vector<Layer*>* value) 
{

}

std::vector<Layer*>* Propertierbase::get(std::string propertyname, bool *success, std::vector<Layer*>* type_helper) const
{

}
void Propertierbase::set(std::string propertyname, unsigned char value) 
{

}

unsigned char Propertierbase::get(std::string propertyname, bool *success, unsigned char type_helper) const
{

}
void Propertierbase::set(std::string propertyname, std::map<std::string, Tileset*>* value) 
{

}

std::map<std::string, Tileset*>* Propertierbase::get(std::string propertyname, bool *success, std::map<std::string, Tileset*>* type_helper) const
{

}
void Propertierbase::set(std::string propertyname, std::map<std::string, Tile*>* value) 
{

}

std::map<std::string, Tile*>* Propertierbase::get(std::string propertyname, bool *success, std::map<std::string, Tile*>* type_helper) const
{

}


void Propertierbase::set(std::string propName, Propertierbase *b)
{
  auto prop_typename = type_name(propName);
  if(prop_typename == std::string(std::string("Script"))) {
Script* t = reinterpret_cast<Script*>(b);
set(propName, t);
return;
}
  else {
    printf("Invalid type %s\n", prop_typename.c_str());
    throw "";
  }
}

Propertierbase* Propertierbase::get(std::string propertyname) const
{
  auto prop_typename = type_name(propertyname);
  if(prop_typename == std::string(std::string("Script"))) {
    bool success = false;
    Script* b = nullptr;
    b = get(propertyname, &success, b);
    if(success) return (Propertierbase*)b;
    else return nullptr;
  }
  return nullptr;
}

std::string Propertierbase::getErrorsOf(std::string field) {
 std::string result;

 std::vector<std::string> vec = field_error_map[field];
 for(auto i = vec.begin(); i < vec.end(); i++) {
   result += *i + "\n";
 }

 return result ;
}

void Propertierbase::clearErrorsOf(std::string field) {
 field_error_map[field].clear();
}

void to_json(json& j, Propertierbase*& b) // TODO rewrite the compiler to output these for all compiled classes
{
  std::string t = b->type_identifier();
  if(t == "root") {
    to_json(j, static_cast<root*>(b));

  }
  else if (t == "Map") {
    to_json(j, static_cast<Map*>(b));
  }
  else if(t == "Script") {
    to_json(j, static_cast<Script*>(b));
  }
  else if(t == "Tileset") {
    to_json(j, static_cast<Tileset*>(b));
  }
  else {
    qDebug() << "Didn't recogniza type " << t.c_str() << " at " << __FILE__ << ":" << __LINE__;
  }
}

void to_json(json& j, std::map<std::string, Propertierbase*> m) {
  for(auto a = m.begin(); a != m.end(); a++) {
    j[a->first] = a->second;
  }
}

//TODO write this for arbitrary deep std::map<K, V>'s
void to_json(json& j, const std::map<std::string, std::map<std::string, Propertierbase*>>* m) 
{
  for(auto a = m->begin(); a != m->end(); a++) {
    std::string k1 = a->first;
    json j2;
    for(auto a2: a->second) {
      std::string k2 = a2.first;
      j2[k2] = a2.second;
    }
    j[k1] = j2;
  }
}

void from_json(const json& j, std::map<std::string, std::map<std::string, Propertierbase*>>* m)
{
  puts("Don't call me yet, TODO IMPLEMENT");
  throw "";
}
