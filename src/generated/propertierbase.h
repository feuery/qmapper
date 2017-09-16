#ifndef propertierbasee
#define propertierbasee
//// generated at 2017-09-16T10:57:44.766Z

#include<string>
#include<vector>

class Propertierbase 
{
public:
Propertierbase ();
virtual ~Propertierbase ();
  virtual const char* type_name(const char* propertyname) = 0;
  virtual const char** names() = 0;
  virtual const char* type_identifier() = 0;
  virtual int property_count() = 0;
virtual void set(const char *propertyname, std::string value);
virtual std::string get(const char *propertyname, bool *success, std::string type_helper);
virtual void set(const char *propertyname, int value);
virtual int get(const char *propertyname, bool *success, int type_helper);


};
#endif