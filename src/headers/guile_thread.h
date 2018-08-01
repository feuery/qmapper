#ifndef GUILE_THREAD_H
#define GUILE_THREAD_H

#include <QThread>

class Guile_Thread: public QThread{
public:
  Guile_Thread(int argc, char** argv); 

  void run();

  bool running = false;
  
private:

  int argc;
  char** argv;

};

#endif //GUILE_THREAD_H
