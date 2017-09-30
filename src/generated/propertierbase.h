#ifndef propertierbasee
#define propertierbasee
//// generated at 2017-09-30T16:56:43.389Z

#include<boost/flyweight.hpp>
#include<QString>
#include<string>
#include<map>
#include<script-types.h>
#include<propertierbase.h>
#include<vector>

#include <cstdlib>

#include<boost/flyweight.hpp>
#include<QOpenGLFunctions_4_3_Core>
#include<QOpenGLFunctions>

using namespace boost::flyweights;
class Map;
class root;
class Layer;
class Tile;
class Texture;

class Propertierbase 
{
public:
Propertierbase ();
virtual ~Propertierbase ();
  virtual flyweight<std::string> type_name(flyweight<std::string> propertyname) = 0;
  virtual std::vector<flyweight<std::string>> names() = 0;
  virtual flyweight<std::string> type_identifier() = 0;
  virtual int property_count() = 0;

  virtual flyweight<std::string> getId() 
  {
    return Id_field;
  }

  virtual void setId(flyweight<std::string> v) 
  {
    Id_field = v;
  }

  void set(flyweight<std::string> propName, Propertierbase *b);
  Propertierbase* get(flyweight<std::string>propertyname);

virtual void set(flyweight<std::string>propertyname, std::string value);
virtual std::string get(flyweight<std::string>propertyname, bool *success, std::string type_helper);
virtual void set(flyweight<std::string>propertyname, scriptTypes value);
virtual scriptTypes get(flyweight<std::string>propertyname, bool *success, scriptTypes type_helper);
virtual void set(flyweight<std::string>propertyname, Texture* value);
virtual Texture* get(flyweight<std::string>propertyname, bool *success, Texture* type_helper);
virtual void set(flyweight<std::string>propertyname, int value);
virtual int get(flyweight<std::string>propertyname, bool *success, int type_helper);


protected:
  flyweight<std::string> Id_field = flyweight<std::string>(std::to_string(rand()));

};

Q_DECLARE_METATYPE(Propertierbase*);

#endif