#ifndef DRAW_SKIN_HPP
#define DRAW_SKIN_HPP

#include <iostream>

struct Returned_skin {
    std::string skin_title;
    std::string skin_description;
    std::string skin_ps;
    std::string skin_quality;
    std::string skin_weapon;
    std::string skin_path;
    std::string skin_float;
    bool skin_stat_track;
};

struct Returned_case {
  std::string case_title;
  std::string case_path;
};

Returned_skin draw_skin(const std::string& collection);

Returned_case* available_cases(int& arr_size);

Returned_skin* selected_case_skins(int& arr_size, std::string collection);

void write_to_inventory(Returned_skin skin);

#endif

