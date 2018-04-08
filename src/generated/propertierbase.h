#ifndef propertierbasee
#define propertierbasee
//// generated at 2018-04-08T11:26:56.031Z

#include<renderer.h>
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

#include <libguile.h>

#include<json.hpp>
#include<QOpenGLFunctions_4_3_Core>
#include<QOpenGLFunctions>

#include <unordered_map>

class Script;
class Sprite;
class Map;
class animatedsprite;
class root;
class Layer;
class Tileset;
class Tile;
class Texture;


class Propertierbase;

class FUN {
public:
  bool call_guile = false;

  SCM guile;
  std::function<void(std::string)> cpp;

  FUN& operator=(FUN& f) {
     guile = f.guile;
     cpp = f.cpp;
     call_guile = f.call_guile;
     return f;
  }

  void operator()(std::string id) {
    if(call_guile) {
      SCM bb = scm_from_locale_string(id.c_str());
      scm_call_1(guile, bb);
    }
    else cpp(id);
  }
};

class Propertierbase 
{
public:
Propertierbase ();
virtual ~Propertierbase ();
  virtual std::string type_name(std::string propertyname) const = 0;
  virtual std::vector<std::string> names() = 0;
  virtual std::string type_identifier() = 0;
  virtual int property_count() = 0;

  std::unordered_map<std::string, std::unordered_map<int, FUN>> event_map;

  virtual Propertierbase* copy() = 0;

  virtual int addEvent(std::string prop, FUN fn) {
     int id = rand();
     event_map[prop][id] = fn;
  }
  virtual void dropEvent(std::string prop, int id) {
    event_map[prop].erase(id);
  }

  void handleEvents(std::string prop) {
    for(auto fn: event_map[prop]) { 
      fn.second(this->getId());
    }
  }

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
virtual void set(std::string propertyname, std::map<std::string, Layer*>* value);
virtual std::map<std::string, Layer*>* get(std::string propertyname, bool *success, std::map<std::string, Layer*>* type_helper) const;
virtual void set(std::string propertyname, std::map<std::string, Script*>* value);
virtual std::map<std::string, Script*>* get(std::string propertyname, bool *success, std::map<std::string, Script*>* type_helper) const;
virtual void set(std::string propertyname, Script* value);
virtual Script* get(std::string propertyname, bool *success, Script* type_helper) const;
virtual void set(std::string propertyname, std::string value);
virtual std::string get(std::string propertyname, bool *success, std::string type_helper) const;
virtual void set(std::string propertyname, scriptTypes value);
virtual scriptTypes get(std::string propertyname, bool *success, scriptTypes type_helper) const;
virtual void set(std::string propertyname, verticalAnchor value);
virtual verticalAnchor get(std::string propertyname, bool *success, verticalAnchor type_helper) const;
virtual void set(std::string propertyname, std::vector<std::vector<Tile*>>* value);
virtual std::vector<std::vector<Tile*>>* get(std::string propertyname, bool *success, std::vector<std::vector<Tile*>>* type_helper) const;
virtual void set(std::string propertyname, std::map<std::string, animatedsprite*>* value);
virtual std::map<std::string, animatedsprite*>* get(std::string propertyname, bool *success, std::map<std::string, animatedsprite*>* type_helper) const;
virtual void set(std::string propertyname, int value);
virtual int get(std::string propertyname, bool *success, int type_helper) const;
virtual void set(std::string propertyname, std::map<std::string, Sprite*>* value);
virtual std::map<std::string, Sprite*>* get(std::string propertyname, bool *success, std::map<std::string, Sprite*>* type_helper) const;
virtual void set(std::string propertyname, horizontalAnchor value);
virtual horizontalAnchor get(std::string propertyname, bool *success, horizontalAnchor type_helper) const;
virtual void set(std::string propertyname, float value);
virtual float get(std::string propertyname, bool *success, float type_helper) const;
virtual void set(std::string propertyname, std::map<std::string, Map*>* value);
virtual std::map<std::string, Map*>* get(std::string propertyname, bool *success, std::map<std::string, Map*>* type_helper) const;
virtual void set(std::string propertyname, std::vector<Layer*>* value);
virtual std::vector<Layer*>* get(std::string propertyname, bool *success, std::vector<Layer*>* type_helper) const;
virtual void set(std::string propertyname, unsigned char value);
virtual unsigned char get(std::string propertyname, bool *success, unsigned char type_helper) const;
virtual void set(std::string propertyname, std::map<std::string, Tileset*>* value);
virtual std::map<std::string, Tileset*>* get(std::string propertyname, bool *success, std::map<std::string, Tileset*>* type_helper) const;
virtual void set(std::string propertyname, std::map<std::string, Tile*>* value);
virtual std::map<std::string, Tile*>* get(std::string propertyname, bool *success, std::map<std::string, Tile*>* type_helper) const;


  std::unordered_map<std::string, std::vector<std::string>> field_error_map;

  std::string getErrorsOf(std::string field);
  void clearErrorsOf(std::string field);

  virtual std::string toJSON() const = 0;
  virtual void fromJSON(const char* json) = 0;

protected:
  std::string Id_field = std::string(std::to_string(rand()));

};

using nlohmann::json;

void to_json(json& j, Propertierbase*& b);
void to_json(json& j, const std::map<std::string, std::map<std::string, Propertierbase*>>* m);
void from_json(const json& j, std::map<std::string, std::map<std::string, Propertierbase*>>* m);
void to_json(json& j, std::map<std::string, Propertierbase*> m);

Q_DECLARE_METATYPE(Propertierbase*);

#endif