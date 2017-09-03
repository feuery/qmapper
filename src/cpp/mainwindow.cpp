#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <mainwindow.h>
#include <QFileDialog>

#define btnW 200
#define btnH 25

#define btn(title) { QPushButton *btn = new QPushButton(title, this); \
    btn->setMaximumWidth(btnW);\
    btn->setMaximumHeight(btnH);\
    toolbox_container->addWidget(btn); }

#define btn2(title, container) { QPushButton *btn = new QPushButton(title, this);	\
    btn->setMaximumWidth(btnW);\
    btn->setMaximumHeight(btnH);\
    container->addWidget(btn); }

void MainWindow::setupTree()
{
  if(ec) {
    tree.setModel(ec->slm);
  }
  else {
    puts("ec is null\n");
    throw "";
  }      
}

QGroupBox* MainWindow::toolbox()
{
   QGroupBox *grp = new QGroupBox("Tools", this);
   QVBoxLayout *l = new QVBoxLayout(this);
   
   btn2("I'm", l);
   btn2("A", l);
   btn2("Dynamically built", l);
   btn2("Toolbox list", l);
   grp->setLayout(l);
   return grp;
}
 
MainWindow::MainWindow(int argc, char** argv) :  QFrame(), t(argc, argv), ec(new editorController())
{
  QHBoxLayout *layout = new QHBoxLayout(this);
  
  QVBoxLayout *toolbox_container = new QVBoxLayout(this);
  toolbox_container->setSpacing(0);

  // r = new Renderer;

  // QPushButton *btn_texture = new QPushButton("Set texture to all surfaces ", this);
  // btn_texture->setMaximumWidth(btnW);
  // btn_texture->setMaximumHeight(btnH);
  // connect(btn_texture, &QPushButton::clicked, this, &MainWindow::setTexture_clicked);
  // toolbox_container->addWidget(btn_texture);
  // toolbox_container->addWidget(toolbox());
  
  // btn("Resize map");
  // btn("Run game");
  // btn("Create new surface ");

  setupTree();
  toolbox_container->addWidget(&tree);
  
  layout->addWidget(&tree); //addLayout(toolbox_container);
  // layout->addWidget(r);
  
  // delete this->layout();
  setLayout(layout);
  
  t.start();
}

MainWindow::~MainWindow()
{
  t.running = false;
  if(!t.wait()) {
    qDebug()<<"Terminating guile_thread";
    t.terminate();
  }
}

void MainWindow::setTexture_clicked(bool checked)
{
  QString texture_path = QFileDialog::getOpenFileName(NULL, "Open a texture file", "~", "Images (*.png *.jpeg *.jpg *.bmp");
  
  r->texturizeDrawingQueue(texture_path);
}  

void MainWindow::registerController(editorController *ec)
{
  this->ec = ec;
  ec->registerWindow(this);
}
