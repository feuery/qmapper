#include <QtDebug>
#include <tilelistmodel.h>
#include <iostream>
#include <guile_fn.h>
#include <editorController.h>

// if only there were advice or non-stupid macros or smthing in cpp

#define START_LIST_METHOD  setCallLogs(false);
#define STOP_LIST_METHOD setCallLogs(true);

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

static std::unordered_map<cl_object, void*> keeper;
static 
void doRegister(void** ptr, cl_object o, holder Root)
{
  auto type_of = makefn("q-Type-of");
  auto s = ecl_string_to_string(cl_funcall(2, type_of, o));
  if(keeper.find(o) == keeper.end()) {
    if(o == Root.getValue()) {
      printf("Registering root to %d\n", *ptr);
    }
    static auto equalp = makefn("equalp");
    if(cl_funcall(3, equalp, o, Root.getValue()) == ECL_T) {
      printf("Really Registering root to %d\n", *ptr);
    }
    keeper[o] = *ptr;
  }
  else {
    *ptr = keeper.at(o);
  }
}

static
cl_object getRegister(void* ptr)
{
  // try{
  //   // auto type_of = makefn("q-type-of");
  //   cl_object o = keeper.at(ptr);
  
  //   // auto s = ecl_string_to_string(cl_funcall(2, type_of, o));

  //   // keeper.erase(p);
  
  //   return o;
  // }
  // catch(std::out_of_range ofr) {
  //   std::cout << "Out of range with " << ptr << " - keys are: " << std::endl;
  //   for(auto i: keeper) {
  //     std::cout << "\t" << i.first << std::endl;
  //   }
  //   throw ofr;
  // }

  for(auto it: keeper) {
    if(it.second == ptr) return it.first;
  }

  return ECL_NIL;
}

int int_cache;

int getInt() {
  return int_cache++;
}

#define log_deref std::cout<< (parent.isValid() ? "Parent is valid": "Paren't isn't valid") << " and ptr is " << ptr << " whereas int_cache is " << int_cache << std::endl;

cl_object deref_index(const QModelIndex &parent) {
  auto Root = editorController::instance->document;
  void* ptr = parent.internalPointer();
  if(parent.isValid() && ptr < INT2VOIDP(int_cache)) {
    // auto type_of = makefn("q-type-Of");
    cl_object toret = getRegister(parent.internalPointer());
    
    return toret;
  }
  else {

    // log_deref;
    return Root.getValue();
    // return ECL_NIL;
  }
}

Tilelistmodel::Tilelistmodel(holder R): Root(R) {
  START_LIST_METHOD;

  STOP_LIST_METHOD;
}

#define type_err_print printf("Got an unknown type %s at line %d\n", type, __LINE__);

int Tilelistmodel::rowCount(const QModelIndex &qparent) const
{
  START_LIST_METHOD;
  
  cl_object value = deref_index(qparent),
    prin = makefn("prin1"),
    type_of = makefn("Q-type-of"),
    length = makefn("length"),
    layers_fn = makefn("map-layers"),
    registrySize = makefn("root-registrysize");

  if(value == ECL_NIL) {
    value = Root.getValue();
  }
  
  QModelIndex parent = qparent.parent();
  if(value == nullptr) {
    throw "";
  }
  
  std::string s = ecl_string_to_string(cl_funcall(2, type_of, value));
  const char* type = s.c_str();

  if(strcmp(type, "MAP") == 0) {
    cl_object layers = cl_funcall(2, layers_fn, value);
    int rc = ecl_to_int32_t(cl_funcall(2, length, layers));
    return rc;
  }
  else if (strcmp(type, "ROOT") == 0) {
    int rc = ecl_to_int32_t(cl_funcall(2, registrySize, value));
    // printf("Root has %d children\n", rc);
    return rc;
  }
  else if(strcmp(type, "LAYER") == 0 ||
  	  strcmp(type, "SCRIPT") == 0 ||
  	  strcmp(type, "SPRITE") == 0 ||
  	  strcmp(type, "TILESET") == 0 ||
  	  strcmp(type, "ANIMATEDSPRITE") == 0) {
    return 0;
  }
  else {
    type_err_print;
    throw "";
  }
  // puts("Rowcount nil 0");
  return 0;

  STOP_LIST_METHOD;
}

