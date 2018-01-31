#ifndef ROTATER_H
#define ROTATER_H

#include <tool.h>

class Rotater: public Tool{
public:

  virtual void use(QMouseEvent *event, int tilex, int tiley, editorController *e);
  
private:

};

#endif //ROTATER_H
