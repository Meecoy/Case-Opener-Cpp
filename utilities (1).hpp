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
    unsigned int skin_price;
};

struct Returned_case {
  std::string case_title;
  std::string case_path;
};

struct User_data {
  std::string username;
  unsigned int money;
}; 

Returned_skin draw_skin(const std::string& collection, int luck = 0);

Returned_case* available_cases(int& arr_size);

Returned_skin* selected_case_skins(int& arr_size, const std::string& collection);

void write_to_inventory(Returned_skin skin);

Returned_skin* get_inventory(int& arr_size);

User_data get_user_info();

void change_user_data(int money = 0, const std::string& username = "");

void sell_skin(int id);

class Case_bot {
private:
  std::string bot_name;
  int luck;
public:
  Case_bot(const std::string& name, int luck_factor);

  void set_bot_name(const std::string& name);
  void set_luck(int luck_factor);

  std::string get_bot_name() const;
  int get_luck() const;

  bool Fight(const std::string& collection, int number_of_rolls);
};

#endif

