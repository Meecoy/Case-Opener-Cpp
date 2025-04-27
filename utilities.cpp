#include <iostream>
#include <simdjson.h>
#include <random>
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include "utilities.hpp"

// 1. Roll skin from selected case
Returned_skin draw_skin(const std::string& collection, int luck) {
  Returned_skin Skin;

  static std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, 1000);
  int random_int = dist(gen);

  struct {
    unsigned int value;
    const char *quality;
  } qualities[] = {
    { 0, "wojskowej jakości"},
    { 350, "przemysłowej jakości"},
    { 675, "spoza obiegu"},
    { 800, "klasy poufne"},
    { 900, "klasy tajne"},
    { 995, "klasy tajne-kosa"}
  };
  
  int i;
  for (i = 0; i < 6 && random_int >= qualities[i].value + luck; i++);

  std::string case_path = "Cases/" + collection + "/case.json";
  simdjson::dom::parser parser;
  simdjson::dom::element skins = parser.load(case_path);

  if (simdjson::error_code error = parser.load(case_path).get(skins); error) {
    std::cout << error << std::endl;
    Skin.skin_title = "N/A";
    Skin.skin_quality = "N/A";
    Skin.skin_description = "N/A";
    Skin.skin_path = "N/A";
    Skin.skin_ps = "N/A";
    Skin.skin_weapon = "N/A";
    Skin.skin_float = "N/A";
    Skin.skin_stat_track = false;
    Skin.skin_price = 0;
    return Skin;
  }

  simdjson::dom::array skins_array = skins.get_array();
  
  std::vector<simdjson::dom::element> quality_skins;

  for (simdjson::dom::element skin : skins_array) {
    std::string_view quality;
    
    if (skin["quality"].get(quality) == simdjson::SUCCESS && quality == qualities[i - 1].quality) {
      quality_skins.push_back(skin);
    }
  }
  

  if (quality_skins.empty()) {
    Skin.skin_title = "N/A";
    Skin.skin_quality = "N/A";
    Skin.skin_description = "N/A";
    Skin.skin_path = "N/A";
    Skin.skin_ps = "N/A";
    Skin.skin_weapon = "N/A";
    Skin.skin_float = "N/A";
    Skin.skin_stat_track = false;
    Skin.skin_price = 0;
    return Skin;
  }

  std::uniform_int_distribution<int> skin_dist(0, quality_skins.size() - 1);
  simdjson::dom::element selected_skin = quality_skins[skin_dist(gen)];

  std::string_view title, description, ps, path, weapon;
  
  if (selected_skin["title"].get(title) == simdjson::SUCCESS) {
    Skin.skin_title = std::string(title);
  }
  if (selected_skin["description"].get(description) == simdjson::SUCCESS) {
    Skin.skin_description = std::string(description);
  }
  if (selected_skin["ps"].get(ps) == simdjson::SUCCESS) {
    Skin.skin_ps = std::string(ps);
  }
  if (selected_skin["skin"].get(path) == simdjson::SUCCESS) {
    Skin.skin_path = std::string(path);
  }
  if (selected_skin["weapon"].get(weapon) == simdjson::SUCCESS) {
    Skin.skin_weapon = std::string(weapon);
  }

  Skin.skin_quality = qualities[i - 1].quality;

  Skin.skin_price = random_int * 10 + 101;
  if (Skin.skin_quality == "tajne-kosa") {
    Skin.skin_price += 1000 + random_int;
  }

  std::string skin_float = "0.";
  std::uniform_int_distribution<char> dist_float(0, 9);
  for (int i = 0; i < 12; i++) {
    skin_float += std::to_string(dist_float(gen)); 
  }
  
  Skin.skin_float = skin_float;

  if(skin_float[7] == '5') {
    Skin.skin_stat_track = true;
    Skin.skin_price += 100 + random_int;
    return Skin;
  }

  Skin.skin_stat_track = false;
  return Skin;
}

// 2. Return dynamic array available cases in Case dir and array size  

Returned_case* available_cases(int& arr_size) {
  std::string path = "Cases/";

  int count_dir = 0;
  for (const std::filesystem::directory_entry dirs_entry : std::filesystem::directory_iterator(path)) {
    if (dirs_entry.is_directory()) {
      count_dir++;
    }
  }

  Returned_case* cases_array = new Returned_case[count_dir];

  int i = 0;
  for (const std::filesystem::directory_entry cases_entry : std::filesystem::directory_iterator(path)) {
    if (cases_entry.is_directory()) {
      cases_array[i].case_title = cases_entry.path().filename().string();
      cases_array[i].case_path = cases_entry.path().string() + "/case.png";
      i++;
    }
  }
  arr_size = count_dir;
  return cases_array;
}

// 3. Return all available skins from selected case

