#include "libs/raylib/src/raylib.h"
#include "utilities.hpp"
#include "simdjson.h"

#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

const int WIN_W = 800;
const int WIN_H = 600;
const int FPS = 60;

const float ITEM_WIDTH = 128.0f;
const float ITEM_HEIGHT = 128.0f;
const float SPACING = 150.0f;
const int TOTAL_ITEMS = 100;
const int WINNING_INDEX = 78;
const float ROLL_DURATION = 4.5f;

struct CaseItem {
    Texture2D texture;
    int index;
    std::string rarity;

    CaseItem(Texture2D tex, int idx, const std::string& rarityStr)
        : texture(tex), index(idx), rarity(rarityStr) {}

    void Draw(float scrollOffset) {
        float x = index * SPACING - scrollOffset;
        float y = 400;
        Rectangle dst = { x, y, ITEM_WIDTH, ITEM_HEIGHT };

        // Draw glow effect under the box
        Color glowColor = SKYBLUE;
        glowColor.a = 80; // Semi-transparent
        DrawRectangleRounded({ x - 8, y - 8, ITEM_WIDTH + 16, ITEM_HEIGHT + 16 }, 0.2f, 10, glowColor);

        // Draw background and texture
        DrawRectangleRec(dst, { 100, 100, 100, 100 });
        DrawTexturePro(texture, { 0, 0, (float)texture.width, (float)texture.height }, dst, { 0, 0 }, 0, WHITE);
    }
};

// Easing function for smooth scroll
float EaseOutCubic(float t) {
    return 1 - pow(1 - t, 3);
}

Returned_skin draw_skin(const std::string& collection, int luck);

void DrawSkin(const std::string& case_name, int luck) {
    SetTargetFPS(FPS);
    std::srand((unsigned int)std::time(nullptr));

    int dummy_size;
    Returned_skin* available_skins = selected_case_skins(dummy_size, case_name);
    if (!available_skins) {
        std::cerr << "Failed to load skins.\n";
        return;
    }

    std::vector<Texture2D> textureDB;
    for (int i = 0; i < dummy_size; ++i) {
        textureDB.push_back(LoadTexture(available_skins[i].skin_path.c_str()));
    }

    std::vector<CaseItem> items;
    Texture2D winningTexture;
    Returned_skin final_skin;

    float scrollOffset = 0.0f;
    float targetOffset = 0.0f;
    float elapsedTime = 0.0f;
    bool rolling = false;
    bool resultShown = false;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (IsKeyPressed(KEY_SPACE) && !rolling) {
            items.clear();

            final_skin = draw_skin(case_name, luck);
            Texture2D winnerTex = LoadTexture(final_skin.skin_path.c_str());
            winningTexture = winnerTex;

            for (int i = 0; i < TOTAL_ITEMS; i++) {
                bool isWinner = (i == WINNING_INDEX);
                Texture2D tex = isWinner ? winnerTex : textureDB[std::rand() % textureDB.size()];
                std::string rarity = isWinner ? final_skin.skin_quality : available_skins[std::rand() % dummy_size].skin_quality;
                items.emplace_back(tex, i, rarity);
            }

            targetOffset = WINNING_INDEX * SPACING - (WIN_W / 2.0f - ITEM_WIDTH / 2.0f);
            scrollOffset = 0.0f;
            elapsedTime = 0.0f;
            rolling = true;
            resultShown = false;
        }

        if (rolling) {
            elapsedTime += dt;
            float t = elapsedTime / ROLL_DURATION;
            if (t > 1.0f) {
                t = 1.0f;
                rolling = false;
                resultShown = true;
                write_to_inventory(final_skin);
            }

            scrollOffset = EaseOutCubic(t) * targetOffset;
        }

        BeginDrawing();
        ClearBackground({ 25, 25, 25, 255 });

        for (auto& item : items) {
            item.Draw(scrollOffset);
        }

        if (!resultShown)
            DrawLine(WIN_W / 2, 0, WIN_W / 2, WIN_H, RED);

        if (resultShown) {
            Rectangle dst = { 325, 30, ITEM_WIDTH, ITEM_HEIGHT };
            DrawRectangleRec(dst, { 80, 80, 80, 100 });
            DrawTexturePro(winningTexture, { 0, 0, (float)winningTexture.width, (float)winningTexture.height },
                           dst, { 0, 0 }, 0.0f, WHITE);

            std::string name = final_skin.skin_title + " | " + final_skin.skin_weapon;
            std::string stattrak = std::string("StatTrak: ") + (final_skin.skin_stat_track ? "Tak" : "Nie");
            std::string description = final_skin.skin_description;
            std::string floatStr = "Float: " + final_skin.skin_float;
            std::string price = "Cena: $" + std::to_string(final_skin.skin_price);

            DrawText(name.c_str(), 275, 170, 20, WHITE);
            DrawText(description.c_str(), 275, 200, 15, WHITE);
            DrawText(stattrak.c_str(), 275, 230, 15, WHITE);
            DrawText(floatStr.c_str(), 275, 260, 15, WHITE);
            DrawText(price.c_str(), 275, 290, 15, WHITE);
        }

        EndDrawing();
    }

    for (auto& tex : textureDB) UnloadTexture(tex);
    UnloadTexture(winningTexture);
    delete[] available_skins;
}

int main() {
    InitWindow(WIN_W, WIN_H, "Case Opening");

    User_data user = get_user_info();
    std::string selected_case = "polityczka";
    int luck = 0;

    DrawSkin(selected_case, luck);

    CloseWindow();
    return 0;
}
