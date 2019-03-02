// ECL reference
// http://vwood.github.io/embedded-ecl.html

// pitäiskö toi asdf-kakka korvata tällä?
// https://web.archive.org/web/20141018030226/https://cwndr.ws/posts/Calling-Lisp-from-C-using-ECL.html

#include <QDebug>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <mainwindow.h>
#include <QFileDialog>
#include <propertyEditor.h>

#include <iostream>

#include <QMessageBox>
#include <editorController_guile.h>

#include <tileview_renderer.h>

#include <tool_list.h>
#include <animationLoaderUi.h>
#include <engine.h>

#include <ecl/ecl.h>
#include <guile_fn.h>

#include <QAbstractItemModelTester>

#define btnW 200
#define btnH 25

#define setSize(btn) btn->setMaximumWidth(btnW);	\
  btn->setMaximumHeight(btnH);						

#define btn(title) { QPushButton *btn = new QPushButton(title, this);	\
    btn->setMaximumWidth(btnW);						\
    btn->setMaximumHeight(btnH);					\
    toolbox_layout->addWidget(btn); }

#define btn2(title, container) { QPushButton *btn = new QPushButton(title, this); \
    btn->setMaximumWidth(btnW);						\
    btn->setMaximumHeight(btnH);					\
    container->addWidget(btn); }

QString MainWindow::getTextureLocation() {
  return QFileDialog::getOpenFileName(this, "Load texture file", ".", "Image Files (*.png)");
}

void prepareModel(cl_object doc)
{
  MainWindow *w = editorController::instance->w;
  QTreeWidget &tree = w->tree;
  holder &rootThing = editorController::instance->document;
  cl_object root = rootThing.getValue(),
    regToList = makefn("qmapper.root:root-registrytolIst"),
    rootLayers = makefn("qmapper.root:root-layers"),
    rootSprites = makefn("qmapper.root:root-sprites"),
    rootAnimes = makefn("qmapper.root:root-animatedsprites"),
    typeOf = makefn("qmapper.std:q-type-of"),
    get_prop = makefn("qmapper.std:get-prop"),
    nth = makefn("common-lisp:nth"),
    prin = makefn("prin1"),
    len = makefn("length"),
    format = makefn("format"),
    mapLayers = makefn("qmapper.map:map-layers"),
    mapSprites = makefn("qmapper.map:map-sprites"),
    mapAnims = makefn("qmapper.map:map-animatedsprites"),
    rootAsList = cl_funcall(2, regToList, root),
    map_keys = makefn("qmapper.std:fset-map-keys"),
    keys = cl_funcall(2, map_keys, rootAsList);

  puts("preparing model");
  tree.clear();
  
  int length = fixint(cl_funcall(2, len, keys));
  for(int i = 0; i < length; i++) {
    cl_object key = cl_funcall(3, nth, ecl_make_int32_t(i), keys);
    // cl_funcall(5, format, ECL_T, c_string_to_object("\"searching for ~a in ~a~%\""), key, rootAsList);

    cl_funcall(4, format, ECL_T, c_string_to_object("\"key is ~a~%\""), key);
    cl_object obj = cl_funcall(3, get_prop, rootAsList, key);
    assert(obj != ECL_NIL);

    std::string type = ecl_string_to_string(cl_funcall(2, typeOf, obj));

    if(type == "LAYER") continue;
    if(type == "SPRITE") continue;

    QTreeWidgetItem *item = new QTreeWidgetItem;
    QString id (ecl_string_to_string(get(obj, "id")).c_str());
    item->setText(0, QString(ecl_string_to_string(get(obj, "name")).c_str()) + " | " + id);

    if(type == "MAP") {
      cl_object layers = cl_funcall(2, mapLayers, obj);
      cl_object sprites = cl_funcall(2, mapSprites, obj);
      cl_object animes = cl_funcall(2, mapAnims, obj);
      int layer_len = fixint(cl_funcall(2, len, layers)),
	sprite_len = fixint(cl_funcall(2, len, sprites)),
	anime_len = fixint(cl_funcall(2, len, animes));
      
      for(int l = 0; l < layer_len; l++) {
	QString cl_layer_id(ecl_string_to_string(cl_funcall(3, nth, ecl_make_int32_t(l), layers)).c_str());
	cl_layer_id = cl_layer_id.replace("\"", "");
	
	
	auto layer = get(cl_funcall(2, rootLayers, root), cl_layer_id.toStdString().c_str());
	
	QTreeWidgetItem *layer_item = new QTreeWidgetItem;
	assert(layer != ECL_NIL);

	QString layer_id (ecl_string_to_string(get(layer, "id")).c_str());
	layer_item->setText(0, QString(ecl_string_to_string(get(layer, "name")).c_str()) + " | " + layer_id);
	item->addChild(layer_item);
      }

      for(int sp = 0; sp < sprite_len; sp++) {
	QString cl_sprite_id(ecl_string_to_string(cl_funcall(4, format, ECL_NIL, c_string_to_object("\"~a\""),
							     cl_funcall(3, nth, ecl_make_int32_t(sp), sprites))).c_str());
	cl_sprite_id = cl_sprite_id.replace("\"", "");
	std::string std_id = cl_sprite_id.toStdString();

	cl_object sprite = get(cl_funcall(2, rootSprites, root), std_id.c_str());
	printf("spriteID is %s\n", std_id.c_str());
	cl_funcall(4, format, ECL_T, c_string_to_object("\"sprites are ~a~%\""), sprites);
	cl_funcall(5, format, ECL_T, c_string_to_object("\"sprite is ~a, spriteList is ~a~%\""), sprite, cl_funcall(2, rootSprites, root));
	QTreeWidgetItem *sprite_item = new QTreeWidgetItem;
	assert(sprite != ECL_NIL);

	QString sprite_id (ecl_string_to_string(get(sprite, "id")).c_str());
	sprite_item->setText(0, QString(ecl_string_to_string(get(sprite, "name")).c_str()) + " | " + sprite_id);
	item->addChild(sprite_item);
      }

      for(int anim_p = 0; anim_p < anime_len; anim_p++) {
	QString cl_anim_id(ecl_string_to_string(cl_funcall(4, format, ECL_NIL, c_string_to_object("\"~a\""),
							     cl_funcall(3, nth, ecl_make_int32_t(anim_p), animes))).c_str());
	cl_anim_id = cl_anim_id.replace("\"", "");
	std::string std_id = cl_anim_id.toStdString();

	cl_object anim = get(cl_funcall(2, rootAnimes, root), std_id.c_str());
	printf("animID is %s\n", std_id.c_str());
	QTreeWidgetItem *anim_item = new QTreeWidgetItem;
	assert(anim != ECL_NIL);

	QString anim_id (ecl_string_to_string(get(anim, "id")).c_str());
	anim_item->setText(0, QString(ecl_string_to_string(get(anim, "name")).c_str()) + " | " + anim_id);
	item->addChild(anim_item);
      }
    }
    tree.addTopLevelItem(item);
  }

  puts("model prepared");
}

