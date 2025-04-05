#include <iostream>
#include <simdjson.h>
#include <random>
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>

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



Returned_skin draw_skin(const std::string& collection) {
  Returned_skin Skin;

  static std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, 1000);
  int random_int = dist(gen);

  struct {
    unsigned int value;
    const char *quality;
  } qualities[] = {
    { 0, "konsumenckiej jakości"},
    { 300, "wojskowej jakości"},
    { 550, "przemysłowej jakości"},
    { 675, "spoza obiegu"},
    { 800, "klasy poufne"},
    { 900, "klasy tajne"},
    { 985, "klasy tajne-kosa"}
  };
  
  int i;
  for (i = 0; i < 7 && random_int >= qualities[i].value; i++);

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

  std::string skin_float = "0.";
  std::uniform_int_distribution<char> dist_float(0, 9);
  for (int i = 0; i < 12; i++) {
    skin_float += std::to_string(dist_float(gen)); 
  }
  
  Skin.skin_float = skin_float;

  if(skin_float[7] == '5') {
    Skin.skin_stat_track = true;
    return Skin;
  }

  Skin.skin_stat_track = false;
  return Skin;
}



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
      /*cases_array[i] = {
        cases_entry.path().filename().string(),
        cases_entry.path().string() + "/case.png"
      };*/

      cases_array[i].case_title = cases_entry.path().filename().string();
      cases_array[i].case_path = cases_entry.path().string() + "/case.png";
      i++;
    }
  }
  arr_size = count_dir;
  return cases_array;
}



Returned_skin* selected_case_skins(int& arr_size, const std::string collection){
  std::string case_path = "Cases/" + collection + "/case.json";
  simdjson::dom::parser parser;
  simdjson::dom::element skins = parser.load(case_path);

  
  auto error = parser.load(case_path).get(skins);
  if (simdjson::error_code error = parser.load(case_path).get(skins); error) {
    std::cout << error << std::endl;
    return nullptr;
  }

  simdjson::dom::array skins_array = skins.get_array();

  int skin_count = 0;
  for(simdjson::dom::element skin : skins_array) {
    skin_count++;
  };

  Returned_skin* available_skins_array = new Returned_skin[skin_count];
  
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

  arr_size = skin_count;
  return available_skins_array;
}



void write_to_inventory(Returned_skin skin) {
  std::string inventory_path = "Inventory/inventory.json";

  simdjson::dom::parser parser;
  auto inv_results = parser.load(inventory_path);

  std::ostringstream out;
  out << "[\n";

  bool first = true;

  if (inv_results.error() == simdjson::error_code::SUCCESS) {
    simdjson::dom::element inventory = inv_results.value();

    if (inventory.is_array()) {
      simdjson::dom::array inv_array = inventory.get_array();

      for (auto skin_obj : inv_array) {
        if (!first) out << ",\n";
        out << skin_obj;
        first = false;
      }
    }
  }

  if (!first) out << ",\n";

  out << "{\n";
  out << "  \"title\": \"" << skin.skin_title << "\",\n";
  out << "  \"description\": \"" << skin.skin_description << "\",\n";
  out << "  \"ps\": \"" << skin.skin_ps << "\",\n";
  out << "  \"quality\": \"" << skin.skin_quality << "\",\n";
  out << "  \"weapon\": \"" << skin.skin_weapon << "\",\n";
  out << "  \"skin\": \"" << skin.skin_path << "\"\n";
  out << "}\n";

  out << "]";

  std::ofstream inventory_file(inventory_path);
  inventory_file << out.str();
}

