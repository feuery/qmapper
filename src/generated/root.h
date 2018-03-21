#ifndef roote
#define roote

#include<propertierbase.h>

#include<json.hpp>

using nlohmann::json;
#include<propertierbase.h>
#include<map>
#include<layer.h>
#include<map.h>
#include<animatedSprite.h>
#include<sprite.h>
#include<tileset.h>
#include<script.h>
#include<string>
#include<QString>
#include<either.h>
#include<cstring>
class Map;
class Sprite;
class Tileset;
class animatedsprite;
class root: public Propertierbase {
 public: virtual void setAnimatedsprites(std::map<std::string, animatedsprite*>* val);
virtual std::map<std::string, animatedsprite*>* getAnimatedsprites() const;
std::map<std::string, animatedsprite*>* Animatedsprites_field = nullptr;
public: virtual void setLayers(std::map<std::string, Layer*>* val);
virtual std::map<std::string, Layer*>* getLayers() const;
std::map<std::string, Layer*>* Layers_field = nullptr;
public: virtual void setMaps(std::map<std::string, Map*>* val);
virtual std::map<std::string, Map*>* getMaps() const;
std::map<std::string, Map*>* Maps_field = nullptr;
public: virtual void setScripts(std::map<std::string, Script*>* val);
virtual std::map<std::string, Script*>* getScripts() const;
std::map<std::string, Script*>* Scripts_field = nullptr;
public: virtual void setSprites(std::map<std::string, Sprite*>* val);
virtual std::map<std::string, Sprite*>* getSprites() const;
std::map<std::string, Sprite*>* Sprites_field = nullptr;
public: virtual void setTiles(std::map<std::string, Tile*>* val);
virtual std::map<std::string, Tile*>* getTiles() const;
std::map<std::string, Tile*>* Tiles_field = nullptr;
public: virtual void setTilesets(std::map<std::string, Tileset*>* val);
virtual std::map<std::string, Tileset*>* getTilesets() const;
std::map<std::string, Tileset*>* Tilesets_field = nullptr;
public: virtual int registrySize () = 0;
public: virtual Propertierbase* fetchRegister (std::string Type, std::string id) = 0;
public: virtual either<scriptTypes, std::string> findNs (std::string ns) = 0;
public: virtual void saveNs (std::string ns, std::string content) = 0;
public: virtual bool containsNs (std::string ns) = 0;
public: virtual std::vector<Propertierbase*>* registryOf (std::string type) = 0;
public: virtual std::vector<Propertierbase*> registryToList () = 0;virtual void set(std::string propertyname, std::string value) {
if(propertyname == std::string("Id") ) { setId(value); return; } }
virtual void set(std::string propertyname, std::map<std::string, animatedsprite*>* value) {
if(propertyname == std::string("animatedSprites") ) { setAnimatedsprites(value); return; } }
virtual void set(std::string propertyname, std::map<std::string, Layer*>* value) {
if(propertyname == std::string("layers") ) { setLayers(value); return; } }
virtual void set(std::string propertyname, std::map<std::string, Map*>* value) {
if(propertyname == std::string("maps") ) { setMaps(value); return; } }
virtual void set(std::string propertyname, std::map<std::string, Script*>* value) {
if(propertyname == std::string("scripts") ) { setScripts(value); return; } }
virtual void set(std::string propertyname, std::map<std::string, Sprite*>* value) {
if(propertyname == std::string("sprites") ) { setSprites(value); return; } }
virtual void set(std::string propertyname, std::map<std::string, Tile*>* value) {
if(propertyname == std::string("tiles") ) { setTiles(value); return; } }
virtual void set(std::string propertyname, std::map<std::string, Tileset*>* value) {
if(propertyname == std::string("tilesets") ) { setTilesets(value); return; } }virtual std::string get(std::string propertyname, bool *success, std::string type_helper) const {
if(propertyname == std::string("Id")) {
  *success = true;
  return getId();
} *success = false; std::string invalid_data; return invalid_data;
}
virtual std::map<std::string, animatedsprite*>* get(std::string propertyname, bool *success, std::map<std::string, animatedsprite*>* type_helper) const {
if(propertyname == std::string("animatedSprites")) {
  *success = true;
  return getAnimatedsprites();
} *success = false; std::map<std::string, animatedsprite*>* invalid_data; return invalid_data;
}
virtual std::map<std::string, Layer*>* get(std::string propertyname, bool *success, std::map<std::string, Layer*>* type_helper) const {
if(propertyname == std::string("layers")) {
  *success = true;
  return getLayers();
} *success = false; std::map<std::string, Layer*>* invalid_data; return invalid_data;
}
virtual std::map<std::string, Map*>* get(std::string propertyname, bool *success, std::map<std::string, Map*>* type_helper) const {
if(propertyname == std::string("maps")) {
  *success = true;
  return getMaps();
} *success = false; std::map<std::string, Map*>* invalid_data; return invalid_data;
}
virtual std::map<std::string, Script*>* get(std::string propertyname, bool *success, std::map<std::string, Script*>* type_helper) const {
if(propertyname == std::string("scripts")) {
  *success = true;
  return getScripts();
} *success = false; std::map<std::string, Script*>* invalid_data; return invalid_data;
}
virtual std::map<std::string, Sprite*>* get(std::string propertyname, bool *success, std::map<std::string, Sprite*>* type_helper) const {
if(propertyname == std::string("sprites")) {
  *success = true;
  return getSprites();
} *success = false; std::map<std::string, Sprite*>* invalid_data; return invalid_data;
}
virtual std::map<std::string, Tile*>* get(std::string propertyname, bool *success, std::map<std::string, Tile*>* type_helper) const {
if(propertyname == std::string("tiles")) {
  *success = true;
  return getTiles();
} *success = false; std::map<std::string, Tile*>* invalid_data; return invalid_data;
}
virtual std::map<std::string, Tileset*>* get(std::string propertyname, bool *success, std::map<std::string, Tileset*>* type_helper) const {
if(propertyname == std::string("tilesets")) {
  *success = true;
  return getTilesets();
} *success = false; std::map<std::string, Tileset*>* invalid_data; return invalid_data;
}
public: virtual std::string toJSON() const;
 virtual void fromJSON(const char* json);
public: root();

std::vector<std::string> r;
std::vector<std::string> names() { return r; }

virtual std::string type_identifier() { return std::string("root"); }
virtual int property_count() { return 8; }
virtual std::string type_name(std::string propertyname) const {
if(propertyname == std::string("Id")) return std::string("std::string");
if(propertyname == std::string("animatedSprites")) return std::string("std::map<std::string, animatedsprite>");
if(propertyname == std::string("layers")) return std::string("std::map<std::string, Layer>");
if(propertyname == std::string("maps")) return std::string("std::map<std::string, Map>");
if(propertyname == std::string("scripts")) return std::string("std::map<std::string, Script>");
if(propertyname == std::string("sprites")) return std::string("std::map<std::string, Sprite>");
if(propertyname == std::string("tiles")) return std::string("std::map<std::string, Tile>");
if(propertyname == std::string("tilesets")) return std::string("std::map<std::string, Tileset>");return std::string("");
}

};

root* toRoot(Propertierbase *b);

    void to_json(json& j, const root& c);
    void from_json(const json& j, root& c);
    void to_json(json& j, const root* c);
    void from_json(const json& j, root* c);
    void to_json(json& j, const std::vector<root*>* v);     void to_json(json& j, const std::vector<std::vector<root*>>* v);
    void to_json(json& j, const std::vector<std::vector<root>>* v);
void to_json(json& j, std::map<std::string, root*>* m);
void from_json(const json& j, std::map<std::string, root*>* m);

#else 
class root;
#endif