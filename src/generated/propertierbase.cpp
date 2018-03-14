#include<propertierbase.h>
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
void Propertierbase::set(std::string propertyname, std::map<std::string, std::map<std::string, Propertierbase*>>* value) 
{

}

std::map<std::string, std::map<std::string, Propertierbase*>>* Propertierbase::get(std::string propertyname, bool *success, std::map<std::string, std::map<std::string, Propertierbase*>>* type_helper) const
{

}
void Propertierbase::set(std::string propertyname, int value) 
{

}

int Propertierbase::get(std::string propertyname, bool *success, int type_helper) const
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
void Propertierbase::set(std::string propertyname, std::vector<std::vector<Tile>>* value) 
{

}

std::vector<std::vector<Tile>>* Propertierbase::get(std::string propertyname, bool *success, std::vector<std::vector<Tile>>* type_helper) const
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
