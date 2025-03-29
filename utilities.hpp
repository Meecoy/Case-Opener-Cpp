#ifndef DRAW_SKIN_HPP
#define DRAW_SKIN_HPP

#include <simdjson.h>

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

Returned_skin draw_skin(const std::string& collection);

#endif