void MainWindow::setupTree()
{
  if(ec) {
    auto reg_hook = makefn("qmapper.root:register-doc-hook");  
    auto hw_lambda =  LAMBDA(prepareModel, 1);
    assert(hw_lambda != ECL_NIL);
    cl_funcall(2, reg_hook, hw_lambda);

    prepareModel(nullptr);
   
    connect(&tree, &QTreeView::clicked, [&](QModelIndex index) {
					  puts("We're at DOMTree::clicked");
					  QString selectedText = tree.currentItem()->text(0);
					  printf("Selected text is %s\n", selectedText.toStdString().c_str());
					  
					  QString id = selectedText.split("|").at(1);
					  id = id.replace("\"", "").replace(" ", "");
					  printf("finding by id %s\n", id.toStdString().c_str());
					  cl_object selected_object = find_by_id(id.toStdString().c_str()),
					    qtype_of = makefn("qmapper.std:q-type-of"),
					    format = makefn("format");
					  puts("Found by id");

					  std::string type = ecl_string_to_string(cl_funcall(2, qtype_of, selected_object));
					  printf("type is %s\n", type.c_str());
					  
					  if(type == "TILESET") {
					    cl_object t = selected_object,
					      select_tileset = makefn("qmapper.tileset:select-tileset");
					    cl_funcall(2, select_tileset, t);
					    puts("Selected a tileset");
					  }
					  else if(type == "MAP") {
					    cl_object m = selected_object,
					      first = makefn("first"),
					      map_layers = makefn("qmapper.map:map-layers"),
					      layer_id = cl_funcall(2, first, cl_funcall(2, map_layers, m)),
					      map_id = get(m, "id"),
					      select_map_layer = makefn("qmapper.map:select-map-layer");
					    
					    ec->document.setValue(cl_funcall(4, select_map_layer, ec->document.getValue(), map_id, layer_id));
					  }
					  else if(type == "LAYER") {
					    auto format = makefn("format");
					    cl_funcall(4, format, ECL_T, c_string_to_object("\"layer is ~a~%\""), selected_object);
					    cl_object find_l_parent = makefn("qmapper.map:find-layer-parent"),				      
					      map = cl_funcall(3, find_l_parent, get(selected_object, "id"), ec->document.getValue()),
					      map_id = get(map, "ID"),
					      layer_id = get(selected_object, "ID"),
					      select_map_layer = makefn("qmapper.map:select-map-layer");
					    

					    cl_funcall(6, format, ECL_T, c_string_to_object("\"selecting layer ~a from map ~a (~a)~%\""), layer_id, map_id, map);
					    
					    ec->document.setValue(cl_funcall(4, select_map_layer, ec->document.getValue(), map_id, layer_id));
					    
					  }
					  else puts("You didn't click on a recognizable object");
					});
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

  for(auto i: tool_cache) {
    QPushButton *btn = new QPushButton(i.first.c_str(), this); 
    btn->setMaximumWidth(btnW);						
    btn->setMaximumHeight(btnH);
    connect(btn, &QPushButton::clicked,
	    [=]()
	    {
	      qDebug() << "Setting " << i.first.c_str() << "as selected tool";
	      editorController::instance->t = tool_cache.at(i.first);
	    });
    l->addWidget(btn);
  }
    
  grp->setLayout(l);
  return grp;
}

void MainWindow::editObject()
{
  QString itemtext = tree.currentItem()->text(0);
  auto splitted = itemtext.split("|");
  if(splitted.count() < 1) {
    printf("item %s doesn't have id\n", itemtext.toStdString().c_str());
    return;
  }
  
  QString id = splitted[1].replace("\"", "").replace(" ", "");

  printf("id to edit is %s\n", id.toStdString().c_str());
  holder &rootThing = editorController::instance->document;
  cl_object root = rootThing.getValue(),
    regToList = makefn("qmapper.root:root-registrytolIst"),
    nth = makefn("common-lisp:nth"),
    len = makefn("length"),
    format = makefn("format"),
    rootAsList = cl_funcall(2, regToList, root);

  printf("id is %s\n", id.toStdString().c_str());
  // cl_funcall(4, format, ECL_T, c_string_to_object("\"rootAsList: ~a~%\""), rootAsList);
  auto b = get(rootAsList, id.toStdString().c_str());
  assert(b != ECL_NIL);
  
  auto find_path = makefn("qmapper.root:find-path-of"),
    path = cl_funcall(3, find_path, root, b);
  assert(path != ECL_NIL);

  cl_funcall(4, format, ECL_T, c_string_to_object("\"path to b is ~a~%\""), path);
  
  Propertyeditor *p = new Propertyeditor(path, this, true);
  p->show();
  
}

void MainWindow::prepare_server_button(QVBoxLayout *toolbox_layout)
{
  server_connection = new QPushButton("Start documentation server");
  setSize(server_connection);

  toolbox_layout->addWidget(server_connection);

  connect(server_connection, &QPushButton::clicked,
	  [&]() {
	    if(!s.initialized && s.start_server()) {
	      server_connection->setText("Server running");
	    }
	  });
}

void MainWindow::setupTreeCtxMenu()
{
  QAction *edit = new QAction("&Edit", this);
  edit->setShortcuts(QKeySequence::New);
  edit->setStatusTip("Edit object");
  connect(edit, &QAction::triggered, this, &MainWindow::editObject);

  // TODO
  // Add a menu:
  //       new -> glsl script	
  //           -> scheme script
  //           -> ------------
  //           -> texture
  //           -> ------------			
  //           -> sprite
  //           -> animation

  tree.setContextMenuPolicy(Qt::ActionsContextMenu);
  tree.addAction(edit);
  QMenu *newMenu = new QMenu(this);

  QAction *glsl = new QAction("&GLSL", this),
    *scheme = new QAction("&Scheme", this),
    *tileset = new QAction("T&ileset", this),
    *sprite = new QAction("S&prite", this),
    *animation = new QAction("&Animation", this);
  
  glsl->setStatusTip("New GLSL-script asset");
  scheme->setStatusTip("New Scheme asset");
  tileset->setStatusTip("New tileset");
  sprite->setStatusTip("Load a new sprite");
  animation->setStatusTip("Load a new animation");

  connect(glsl, &QAction::triggered, []() {
      add_glsl_script();
    });
  connect(scheme, &QAction::triggered, []() {
      add_scheme_script();
    });;

  connect(tileset, &QAction::triggered, this, &MainWindow::newTileset) ;
  connect(sprite, &QAction::triggered, [=]() {
        QString texture_path = QFileDialog::getOpenFileName(this, "Load texture file", ".", "Image Files (*.png)");
	if(!texture_path.isNull())
	  editorController::instance->loadSprite(texture_path.toStdString().c_str());
    });

  connect(animation, &QAction::triggered, [=]() {
      animationLoader *l = new animationLoader;
      l->show();
    });

  newMenu->addAction(glsl);
  newMenu->addAction(scheme);
  newMenu->addSeparator();
  newMenu->addAction(tileset);
  newMenu->addAction(sprite);
  newMenu->addAction(animation);

  QAction *newMenu_act = new QAction("&New", this);
  newMenu_act->setMenu(newMenu);
  tree.addAction(newMenu_act);
}

void MainWindow::newTileset() {
  QString texture_file = QFileDialog::getOpenFileName(this, "Load texture file", ".", "Image Files (*.png)");
  if(!texture_file.isNull()) {
    cl_object make_tset = makefn("qmapper.tileset:load-tileset"),
      tileset = cl_funcall(2, make_tset, c_string_to_object(("\""+texture_file.toStdString()+"\"").c_str()));
  }
}

void MainWindow::prepareStartEngine(QVBoxLayout *toolbox_layout)
{
  QPushButton *start = new QPushButton("Start engine");
  setSize(start);

  toolbox_layout->addSpacing(10);
  toolbox_layout->addWidget(start);

  connect(start, &QPushButton::clicked,
	  [&]() {
	    qDebug() << "Starting engine...";

	    auto e = ec->e;
	    
	    e->er->getDrawQueue().clear();
	    int w = e->er->width(),
	      h = e->er->height();
	    
	    for(Renderable *dq: map_view->getDrawQueue()) {
	      Renderable *to_render = dq->copyRenderable();
	      // to_render->redoMatrices(e->er, f, w, h);
	      e->er->getDrawQueue().push_back(to_render);
	    }
	    
	    e->setWindowState(e->windowState()|Qt::WindowMaximized);
	    e->show();
	  });
}

void MainWindow::prepareResizeBtn(QVBoxLayout *toolbox_layout)
{
  QPushButton *btn = new QPushButton("Resize map");
  setSize(btn);

  toolbox_layout->addWidget(btn);

  connect(btn, &QPushButton::clicked,
	  [&]() {
	    // static cl_object fetchRegister = scm_c_lookup("root-fetchRegister");
	    // ec->chosenMap = scm_call_3(fetchRegister, ec->document, scm_from_locale_string("Map"), scm_from_locale_symbol(ec->indexOfChosenMap.c_str()));
	    // resize_data *o = new resize_data;
	    // o->setNew_width(currentMap->width());
	    // o->setNew_height(currentMap->height());
	    // Propertyeditor *p = new Propertyeditor(o, this);
	    // p->onClose = [=]() {
	    //   qDebug() << "Resizing to " << o->getNew_width() << ", " << o->getNew_height() << ", " << (o->getVertical_anchor() == TOP? "TOP": "BOTTOM") << ", " << (o->getHorizontal_anchor() == LEFT? "LEFT": "RIGHT");
	    //   currentMap->resize(o->getNew_width(), o->getNew_height(), o->getVertical_anchor(), o->getHorizontal_anchor());
	    //   delete o;
	    //   p->accept();
	    // };
	    // p->show();
	    puts("TODO implement");
	  });
}

MainWindow::MainWindow(int argc, char** argv) :  QMainWindow()
{
  // We need to start lisp subsystem before farting up an editorController
  t.load_lisp();
  puts("Lispit ladattu");
  ec = new editorController();
  
  ui.setupUi(this);
  ec->registerWindow(this);

  map_view = new Renderer(this);
  map_view->name = "MAP VIEW";
  ec->map_view = map_view;
  ec->populateMaps();
  
  tileset_view = new Renderer(this);
  tileset_view->name = "TILESET VIEW";
  ec->tilesetView = tileset_view;
  
  auto _tileview = new tileview_renderer(this);
  _tileview->name = "TILE VIEW";
  
  _tileview->setMinimumSize(50,50);
  _tileview->setMaximumSize(50,50);

  tileview = _tileview;
  ec->tileRenderer = tileview;

  tileset_view->mouseMoveEvents.push_back([=](QMouseEvent *e) {
      if(e->buttons() != Qt::LeftButton) return;

      int x = e->x() / 50, y = e->y() / 50;
      // QMessageBox::information(this, QString("Selected tile %1, %2").arg(x).arg(y), QString("Selected tile %1, %2").arg(x).arg(y));
      ec->setSelectedTile(x, y, tileset_view, _tileview);
    });

  map_view->mouseDownEvents.push_back([=](QMouseEvent *e) {
      puts("mouseDowning()");
      ec->t->mouseDown(e, ec);
      puts("mouseDowned");
    });

  map_view->mouseMoveEvents.push_back([=](QMouseEvent *e) {
      if(e->buttons() != Qt::LeftButton) return;

      int x = e->x() / 50, y = e->y() / 50;

      ec->t->doUse(e, x, y, ec);
      ec->t->registerDrag(x, y);
    });
  map_view->mouseUpEvents.push_back([=](QMouseEvent *E) {
      ec->t->mouseUp(E);
    });

  QWidget *tb = new QWidget(this);
  tb->setMaximumSize(200, INT_MAX);
  QVBoxLayout *toolbox_layout = new QVBoxLayout(this);
  toolbox_layout->setSpacing(0);

  toolbox_layout->addWidget(new QLabel(QString("Selected Tile"), this));
  toolbox_layout->addWidget(tileview);

  toolbox_layout->addWidget(toolbox());
  
  prepare_server_button(toolbox_layout);
  prepareResizeBtn(toolbox_layout);
  prepareStartEngine(toolbox_layout);

  setupTree();
  setupTreeCtxMenu();
  setupMainMenu();
  toolbox_layout->addWidget(&tree);

  QPushButton *close = new QPushButton("Close", this);
  connect(close, &QPushButton::clicked, this, &QMainWindow::close);
  toolbox_layout->addWidget(close);

  tb->setLayout(toolbox_layout);;

  Scroll *s = new Scroll(this);
  s->setWidget(map_view);

  Scroll *s2 = new Scroll(this);
  s2->setWidget(tileset_view);

  QSplitter *splitter = new QSplitter(Qt::Vertical, this);
  splitter->addWidget(s);
  splitter->addWidget(s2);

  ui.splitter->addWidget(tb);
  ui.splitter->addWidget(splitter);
}

MainWindow::~MainWindow()
{
} 

void MainWindow::registerController(editorController *ec)
{
  this->ec = ec;

  puts("Controller registered");
  ec->registerWindow(this);
}

void MainWindow::setupMainMenu()
{
  auto filemenu = menuBar()->addMenu("File");
  QAction *save = new QAction("Save project", this),
    *load = new QAction("Load project", this);
  connect(save, &QAction::triggered, [=]() {
      QString save_file = QFileDialog::getSaveFileName(this, "Save project file", ".", "QMapper projects (*.qmapper)");

      if(save_file == "") {
	qDebug() << "Got empty save_file_path";
	return;
      }
      
      if(!save_file.endsWith(".qmapper"))
	save_file = save_file + ".qmapper";
      qDebug() << "Trying to save to " << save_file;
      ec->saveTo(save_file);
    });

  connect(load, &QAction::triggered, [=]() {
      QString load_file = QFileDialog::getOpenFileName(this, "Load project file", ".", "QMapper projects (*.qmapper)");

      if(load_file == "") {
	qDebug() << "Got empty load_file_path";
	return;
      }
      
      if(!load_file.endsWith(".qmapper"))
	load_file = load_file + ".qmapper";
      qDebug() << "Trying to load from " << load_file;
      ec->loadFrom(load_file);
    });

  filemenu->addAction(save);
  filemenu->addAction(load);
}
