#include <files.h>
#include <QtDebug>
#include <QString>

#include <stdlib.h>

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
  system("pwd");
  if (!f) {
    QString src3 = QCoreApplication::applicationDirPath() +"/"+ src;
    qDebug() << "File not found in " << src << ". Trying to open " << src3;
        f = fopen(src3.toStdString().c_str(), "r");
  }

  if(!f) {
    QString src2 = "./shaders/"+QString(src);
    qDebug() << "File not found in " << src << ". Trying to open " << src2;

    f = fopen(src2.toStdString().c_str(), "r");
  }
  if(f) {
    qDebug() << "File opened";
    long f_size = filesize(f);
    char* buffer = new char[f_size];

    if(!buffer) {
      fclose(f);
      delete [] buffer;
      return NULL;
    }
    else {
      fread(buffer, 1, f_size, f);
      fclose(f);
      std::string toret = buffer;
      delete [] buffer;
      return toret;
    }
  } else qDebug() << "File wasn't opened";

  return NULL;
}
