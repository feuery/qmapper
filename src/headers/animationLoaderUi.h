#ifndef ANIMATIONLOADERUI_H
#define ANIMATIONLOADERUI_H
#include <ui_animationloader.h>
#include <QDialog>

class animationLoader: public QDialog {
  Q_OBJECT
 public:
  animationLoader();

  Ui::Dialog ui;
};;

#endif //ANIMATIONLOADERUI_H
