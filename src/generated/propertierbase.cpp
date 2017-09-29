#include<propertierbase.h>
Propertierbase::Propertierbase() 
{

}

Propertierbase::~Propertierbase() 
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
void Propertierbase::set(flyweight<std::string>propertyname, int value) 
{

}

int Propertierbase::get(flyweight<std::string>propertyname, bool *success, int type_helper)
{

}

// NOT CREATED WITH PROPERTIER
void Propertierbase::set(flyweight<std::string> propName, Propertierbase *b)
{
  auto prop_typename = type_name(propName);
  if(prop_typename == flyweight<std::string>(std::string("Texture"))) {
    Texture *t = reinterpret_cast<Texture*>(b);
    set(propName, t);
  }
  else {
    printf("Invalid type %s\n", prop_typename.get().c_str());
    throw "";
  }
}

// NOT CREATED WITH PROPERTIER
Propertierbase* Propertierbase::get(flyweight<std::string> propertyname)
{
  auto prop_typename = type_name(propertyname);
  if(prop_typename == flyweight<std::string>(std::string("Texture"))) {
    bool success = false;
    Texture *b = nullptr;
    b = get(propertyname, &success, b);
    if(success) return (Propertierbase*)b;
    else return nullptr;
  }
  return nullptr;
}
