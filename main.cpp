#include <iostream>
#include "utilities.hpp"
int main() {
  
  std::string chosen_case = "polityczka";

  Returned_skin skinek = draw_skin(chosen_case);
  std::cout << "Skinek: " << skinek.skin_title << std::endl;
  std::cout << "Jakość: " << skinek.skin_quality << std::endl;
  std::cout << "Opis: " << skinek.skin_description << std::endl;
  std::cout << "Postscriptum: " << skinek.skin_ps << std::endl;
  std::cout << "Ścieżka do skina: " << skinek.skin_path << std::endl;
  std::cout << "Rodzaj broni: " << skinek.skin_weapon << std::endl;
  std::cout << "Float broni: " << skinek.skin_float << std::endl;
  std::cout << "Stat track: " << skinek.skin_stat_track << std::endl;
  std::cout << "Cena: " << skinek.skin_price << std::endl;
  
  int size;
  Returned_case* skrzynki = available_cases(size);
  std::cout << "Wybrana skrzynka: " << chosen_case << std::endl;

  /*for(int i = 0; i < size; i++){
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
  
  Returned_skin* dropniete_skinki = get_inventory(size);

  std::cout << "\nAktualny ekwipunek: \n"; 

  for(int i = 0; i < size; i++) {
    std::cout << "Dropnięty skinek numer " << i + 1 << ": " << dropniete_skinki[i].skin_title << std::endl;
    std::cout << "Opis skinka: " << dropniete_skinki[i].skin_description << std::endl;
    std::cout << "Postscriptum: " << dropniete_skinki[i].skin_ps << std::endl;
    std::cout << "Jakość: " << dropniete_skinki[i].skin_quality << std::endl;
    std::cout << "Rodzaj broni: " << dropniete_skinki[i].skin_weapon << std::endl;
    std::cout << "Ścieżka do skina: " << dropniete_skinki[i].skin_path << std::endl;
    std::cout << "Float broni: " << dropniete_skinki[i].skin_float << std::endl;
    std::cout << "Stat track: " << dropniete_skinki[i].skin_stat_track << std::endl;
  }
*/

  User_data uzytkownik = get_user_info();
  std::cout << "Nazwa uzytkownika: " << uzytkownik.username << std::endl;
  std::cout << "Ilość pieniędzy: " << uzytkownik.money << std::endl;

  change_user_data(-200);
  return 0;
}