int Tilelistmodel::columnCount(const QModelIndex &parent) const
{
  START_LIST_METHOD;
  
  return 1;

  STOP_LIST_METHOD;
}

QVariant Tilelistmodel::headerData(int section, Qt::Orientation orientation, int role) const
{
  START_LIST_METHOD;
  
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
    return QVariant();
  }

  return QString("Field");

  STOP_LIST_METHOD;
}

QModelIndex Tilelistmodel::index(int row, int column, const QModelIndex &qparent) const
{
  START_LIST_METHOD;
  
  cl_object base = deref_index(qparent),
    nth = makefn("nth"),
    type_of = makefn("q-tYpe-of"),
    mLayers = makefn("map-layers"),
    reg_to_list = makefn("root-registryToList"),
    prin = makefn("prin1");

  if(base == ECL_NIL) {
    puts("base is nil");
    base = Root.getValue();
  }
  

  // printf("::index row %d, column %d, valid? %s\n", row, column, qparent.isValid()? "IS": "ISN'T");

  // if(row == 0 && column == 0 && qparent.isValid())
    // puts("Breakpoint place!");

  if(!hasIndex(row, column, qparent)) {
    // puts("Returning invalid qmodelindex #0");
    return QModelIndex();
  }

  if(row < 0) {
    // puts("Returning invalid qmodelindex #1");
    return QModelIndex();
  }

  std::string type = ecl_string_to_string(cl_funcall(2, type_of, base));
  
  if(type == "MAP") {
    cl_object m = base,
      layers = cl_funcall(2, mLayers, m),
      l = cl_funcall(3, nth, ecl_make_int32_t(row),
  		     layers);

    void* ptr = INT2VOIDP(getInt());
    doRegister(&ptr, l, Root);
    // puts("Returning layer");
    return createIndex(row, column, ptr);
  }
  else if(type == "ROOT") {
    // tän pitäis varmaan tarkistaa onko meillä jo mappia rekisteröitynä
    auto reg = cl_funcall(2, reg_to_list, Root.getValue());

    cl_object o = cl_funcall(3, nth, ecl_make_int32_t(row), reg);

    auto s = ecl_string_to_string(cl_funcall(2, type_of, o));

    void* ptr = INT2VOIDP(getInt());
    doRegister(&ptr, o, Root);

    // puts("Returning a child of root");

    return createIndex(row, column, ptr);
  }
  else {
    throw "";
  }

  // puts("Returning invalid qmodelindex #2");  
  return QModelIndex();

  STOP_LIST_METHOD;
}

QVariant Tilelistmodel::data(const QModelIndex &index, int role) const
{
  START_LIST_METHOD;
  
  if(!index.isValid()) return QVariant();
  if(role != Qt::DisplayRole) return QVariant();

  auto type_of = makefn("q-tyPe-of");
  
  cl_object base = deref_index(index);

  if(base == ECL_NIL) {
    puts("base is nil");
    throw "";
  }
  
  std::string type = ecl_string_to_string(cl_funcall(2, type_of, base));
  int row = index.row();
  if(row < 0) return QVariant();

  if(type == "ROOT") return QString("Root");
  else {
    cl_object name = get(base, "name");

    if(name != ECL_NIL) {
      auto string = ecl_string_to_string(name);
      if(string == "") return QString("Lolwat, ^P?");
    

      return QString(string.c_str());
    }
    return QString("NO NAME FOUND");
  }

  STOP_LIST_METHOD;
}

