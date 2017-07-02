#ifndef layer_inclguard
#define layer_inclguard
//// generated at #<date-time 2017-07-02 15:21:43.062 {10042CBB63}>
#include<cstring>
#include<propertierbase.h>
#include<tile.h>
#include<vector>
class layer: public Propertierbase 
{ 
 public: 
  std::vector<std::vector<tile>>*  tiles = nullptr;
 public: 
  layer();
  const char* type_name(const char* propertyname); 
  virtual const char** names() { return r; }
  int property_count = 0;
  const char* r[0];
};
#endif
