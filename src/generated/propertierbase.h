#ifndef propertierbasee
#define propertierbasee
//// generated at 2018-02-11T07:23:28.776Z

#include<renderer.h>
#include<boost/flyweight.hpp>
#include<QString>
#include<nsValidators.h>
#include<either.h>
#include<new_obj.h>
#include<string>
#include<QOpenGLFunctions_4_3_Core>
#include<map>
#include<QOpenGLFunctions>
#include<script-types.h>
#include<propertierbase.h>
#include<vector>
#include<resize-enums.h>

#include <cstdlib>

#include<json.hpp>
#include<QOpenGLFunctions_4_3_Core>
#include<QOpenGLFunctions>

#include <unordered_map>

class Script;
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
  virtual std::string type_name(std::string propertyname) const = 0;
  virtual std::vector<std::string> names() = 0;
  virtual std::string type_identifier() = 0;
  virtual int property_count() = 0;

  virtual std::string getId() const
  {
    return Id_field;
  }

  virtual void setId(std::string v) 
  {
    Id_field = v;
  }

  void set(std::string propName, Propertierbase *b);
  Propertierbase* get(std::string propertyname) const;

virtual void set(std::string propertyname, bool value);
virtual bool get(std::string propertyname, bool *success, bool type_helper) const;
virtual void set(std::string propertyname, Script* value);
virtual Script* get(std::string propertyname, bool *success, Script* type_helper) const;
virtual void set(std::string propertyname, std::string value);
virtual std::string get(std::string propertyname, bool *success, std::string type_helper) const;
virtual void set(std::string propertyname, scriptTypes value);
virtual scriptTypes get(std::string propertyname, bool *success, scriptTypes type_helper) const;
virtual void set(std::string propertyname, verticalAnchor value);
virtual verticalAnchor get(std::string propertyname, bool *success, verticalAnchor type_helper) const;
virtual void set(std::string propertyname, int value);
virtual int get(std::string propertyname, bool *success, int type_helper) const;
virtual void set(std::string propertyname, horizontalAnchor value);
virtual horizontalAnchor get(std::string propertyname, bool *success, horizontalAnchor type_helper) const;
virtual void set(std::string propertyname, std::vector<std::vector<Tile>>* value);
virtual std::vector<std::vector<Tile>>* get(std::string propertyname, bool *success, std::vector<std::vector<Tile>>* type_helper) const;
virtual void set(std::string propertyname, std::vector<Layer*>* value);
virtual std::vector<Layer*>* get(std::string propertyname, bool *success, std::vector<Layer*>* type_helper) const;
virtual void set(std::string propertyname, unsigned char value);
virtual unsigned char get(std::string propertyname, bool *success, unsigned char type_helper) const;


  std::unordered_map<std::string, std::vector<std::string>> field_error_map;

  std::string getErrorsOf(std::string field);
  void clearErrorsOf(std::string field);

  virtual std::string toJSON() const = 0;
  virtual void fromJSON(const char* json) = 0;

protected:
  std::string Id_field = std::string(std::to_string(rand()));

};

using nlohmann::json;

Q_DECLARE_METATYPE(Propertierbase*);

#endif