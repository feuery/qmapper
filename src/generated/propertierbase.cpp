#include<propertierbase.h>
Propertierbase::Propertierbase() 
{

}

Propertierbase::~Propertierbase() 
{

}

void Propertierbase::set(flyweight<std::string>propertyname, Script* value) 
{

}

Script* Propertierbase::get(flyweight<std::string>propertyname, bool *success, Script* type_helper)
{

}
void Propertierbase::set(flyweight<std::string>propertyname, flyweight<std::string> value) 
{

}

flyweight<std::string> Propertierbase::get(flyweight<std::string>propertyname, bool *success, flyweight<std::string> type_helper)
{

}
void Propertierbase::set(flyweight<std::string>propertyname, std::string value) 
{

}

std::string Propertierbase::get(flyweight<std::string>propertyname, bool *success, std::string type_helper)
{

}
void Propertierbase::set(flyweight<std::string>propertyname, scriptTypes value) 
{

}

scriptTypes Propertierbase::get(flyweight<std::string>propertyname, bool *success, scriptTypes type_helper)
{

}
void Propertierbase::set(flyweight<std::string>propertyname, Texture* value) 
{

}

Texture* Propertierbase::get(flyweight<std::string>propertyname, bool *success, Texture* type_helper)
{

}
void Propertierbase::set(flyweight<std::string>propertyname, std::vector<std::vector<Tile*>>* value) 
{

}

std::vector<std::vector<Tile*>>* Propertierbase::get(flyweight<std::string>propertyname, bool *success, std::vector<std::vector<Tile*>>* type_helper)
{

}
void Propertierbase::set(flyweight<std::string>propertyname, int value) 
{

}

int Propertierbase::get(flyweight<std::string>propertyname, bool *success, int type_helper)
{

}


void Propertierbase::set(flyweight<std::string> propName, Propertierbase *b)
{
  auto prop_typename = type_name(propName);
  if(prop_typename == flyweight<std::string>(std::string("Script"))) {
Script* t = reinterpret_cast<Script*>(b);
set(propName, t);
return;
}
if(prop_typename == flyweight<std::string>(std::string("Texture"))) {
Texture* t = reinterpret_cast<Texture*>(b);
set(propName, t);
return;
}
  else {
    printf("Invalid type %s\n", prop_typename.get().c_str());
    throw "";
  }
}

Propertierbase* Propertierbase::get(flyweight<std::string> propertyname)
{
  auto prop_typename = type_name(propertyname);
  if(prop_typename == flyweight<std::string>(std::string("Script"))) {
    bool success = false;
    Script* b = nullptr;
    b = get(propertyname, &success, b);
    if(success) return (Propertierbase*)b;
    else return nullptr;
  }
if(prop_typename == flyweight<std::string>(std::string("Texture"))) {
    bool success = false;
    Texture* b = nullptr;
    b = get(propertyname, &success, b);
    if(success) return (Propertierbase*)b;
    else return nullptr;
  }
  return nullptr;
}

std::string Propertierbase::getErrorsOf(flyweight<std::string> field) {
 std::string result;

 std::vector<std::string> vec = field_error_map[field.get()];
 for(auto i = vec.begin(); i < vec.end(); i++) {
   result += *i + "\n";
 }

 return result ;
}

void Propertierbase::clearErrorsOf(flyweight<std::string> field) {
 field_error_map[field.get()].clear();
}
