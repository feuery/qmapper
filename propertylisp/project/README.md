# Propertier/property lisp

Hi, this is my common lisp server that reads lisp from one folder and outputs cpp and header files to other.

## Introduction (or: wtf, why?)

1. Common Lisp is actually really cool and I needed an excuse to get familiarish with it and SLIME
2. For QMapper I needed objects that know their own fields. This is impossible with pure C++. You could write this by hand for every "domain" class:
```
public: 
  tile() {
    r[0] = "rotation";
    r[1] = "tile";
    r[2] = "y";
    r[3] = "x";
  }
  const char* type_name(const char* propertyname) {
    if (strcmp(propertyname, "rotation") == 0) return "int ";
    if (strcmp(propertyname, "tile") == 0) return "int ";
    if (strcmp(propertyname, "y") == 0) return "int ";
    if (strcmp(propertyname, "x") == 0) return "int ";
    return "";
  }
  const char** names() { return r; }
  int property_count = 4;
  template<typename T>
  void set(const char* propertyname, T value) { 
    if(strcmp(propertyname, "rotation") == 0) rotation_field = value;
    if(strcmp(propertyname, "tile") == 0) tile_field = value;
    if(strcmp(propertyname, "y") == 0) y_field = value;
    if(strcmp(propertyname, "x") == 0) x_field = value;
  }
  template<typename T>
  T get(const char* propertyname, bool* success) {
    if(strcmp(propertyname, "rotation") == 0) {
      *success = true; 
      return rotation_field; 
    }
    if(strcmp(propertyname, "tile") == 0) {
      *success = true; 
      return tile_field; 
    }
    if(strcmp(propertyname, "y") == 0) {
      *success = true; 
      return y_field; 
    }
    if(strcmp(propertyname, "x") == 0) {
      *success = true; 
      return x_field; 
    }
    /////
    T t;
    *success = false; 
    return t; 
  }
  /////
  const char* r[4];
```
and when you started refactoring, you'd be screwed because c++ lacks both a not-awful object model and not-awful macros.

Or instead, you could use a pre-processor with humane macro facilities, which creates the final cpp classes with their boilerplate reflection interfaces:

```
(defcppclass Tile ("<string>" "<iostream>")   
  (inherits common_parent_class)
  (public
   (properties
    (int x 0)
    (int y 0)
    (int tile 0)
    (int rotation 0))
   (functions
    (void render (QOpenGLFunctions_4_3_Core* f))))
  (protected
   (fields
    (std__string name "A new tile"))))
```
which expands in full the following header file:
```
#ifndef tile
#define tile
//// generated at #<date-time 2017-06-14 18:23:27.281 {10029C10D3}>
#include<cstring>
#include<string>
#include<iostream>
class tile 
: public common_parent_class
{
 public: 
  int  getX();

  void setX(int  val);


  int  x_field = 0;
  int  getY();

  void setY(int  val);


  int  y_field = 0;
  int  getTile();

  void setTile(int  val);


  int  tile_field = 0;
  int  getRotation();

  void setRotation(int  val);


  int  rotation_field = 0;
  virtual void render(qopenglfunctions_4_3_core* f) = 0;
 protected: 
  std::string  name = "A new tile";
public: 
  tile() {
    r[0] = "rotation";
    r[1] = "tile";
    r[2] = "y";
    r[3] = "x";
  }
  const char* type_name(const char* propertyname) {
    if (strcmp(propertyname, "rotation") == 0) return "int ";
    if (strcmp(propertyname, "tile") == 0) return "int ";
    if (strcmp(propertyname, "y") == 0) return "int ";
    if (strcmp(propertyname, "x") == 0) return "int ";
    return "";
  }
  const char** names() { return r; }
  int property_count = 4;
  template<typename T>
  void set(const char* propertyname, T value) { 
    if(strcmp(propertyname, "rotation") == 0) rotation_field = value;
    if(strcmp(propertyname, "tile") == 0) tile_field = value;
    if(strcmp(propertyname, "y") == 0) y_field = value;
    if(strcmp(propertyname, "x") == 0) x_field = value;
  }
  template<typename T>
  T get(const char* propertyname, bool* success) {
    if(strcmp(propertyname, "rotation") == 0) {
      *success = true; 
      return rotation_field; 
    }
    if(strcmp(propertyname, "tile") == 0) {
      *success = true; 
      return tile_field; 
    }
    if(strcmp(propertyname, "y") == 0) {
      *success = true; 
      return y_field; 
    }
    if(strcmp(propertyname, "x") == 0) {
      *success = true; 
      return x_field; 
    }
    /////
    T t;
    *success = false; 
    return t; 
  }
  /////
  const char* r[4];
};
#endif
```
and this source file:
```
#include <tile.h>
///////// CPP FILE STARTS HERE, generated at #<date-time 2017-06-14 18:23:31.282 {1002D6ACE3}>
int  tile::getRotation() {
 return rotation_field; 
};
void tile::setRotation(int  val) {
 rotation_field = val; 
}
int  tile::getTile() {
 return tile_field; 
};
void tile::setTile(int  val) {
 tile_field = val; 
}
int  tile::getY() {
 return y_field; 
};
void tile::setY(int  val) {
 y_field = val; 
}
int  tile::getX() {
 return x_field; 
};
void tile::setX(int  val) {
 x_field = val; 
}

```
## How to install
These instructions are know to work only on my arch box. In mac world the asdf registry is somewhere different, and in windows world :cl-fsnotify is broken, so I'm sorry if you're building these anywhere else than linux. 

