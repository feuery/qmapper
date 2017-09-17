#ifndef MAIN_H
#define MAIN_H

#include <QMainWindow>
#include <QGroupBox>
#include <QTreeView>
#include <QMenu>
#include <QAction>
#include <renderer.h>
#include <guile_thread.h>
#include <editorController.h>

#include <ui_main.h>

class editorController;

class MainWindow: public QMainWindow {
  Q_OBJECT
public:
  MainWindow(int argc, char** argv);
  ~MainWindow();

  void registerController(editorController *ec);
public slots:
  void setTexture_clicked(bool checked);
  
private:

  Ui::MainWindow ui;
  
  Renderer *map_view;
  Renderer *tileset_view;
  Guile_Thread t;
  editorController *ec;
  QGroupBox* toolbox();

  void setupTree();

  QTreeView tree;

  void setupTreeCtxMenu();
  void editObject();
};

#endif //MAIN_H