Returned_skin* selected_case_skins(int& arr_size, const std::string collection){
  std::string case_path = "Cases/" + collection + "/case.json";
  simdjson::dom::parser parser;
  simdjson::dom::element skins = parser.load(case_path);

  
  simdjson::error_code error = parser.load(case_path).get(skins);
  if (error) {
    std::cout << error << std::endl;
    return nullptr;
  }

  simdjson::dom::array skins_array = skins.get_array();

  int skin_count = 0;
  for(simdjson::dom::element skin : skins_array) {
    skin_count++;
  };
  arr_size = skin_count;
  Returned_skin* available_skins_array = new Returned_skin[arr_size];
  
  int i = 0;
  for(simdjson::dom::element skin : skins_array){
    std::string_view title, path, quality, weapon;
    if (skin["title"].get(title) == simdjson::SUCCESS) {
      available_skins_array[i].skin_title = title;
    }
    if (skin["skin"].get(path) == simdjson::SUCCESS) {
      available_skins_array[i].skin_path = path;
    }
    if (skin["quality"].get(quality) == simdjson::SUCCESS) {
      available_skins_array[i].skin_quality = quality;
    }
    if (skin["weapon"].get(weapon) == simdjson::SUCCESS) {
      available_skins_array[i].skin_weapon = weapon;
    }

    available_skins_array[i].skin_description = "N/A";
    available_skins_array[i].skin_ps = "N/A";
    available_skins_array[i].skin_float = "N/A";
    available_skins_array[i].skin_stat_track = false;
    i++;
  };

  return available_skins_array;
}

// 4. Write skin to user inventory

void write_to_inventory(Returned_skin skin) {
  std::string inventory_path = "User/inventory.json";

  simdjson::dom::parser parser;
  simdjson::simdjson_result<simdjson::dom::element> inv_results = parser.load(inventory_path);

  std::ostringstream out;
  out << "[\n";

  bool first = true;

  if (inv_results.error() == simdjson::error_code::SUCCESS) {
    simdjson::dom::element inventory = inv_results.value();

    if (inventory.is_array()) {
      simdjson::dom::array inv_array = inventory.get_array();

      for (simdjson::dom::element skin_obj : inv_array) {
        if (!first) out << ",\n";
        out << skin_obj;
        first = false;
      }
    }
  }
  
  if (!first) out << ",\n";
  
  int skin_id = 0;

  if (inv_results.error() == simdjson::error_code::SUCCESS) {
    simdjson::dom::element inventory = inv_results.value();

    if (inventory.is_array()) {
      simdjson::dom::array inv_array = inventory.get_array();

      for (simdjson::dom::element skin_obj : inv_array) {
        skin_id++;
      }
    }
  }


  out << "{\n";
  out << "  \"id\": " << skin_id << ",\n";
  out << "  \"title\": \"" << skin.skin_title << "\",\n";
  out << "  \"description\": \"" << skin.skin_description << "\",\n";
  out << "  \"ps\": \"" << skin.skin_ps << "\",\n";
  out << "  \"quality\": \"" << skin.skin_quality << "\",\n";
  out << "  \"weapon\": \"" << skin.skin_weapon << "\",\n";
  out << "  \"skin\": \"" << skin.skin_path << "\",\n";
  out << "  \"stat_track\": " << (skin.skin_stat_track ? "true" : "false") << ",\n";
  out << "  \"skin_float\": \"" << skin.skin_float << "\",\n";
  out << "  \"price\": " << skin.skin_price << "\n";
  out << "}\n";

  out << "]";

  std::ofstream inventory_file(inventory_path);
  inventory_file << out.str();
}

// 5. Return dynamic array of all user skins

Returned_skin* get_inventory(int& arr_size) {
  std::string inventory_path = "User/inventory.json";
  simdjson::dom::parser parser;
  
  simdjson::dom::element skins = parser.load(inventory_path); 
  simdjson::error_code error = parser.load(inventory_path).get(skins);

  if (error) {
    std::cout << error << std::endl;
    return nullptr;
  }

  simdjson::dom::array skins_array = skins.get_array();
  
  int count = 0;
  for(simdjson::dom::element skin : skins_array) {
    count++;
  }
  
  arr_size = count;
  Returned_skin* inv_array = new Returned_skin[arr_size];

  int i = 0;
  for(simdjson::dom::element skin : skins_array) {
    std::string_view title, description, ps, quality, weapon, path, skin_float;
    bool stat_track;
    int64_t price;
    if (skin["title"].get(title) == simdjson::SUCCESS) {
      inv_array[i].skin_title = title;
    }
    if (skin["description"].get(description) == simdjson::SUCCESS) {
      inv_array[i].skin_description = description;
    }
    if (skin["ps"].get(ps) == simdjson::SUCCESS) {
      inv_array[i].skin_ps = ps;
    }
    if (skin["quality"].get(quality) == simdjson::SUCCESS) {
      inv_array[i].skin_quality = quality;
    }
    if (skin["weapon"].get(weapon) == simdjson::SUCCESS) {
      inv_array[i].skin_weapon = weapon;
    }
    if (skin["skin"].get(path) == simdjson::SUCCESS) {
      inv_array[i].skin_path = path;
    }
    if (skin["stat_track"].get(stat_track) == simdjson::SUCCESS) {
      inv_array[i].skin_stat_track = stat_track;
    }
    if (skin["skin_float"].get(skin_float) == simdjson::SUCCESS) {
      inv_array[i].skin_float = skin_float;
    }
    if (skin["price"].get(price) == simdjson::SUCCESS) {
      inv_array[i].skin_price = price;
    }
    i++;
  }

  return inv_array;
}

