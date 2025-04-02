#include <simdjson.h>
#include <random>

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

Returned_skin draw_skin(const std::string& collection) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, 1000);
  int random_int = dist(gen);

  struct {
    int value;
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
  simdjson::dom::element case = parser.load(case_path);
  simdjson::dom::array skins_array = case.get_array();
  
  std::vector<simdjson::dom::element> quality_skins;

  for (simdjson::dom::element skin : skins_array) {
    std::string_view quality;
    
    if (skin["quality"].get(quality) == simdjson::SUCCESS && quality == qualities[i - 1].quality) {
      quality_skins.push_back(skin);
    }
  }
  
  Returned_skin Skin;

  if (quality_skins.empty()) {
    Skin.skin_title = "N/A";
    Skin.skin_quality = "N/A";
    Skin.skin_description = "N/A";
    Skin.skin_path = "N/A";
    Skin.skin_ps = "N/A";
    Skin.skin_weapon = "N/A";
    Skin.skin_float = "N/A";
    Skin.skin_stat_track = "N/A";
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
  std::uniform_int_distribution<int> dist_float(0, 9);
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
