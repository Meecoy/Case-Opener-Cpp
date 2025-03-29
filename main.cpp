#include <iostream>
#include "utilities.hpp"
int main() {
  Returned_skin skinek = draw_skin("polityczka");

  std::cout << "Skinek: " << skinek.skin_title << std::endl;
  std::cout << "Jakość: " << skinek.skin_quality << std::endl;
  std::cout << "Opis: " << skinek.skin_description << std::endl;
  std::cout << "Post scriptum: " << skinek.skin_ps << std::endl;
  std::cout << "Ścieżka do skina: " << skinek.skin_path << std::endl;
  std::cout << "Rodzaj broni: " << skinek.skin_weapon << std::endl;
  std::cout << "Float broni: " << skinek.skin_float << std::endl;
  std::cout << "Stat track: " << skinek.skin_stat_track << std::endl;
  
  return 0;
}