// 6. Return struct with username and current money amount

User_data get_user_info() {
  std::string user_path = "User/user.json";
  simdjson::dom::parser parser;
  simdjson::dom::element user_info = parser.load(user_path);
  
  User_data returned_user;
  std::string_view username;
  uint64_t money;

  if (user_info["username"].get(username) == simdjson::SUCCESS) {
    returned_user.username = username;
  }
  if (user_info["money"].get(money) == simdjson::SUCCESS) {
    returned_user.money = money;
  }

  return returned_user;
}

// 7. Write new money amount or username to user info file

void change_user_data(int64_t money = 0, std::string username = "") {
  std::string user_path = "User/user.json";
  simdjson::dom::parser parser;
  simdjson::dom::element user_info = parser.load(user_path);
  
  std::ostringstream out;
  std::string_view username_json;
  int64_t money_json;
  out << "{\n";
  if (username != "") {
    out << "  \"username\": \"" << username << "\",\n";
  }
  else {
    if (user_info["username"].get(username_json) == simdjson::SUCCESS) {
      out << "  \"username\": \"" << username_json << "\",\n";
    }
  }
  if (user_info["money"].get(money_json) == simdjson::SUCCESS) {
    out << "  \"money\": " << money_json + money << "\n";
  }

  out << "}";

  std::ofstream user_file(user_path);
  user_file << out.str();
}

// 8. Sell user skin

void sell_skin(int id) {
  std::string inventory_path = "User/inventory.json";
  simdjson::dom::parser parser;
  simdjson::dom::element inv_info;
  
  if (parser.load(inventory_path).get(inv_info) != simdjson::SUCCESS) {
    std::cerr << "Failed to load inventory." << std::endl;
    return;
  }

  simdjson::dom::array inv_array = inv_info.get_array();
  std::ostringstream out;
  out << "[\n";

  bool first = true;
  int64_t skin_price = 0;
  bool skin_found = false;

  for (simdjson::dom::element skin_obj : inv_array) {
    int64_t skin_id;
    if (skin_obj["id"].get(skin_id) == simdjson::SUCCESS && skin_id == id) {
      // Get skin price and skip writing this skin
        if(skin_obj["price"].get(skin_price) == simdjson::SUCCESS){
          skin_found = true;
          continue;
      }
    }

    if (!first) out << ",\n";
    out << skin_obj;
    first = false;
  }

  out << "\n]";

  if (skin_found) {
    // Write the updated inventory back
    std::ofstream file(inventory_path);
    file << out.str();

    // Update user's money
    change_user_data(skin_price); // Adds skin_price to user money
  } else {
    std::cerr << "Skin with ID " << id << " not found." << std::endl;
  }
}

Case_bot::Case_bot(const std::string& name, int luck_factor) : bot_name(name), luck(luck_factor) {}
  
void Case_bot::set_bot_name (const std::string& name) {
  bot_name = name;
}

void Case_bot::set_luck (int luck_factor) {
  luck = luck_factor;
}

std::string Case_bot::get_bot_name () const {return bot_name;}
int Case_bot::get_luck () const {return luck;}

bool Case_bot::Fight(const std::string& collection, int number_of_rolls){

  int player_skins_value = 0;
  int bot_skins_value = 0;

  std::vector<Returned_skin> player_skins;
  std::vector<Returned_skin> bot_skins;

  for(int i = 0; i < number_of_rolls; i++){
    Returned_skin player_skin = draw_skin(collection, luck);
    player_skins_value += player_skin.skin_price;
    player_skins.push_back(player_skin);
  }

  for(int i = 0; i < number_of_rolls; i++){
    Returned_skin bot_skin = draw_skin(collection, luck);
    bot_skins_value += bot_skin.skin_price;
    bot_skins.push_back(bot_skin);
  }

  if(player_skins_value > bot_skins_value){ 
    for(const Returned_skin& skin : player_skins){
      write_to_inventory(skin);
    }

    for(const Returned_skin& bot_skin : bot_skins){
      write_to_inventory(bot_skin);
    }

    return true;

  } else {
    return false;
  } 
}

