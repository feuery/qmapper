#ifndef propertierbasee
#define propertierbasee
//// generated at #<date-time 2017-06-17 18:44:10.914 {10067809C3}>

class Propertierbase
{
public:
  Propertierbase();
  virtual ~Propertierbase();
  virtual const char* type_name(const char* propertyname) = 0;
  virtual const char** names() = 0;
  virtual const char* type_identifier() = 0;
  virtual bool valid() { return false;}
virtual void set(const char* propertyname, int value);
virtual int get(const char* propertyname, bool *success, int* type_helper);

};
#endif
