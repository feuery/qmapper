#include <gl_apu.h>
#include <SOIL/SOIL.h>


GLuint generateRectangle (QOpenGLFunctions_4_3_Core *f, int text_w, int text_h, int screen_w, int screen_h)
{
  qDebug()<< "Width is " << (float)text_w << "/" << (float)screen_w << ":" << (float)text_w/(float)screen_w;
  qDebug()<< "Height is " << (float)text_h << "/" << (float)screen_h << ":" << (float)text_h/(float)screen_h;

  float w = 2 * (float)text_w/(float)screen_w,
    h = (float)text_h/(float)screen_h;
  
  
  GLfloat vertices[] = {
    // Positions                                  // Colors           // Texture Coords
    (-1.0f + w), 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // Top Right
    (-1.0f + w), (1.0f - h), 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // Bottom Right
    -1.0f, (1.0f - h), 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // Bottom Left
    -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f  // Top Left 
    };

  GLuint indices[] = {  // Note that we start from 0!
    0, 1, 3,   // First Triangle
    1, 2, 3    // Second Triangle
  };
  GLuint EBO, VBO, VAO;
  f->glGenBuffers(1, &EBO);
  
  f->glGenBuffers(1, &VBO);
  
  f->glGenVertexArrays(1, &VAO);

  f->glBindVertexArray(VAO); {
    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Positions
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    f->glEnableVertexAttribArray(0);

    //Color
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    f->glEnableVertexAttribArray(1);

    // Text coords
    f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    f->glEnableVertexAttribArray(2);
  }
  f->glBindVertexArray(0);

  return VAO;
}

bool operator==(Point const& l, Point const& r)
{
  return l.x == r.x && l.y == r.y;
}
bool operator!=(Point const& l, Point const& r)
{
  return !(l == r);
}
bool operator==(Point3D const& l, Point3D const& r)
{
  return l.x == r.x && l.y == r.y && l.z == r.z;
}
bool operator!=(Point3D const& l, Point3D const& r)
{
  return !(l == r);
}

// inline QDebug operator<<(QDebug g, Point3D const& e)

GLuint loadTexture(QOpenGLFunctions_4_3_Core *f, const char* filename, int *text_w, int *text_h) {
  GLuint texture = 0;
  if(!f) {
    qDebug() << "QOpenGLFunctions is null";
    throw "";
  }
  if(strcmp(filename, "") == 0) {
    qDebug() << "Not loading texture's from an empty string";
    return -1;
  }

  
  f->glGenTextures(1, &texture);
  f->glBindTexture(GL_TEXTURE_2D, texture);

  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned char* img = SOIL_load_image(filename, text_w, text_h, 0, SOIL_LOAD_AUTO);
  if(img) {
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *text_w, *text_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    f->glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(img);
    f->glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
  }

  return -1;
}