Currently I'm not able to provide stable binaries, se it's best to build this from source. Install sbcl and [quicklisp](https://www.quicklisp.org/beta/). Then quicklisp:quickload the following common lisp libraries:
- :cl-arrows
- :cl-fsnotify 
- :lparallel 
- :simple-date-time 
- :cl-ppcre

Then close the REPL. Next git clone this repo somewhere. Move to the just-cloned directory and run
```
sudo ln -s $(pwd) /usr/share/common-lisp/source/propertier-testi
```
Then start the repl again. Load propertier-system by invoking (asdf:load-system "propertier") in repl. When building, call next (propertier:build!), which dumps sbcl's state into an executable image in path ./image. When developing, set \*input-dir\* and \*output-dir\* to real directories, call (main) and input test data to the dir marked by \*input-dir\*.

## How to use
I assume you've built the image from the sources. Image is callable from shell, and takes two parameters --input-dir where you put the lisp files that need compiling and --output-dir where the resulting cpp files will be put. Output directory should contain only output spit out by this tool to make cleaning them easier. Inputfiles need to have the *.def - file extension and follow these rules:

## The language
There's an example in the [repo](https://github.com/feuery/qmapper/blob/master/propertylisp/project/tile.def). The general grammar is this:

```
(defcppclass <class-name> ("<iostream>" "<other-include-files>...")
  (inherits std__string std__whatever)
  (public
   (properties
    (int x 0)
    (int y 0)
    (int tile 0)
    (int rotation 0))
   (functions
    (void render (QOpenGLFunctions_4_3_Core* f) (other param type PARAM_NAME)))
   (fields
    (std__string name "default value")))
  (protected
   ...the things valid inside public blocks are valid here too)
  (private 
   ...the things valid inside public blocks are valid here too))
```
in theory you could run both pure and side-effecting lisp forms inside the defcppclass form, but I wouldn't advice to, as the quoting the server does on input defcppclass forms is a bit insane and subject to change. Functions-macro generates pure virtual functions (virtual type fn_name() = 0;), so if you use it, you'll have to inherit your class and implement those functions with real c++. Properties generate the propname\_field, setter and getter, and their types + names are added to the reflection functions. Fields are just simple fields.

Two underscores in the type symbols are changed to ::. The reason you can't use them is that lisp's packages leak through if you use :: in your own symbols, which confuses the reader.

## Rough edges
- THIS ISN'T TESTED IN A REAL C++ PROJECT

Yet. It will be, though. As soon as I get this documentation task out of the way.

- cl-fsnotify or kqueue or something in-between does something stupid on mac. So this project works there in theory, but it might be buggy
- cl-fsnotify doesn't support windows, so neither will I
- There's practically no error handling currently. If you get to error repl, it's easiest to just restart the server
- sbcl is a bit dumb and doesn't die on ^C or ^D. The correct way kill the image is command (sb-ext:exit)
- or if you're not in the error repl, ^C and then (sb-ext:exit)

## if it breaks
- If there's an error thrown, (sb-ext:exit) or ^C (sb-ext:exit) and restart should fix the problem
- If the server starts generating invalid/rubbish c++, invoke `rm *.cpp *.h` in the output-dir and restart the server.
