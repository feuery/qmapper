(defcppclass Tileset
  (public
   (properties
    (std__string name "")
    (Script* vertexShader nullptr)
    (Script* fragmentShader nullptr)
    (std__vector<std__vector<Tile*>>* tiles '()))))

(scm-puts "Loaded tileset")
