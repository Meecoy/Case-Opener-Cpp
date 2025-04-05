#include <iostream>
#include "utilities.hpp"
int main() {
  
  std::string chosen_case = "polityczka";

  Returned_skin skinek = draw_skin(chosen_case);
  std::cout << "Skinek: " << skinek.skin_title << std::endl;
  std::cout << "Jakość: " << skinek.skin_quality << std::endl;
  std::cout << "Opis: " << skinek.skin_description << std::endl;
  std::cout << "Post scriptum: " << skinek.skin_ps << std::endl;
  std::cout << "Ścieżka do skina: " << skinek.skin_path << std::endl;
  std::cout << "Rodzaj broni: " << skinek.skin_weapon << std::endl;
  std::cout << "Float broni: " << skinek.skin_float << std::endl;
  std::cout << "Stat track: " << skinek.skin_stat_track << std::endl;
  
  int size;
  Returned_case* skrzynki = available_cases(size);
  std::cout << "Wybrana skrzynka: " << chosen_case << std::endl;

  for(int i = 0; i < size; i++){
    std::cout << "Skrzynka: " << skrzynki[i].case_title << "\nObrazek skrzynki: " << skrzynki[i].case_path << std::endl;
  }

  delete [] skrzynki;
  
  Returned_skin* skinki_w_skrzynce = selected_case_skins(size, chosen_case);

  for(int i = 0; i < size; i++) {
    std::cout << "Skin numer " << i + 1 << ": " << skinki_w_skrzynce[i].skin_title << std::endl;
    std::cout << "Rzadkość: " << skinki_w_skrzynce[i].skin_quality << std::endl;
    std::cout << "Broń: " << skinki_w_skrzynce[i].skin_weapon << std::endl;
    std::cout << "Ścieżka do skina: " << skinki_w_skrzynce[i].skin_path << std::endl;
  }

  delete [] skinki_w_skrzynce;

  write_to_inventory(skinek); 

  return 0;
}
