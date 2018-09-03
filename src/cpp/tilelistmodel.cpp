#include <QDebug>
#include <tilelistmodel.h>
#include <iostream>
#include <guile_fn.h>
#include <editorController.h>

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

static std::unordered_map<void*, cl_object> keeper;
static 
void doRegister(void* ptr, cl_object o)
{
  auto type_of = makefn("q-Type-of");
  // printf("Registering to %d\n", ptr);
  auto s = ecl_string_to_string(cl_funcall(2, type_of, o));
  // printf("Type is %s\n", s.c_str());
  keeper[ptr] = o; 
}

static
cl_object getRegister(void* ptr)
{
  try{
    // auto type_of = makefn("q-type-of");
    // printf("Fetching from %d\n", p);
    cl_object o = keeper.at(ptr);
  
    // auto s = ecl_string_to_string(cl_funcall(2, type_of, o));
    // printf("Type is %s\n", s.c_str());  

    // keeper.erase(p);
  
    return o;
  }
  catch(std::out_of_range ofr) {
    std::cout << "Out of range with " << ptr << " - keys are: " << std::endl;
    for(auto i: keeper) {
      std::cout << "\t" << i.first << std::endl;
    }
    throw ofr;
  }
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
    log_deref;
    auto type_of = makefn("q-type-Of");
    cl_object toret = getRegister(parent.internalPointer());
    return toret;
  }
  else {
    log_deref;
    // if(parent.isValid()) {
    //   if(!(ptr < INT2VOIDP(int_cache))) puts("Case 2 fails");
    //   std::cout<<"ptr " << ptr << " is rubbish\n";
    //   std::cout<<"int_cache's size: " << int_cache << std::endl;
    //   throw "";
    // }
    // else 
    //   puts("Parent isn't valid. Let's return root-doc");
    return Root.getValue();
  }
}

Tilelistmodel::Tilelistmodel(holder R): Root(R) { }

#define type_err_print printf("Got an unknown type %s at line %d\n", type, __LINE__);

int Tilelistmodel::rowCount(const QModelIndex &qparent) const
{
  cl_object value = deref_index(qparent),
    prin = makefn("prin1"),
    type_of = makefn("Q-type-of"),
    length = makefn("length"),
    layers_fn = makefn("map-layers"),
    registrySize = makefn("root-registrysize");
  QModelIndex parent = qparent.parent();
  if(value == nullptr) {
    throw "";
  }
  
  std::string s = ecl_string_to_string(cl_funcall(2, type_of, value));
  const char* type = s.c_str();

  // cl_funcall(2, prin, value);
  // printf("Returning rows for %s\n", type);
  if(strcmp(type, "MAP") == 0) {
    cl_object layers = cl_funcall(2, layers_fn, value);
    // cl_funcall(2, prin, layers);
    int rc = ecl_to_int32_t(cl_funcall(2, length, layers));
    // printf("Map's Rowcount %d\n", rc);
    return rc;
  }
  else if (strcmp(type, "ROOT") == 0) {
    int rc = ecl_to_int32_t(cl_funcall(2, registrySize, value));
    // printf("Rowcount %d\n", rc);
    return rc;
  }
  else if(strcmp(type, "LAYER") == 0 ||
  	  strcmp(type, "SCRIPT") == 0 ||
  	  strcmp(type, "SPRITE") == 0 ||
  	  strcmp(type, "TILESET") == 0 ||
  	  strcmp(type, "ANIMATEDSPRITE") == 0) {
    // puts("Rowcount 0");
    return 0;
  }
  else {
    type_err_print;
    throw "";
  }
  puts("Rowcount nil 0");
  return 0;
}

int Tilelistmodel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant Tilelistmodel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
    return QVariant();
  }

  return QString("Field");
}

