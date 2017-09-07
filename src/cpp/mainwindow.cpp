#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <mainwindow.h>
#include <QFileDialog>

#define btnW 200
#define btnH 25

#define btn(title) { QPushButton *btn = new QPushButton(title, this);	\
    btn->setMaximumWidth(btnW);						\
    btn->setMaximumHeight(btnH);					\
    toolbox_layout->addWidget(btn); }

#define btn2(title, container) { QPushButton *btn = new QPushButton(title, this); \
    btn->setMaximumWidth(btnW);						\
    btn->setMaximumHeight(btnH);					\
    container->addWidget(btn); }

void MainWindow::setupTree()
{
  if(ec) {
    tree.setModel(ec->documentTreeModel);
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
 
MainWindow::MainWindow(int argc, char** argv) :  QMainWindow(), t(argc, argv), ec(new editorController())
{
  ui.setupUi(this);
  ec->registerWindow(this);

  
  r = new Renderer;

  QWidget *tb = new QWidget(this);
  tb->setMaximumSize(200, INT_MAX);
  QVBoxLayout *toolbox_layout = new QVBoxLayout(this);
  toolbox_layout->setSpacing(0);

  QPushButton *btn_texture = new QPushButton("Set texture to all surfaces ", this);
  btn_texture->setMaximumWidth(btnW);
  btn_texture->setMaximumHeight(btnH);
  connect(btn_texture, &QPushButton::clicked, this, &MainWindow::setTexture_clicked);
  toolbox_layout->addWidget(btn_texture);
  toolbox_layout->addWidget(toolbox());
  
  btn("Resize map");
  btn("Run game");
  btn("Create new surface ");

  setupTree();  
  toolbox_layout->addWidget(&tree);

  tb->setLayout(toolbox_layout);;

  ui.splitter->addWidget(tb);
  ui.splitter->addWidget(r);
  
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

  puts("Controller registered");
  ec->registerWindow(this);
}
