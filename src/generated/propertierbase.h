#ifndef propertierbase
#define propertierbase
//// generated at #<date-time 2017-06-17 18:44:10.914 {10067809C3}>

class propertierbase
{
public:
  propertierbase() {}
  ~propertierbase() {}
  virtual const char* type_name(const char* propertyname) = 0;
  const char** names();
virtual void set(const char* propertyname, int value) { };
virtual int get(const char* propertyname, bool *success, int* type_helper) { *success = false; int A; return A; };
virtual void set(const char* propertyname, int value) { };
virtual int get(const char* propertyname, bool *success, int* type_helper) { *success = false; int A; return A; };
virtual void set(const char* propertyname, int value) { };
virtual int get(const char* propertyname, bool *success, int* type_helper) { *success = false; int A; return A; };

};
#endif
