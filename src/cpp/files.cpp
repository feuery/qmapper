#include <files.h>

long filesize(FILE* f)
{
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);

  return size;
}

std::string read_source(const char* src)
{
  FILE* f = fopen(src, "r");
  if(f) {
      long f_size = filesize(f);
      char* buffer = new char[f_size];

      if(!buffer) {
         fclose(f);
	 delete buffer;
	 return NULL;
      }
      else {
         fread(buffer, 1, f_size, f);
	 fclose(f);
	 std::string toret = buffer;
	 delete buffer;
	 return toret;
      }
  }

  return NULL;
}
