#include <resize_obj.h>
////// generated at 2018-01-31T14:08:33.513Z


void resize_data::setNew_width(int value) { 
New_width_field = value;
}
                                                        int resize_data::getNew_width() {
return New_width_field;
}
void resize_data::setNew_height(int value) { 
New_height_field = value;
}
                                                        int resize_data::getNew_height() {
return New_height_field;
}
void resize_data::setVertical_anchor(verticalAnchor value) { 
Vertical_anchor_field = value;
}
                                                        verticalAnchor resize_data::getVertical_anchor() {
return Vertical_anchor_field;
}
void resize_data::setHorizontal_anchor(horizontalAnchor value) { 
Horizontal_anchor_field = value;
}
                                                        horizontalAnchor resize_data::getHorizontal_anchor() {
return Horizontal_anchor_field;
}
resize_data::resize_data() {
r.push_back(flyweight<std::string>(std::string("Id")));
r.push_back(flyweight<std::string>(std::string("new_width")));
r.push_back(flyweight<std::string>(std::string("new_height")));
r.push_back(flyweight<std::string>(std::string("vertical_anchor")));
r.push_back(flyweight<std::string>(std::string("horizontal_anchor")));
}resize_data* toResize_data(Propertierbase *b)
 {
if(b->type_identifier() == std::string("resize_data")) {
  return static_cast<resize_data*>(b);
}
else {
printf("\"toResize_data called with \"%s\"\n", b->type_identifier().get().c_str());
throw "";
}
}