QModelIndex Tilelistmodel::index(int row, int column, const QModelIndex &qparent) const
{
  puts("Let's go to ::index");
  cl_object base = deref_index(qparent),
    nth = makefn("nth"),
    type_of = makefn("q-tYpe-of"),
    mLayers = makefn("map-layers"),
    reg_to_list = makefn("root-registryToList"),
    prin = makefn("prin1");

  std::string type = ecl_string_to_string(cl_funcall(2, type_of, base));
  // printf("Making index for type %s\n", type.c_str());
  
  if(type == "MAP") {
    cl_object m = base,
      layers = cl_funcall(2, mLayers, m),
      l = cl_funcall(3, nth, ecl_make_int32_t(row),
  		     layers);

    puts("Layer:");
    // cl_funcall(2, prin, layers);
    cl_funcall(2, prin, l);
    puts("");
    
    void* ptr = INT2VOIDP(getInt());
    doRegister(ptr, l);
    // puts("Making index of layer");
    return createIndex(row, column, ptr);
  }
  else if(type == "ROOT") {
    auto reg = cl_funcall(2, reg_to_list, Root.getValue());

    cl_object o = cl_funcall(3, nth, ecl_make_int32_t(row), reg);

    puts("Typeoffing under ROOT");
    auto s = ecl_string_to_string(cl_funcall(2, type_of, o));
    // printf("Making index to type %s\n", s.c_str());

    void* ptr = INT2VOIDP(getInt());
    doRegister(ptr, o);

    return createIndex(row, column, ptr);
  }
  else {
    // printf("Unknown type %s\n", type);
    throw "";
  }
  
  return QModelIndex();
}

QVariant Tilelistmodel::data(const QModelIndex &index, int role) const
{
  if(!index.isValid()) return QVariant();
  if(role != Qt::DisplayRole) return QVariant();

  puts("Going into ::data");

  auto type_of = makefn("q-tyPe-of");
  
  cl_object base = deref_index(index);
  std::string type = ecl_string_to_string(cl_funcall(2, type_of, base));
  int row = index.row();

  if(type == "ROOT") return QString("Root");
  else {
    cl_object name = get(base, "name");

    if(name != ECL_NIL) {
      auto string = ecl_string_to_string(name);
      if(string == "") return QString("Lolwat, ^P?");
    
      // printf("::data => %s\n", string.c_str());

      return QString(string.c_str());
    }
    return QString("NO NAME FOUND");
  }
}

QModelIndex Tilelistmodel::parent(const QModelIndex &index) const
{
  if(!index.isValid()) return QModelIndex();

  puts("Going into ::parent");
  cl_object type_of = makefn("q-typE-of");
  
  cl_object obj = deref_index(index),
    reg_to_list = makefn("root-registryToList");
  if(obj == Root.getValue()) return QModelIndex();

  std::string type = ecl_string_to_string(cl_funcall(2, type_of, obj));
  // printf("Searching for parent to type %s\n", type.c_str());
  auto reg = cl_funcall(2, reg_to_list, Root.getValue());
  if(type == "MAP") {
    int row = indexOf(reg, obj);
    // printf("Returning index to parent root, row %d\n", row);
    void* ptr = INT2VOIDP(getInt());
    auto o = Root.getValue();
    doRegister(ptr, o);
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
    printf("Returning index to parent map %d\n", row);

    void* ptr = INT2VOIDP(getInt());
    doRegister(ptr, parent);
    
    return createIndex(row, 0, ptr);
  }
  else if (type == "SCRIPT") {
    auto o = Root.getValue();
    void* ptr = INT2VOIDP(getInt());
    doRegister(ptr, o);
    cl_object s = obj;
    return createIndex(indexOf(reg, s), 0, ptr);
  }
  else if(type == "TILESET" ) {
    auto o = Root.getValue();
    void* ptr = INT2VOIDP(getInt());
    doRegister(ptr, o);
    auto t = obj;
    return createIndex(indexOf(reg, t), 0, ptr);
  }
  else if(type == "SPRITE") {
    auto o = Root.getValue();
    void* ptr = INT2VOIDP(getInt());
    doRegister(ptr, o);
    auto spr = obj;
    return createIndex(indexOf(reg, spr), 0, ptr);
  }
  else if(type == "ANIMATEDSPRITE") {
    auto o = Root.getValue();
    void* ptr = INT2VOIDP(getInt());
    doRegister(ptr, o);
    return createIndex(indexOf(reg, obj), 0, ptr);
  }
  else {
    printf("Got an unknown type at Tilelistmodel::parent(): %s\n", type.c_str());
    throw "";
  }
  return QModelIndex();
}

void Tilelistmodel::end()
{
  endInsertRows();
}


void Tilelistmodel::begin()
{
  static cl_object regSize = makefn("root-registrySize");
  auto root = QModelIndex();
  int count = fixint(cl_funcall(2, regSize, Root.getValue()));
  beginInsertRows(root, count, count + 1);
}

int indexOf(cl_object vec, cl_object element)
{
  cl_object position = makefn("position");
  return fixint(cl_funcall(3, position, element, vec));
}