QModelIndex Tilelistmodel::parent(const QModelIndex &index) const
{
  START_LIST_METHOD;

  // puts("coming to ::parent");
	
  qDebug() << "::parent Index is " << index;

  // pitäiskö täs dereffata index.parent?

  if(deref_index(index) == Root.getValue()) {
    // puts("It's parent!");
    return QModelIndex();
  }
  
  if(!index.isValid()) {
    // puts("::parent index is invalid");
    return QModelIndex();
  }

  // puts("Index is something sane");
  // printf("r %d, c %d, valid %s\n", index.row(), index.column(), index.isValid()? "T": "NIL");

  if(index.internalPointer() == 0) return QModelIndex();

  if(index.row() == 0 && index.column() == 0 && index.isValid()) {
    qDebug() << "Everything's 0";
    return createIndex(0, 0, nullptr);
  }

  cl_object type_of = makefn("q-typE-of");
  
  cl_object obj = deref_index(index),
    reg_to_list = makefn("root-registryToList");

  if(obj == ECL_NIL) {
    puts("obj is nil");
    throw "";
  }
  
  if(obj == ECL_NIL) return QModelIndex();
  if(obj == Root.getValue()) return QModelIndex();

  std::string type = ecl_string_to_string(cl_funcall(2, type_of, obj));
  printf("type %s\n", type.c_str());
  auto reg = cl_funcall(2, reg_to_list, Root.getValue());

  if(type == "MAP") {
    int row = indexOf(reg, obj);
    if(row < 0) return QModelIndex();
    void* ptr = INT2VOIDP(getInt());
    auto o = Root.getValue();
    doRegister(&ptr, o, Root);

    return createIndex(row, 0, ptr);
  }
  else if(type == "LAYER") {
    cl_object find_layer_parent = makefn("find-layer-parent");
    
    cl_object getLayers = makefn("map-layers");
    cl_object l = obj,
      parent = cl_funcall(3, find_layer_parent, l, Root.getValue());

    if(parent == ECL_NIL) {
      qDebug() << "Layer has invalid parent";
      return QModelIndex();
    }
    int row = indexOf(cl_funcall(2, getLayers, parent), l);
    if(row < 0) return QModelIndex();

    void* ptr = INT2VOIDP(getInt());
    doRegister(&ptr, parent, Root);
    
    return createIndex(row, 0, ptr);
  }
  else if (type == "SCRIPT") {
    auto o = Root.getValue();
    void* ptr = INT2VOIDP(getInt());
    doRegister(&ptr, o, Root);
    cl_object s = obj;
    return createIndex(indexOf(reg, s), 0, ptr);
  }
  else if(type == "TILESET" ) {
    auto o = Root.getValue();
    void* ptr = INT2VOIDP(getInt());
    doRegister(&ptr, o, Root);
    auto t = obj;
    return createIndex(indexOf(reg, t), 0, ptr);
  }
  else if(type == "SPRITE") {
    auto o = Root.getValue();
    void* ptr = INT2VOIDP(getInt());
    doRegister(&ptr, o, Root);
    auto spr = obj;
    return createIndex(indexOf(reg, spr), 0, ptr);
  }
  else if(type == "ANIMATEDSPRITE") {
    auto o = Root.getValue();
    void* ptr = INT2VOIDP(getInt());
    doRegister(&ptr, o, Root);
    return createIndex(indexOf(reg, obj), 0, ptr);
  }
  else {
    throw "";
  }

  qDebug() << "Lollero";
  return QModelIndex();

  STOP_LIST_METHOD;
}

void Tilelistmodel::end()
{
  START_LIST_METHOD;
  
  endInsertRows();

  STOP_LIST_METHOD;
}


void Tilelistmodel::begin()
{
  START_LIST_METHOD;
  
  static cl_object regSize = makefn("root-registrySize");
  auto root = QModelIndex();
  int count = fixint(cl_funcall(2, regSize, Root.getValue()));
  beginInsertRows(root, count, count + 1);

  STOP_LIST_METHOD;
}

int indexOf(cl_object vec, cl_object element)
{
  cl_object position = makefn("position");
  return fixint(cl_funcall(3, position, element, vec));
}


bool Tilelistmodel::hasChildren(const QModelIndex &parent) const {
  bool result = rowCount(parent) > 0;
  // printf("::hasChildren? r %d, c %d, %s? %s\n", parent.row(), parent.column(), parent.isValid()? "VALID": "INVALID", result? "HAS": "DOESN'T HAS");
  return result;
}
