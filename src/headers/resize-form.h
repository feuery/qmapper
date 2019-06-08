#ifndef RESIZEFORM
#define RESIZEFORM

#include <ui_resize_ui.h>
#include <QDialog>

struct xy_thing {
  int x, y;
};

class resize_form: public QDialog {
  Q_OBJECT
public:
  resize_form(int oldw, int oldh);
  Ui::resize_form ui;

  std::function<void(xy_thing)> deliverResult;		    
		    

private slots:
  void accept();
  void rejected();
};
  
#endif
