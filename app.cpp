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

    CaseItem(Texture2D tex, int idx) : texture(tex), index(idx) {}

    void Draw(float scrollOffset) {
        float x = index * SPACING - scrollOffset;
        float y = 400;
        Rectangle dst = { x, y, ITEM_WIDTH, ITEM_HEIGHT };
        DrawRectangleRec(dst, { 100, 100, 100, 100 });
        DrawTexturePro(texture, { 0, 0, (float)texture.width, (float)texture.height }, dst, { 0, 0 }, 0, WHITE);
    }
};

// Easing function for smooth scroll (ease out cubic)
float EaseOutCubic(float t) {
    return 1 - pow(1 - t, 3);
}

int main() {
    InitWindow(WIN_W, WIN_H, "Case Opening");
    SetTargetFPS(FPS);
    std::srand((unsigned int)std::time(nullptr));

    std::vector<Texture2D> textureDB = {
        LoadTexture("Cases/polityczka/Skins/Chanuka.png"),
        LoadTexture("Cases/polityczka/Skins/Skąpiec.png"),
        LoadTexture("Cases/polityczka/Skins/GierekCukierek.png"),
        LoadTexture("Cases/polityczka/Skins/Dewiant.png"),
        LoadTexture("Cases/polityczka/Skins/11l9(premium).png"),
        LoadTexture("Cases/polityczka/Skins/Cziłała.png"),
        LoadTexture("Cases/polityczka/Skins/NiemieckiAgent.png"),
        LoadTexture("Cases/polityczka/Skins/StalowaBrzoza.png"),
        LoadTexture("Cases/polityczka/Skins/RekaWolnegoRynku.png")
    };

    std::vector<CaseItem> items;
    Texture2D winningTexture = textureDB[0];

    float scrollOffset = 0.0f;
    float targetOffset = 0.0f;
    float elapsedTime = 0.0f;
    bool rolling = false;
    bool resultShown = false;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (IsKeyPressed(KEY_SPACE)) {
            items.clear();
            for (int i = 0; i < TOTAL_ITEMS; i++) {
                Texture2D tex = (i == WINNING_INDEX) ? textureDB[0] : textureDB[std::rand() % textureDB.size()];
                items.emplace_back(tex, i);
                if (i == WINNING_INDEX) winningTexture = tex;
            }

            // Calculate how far we need to scroll to center the winning item
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
            }

            scrollOffset = EaseOutCubic(t) * targetOffset;
        }

        BeginDrawing();
        ClearBackground({ 25, 25, 25, 255 });

        for (auto& item : items) {
            item.Draw(scrollOffset);
        }

        DrawLine(WIN_W / 2, 0, WIN_W / 2, WIN_H, RED);

        if (resultShown) {
            DrawText("You got:", 50, 50, 24, WHITE);
            Rectangle dst = { 200, 30, ITEM_WIDTH, ITEM_HEIGHT };
            DrawRectangleRec(dst, { 80, 80, 80, 100 });
            DrawTexturePro(winningTexture, { 0, 0, (float)winningTexture.width, (float)winningTexture.height },
                           dst, { 0, 0 }, 0.0f, WHITE);
        }

        EndDrawing();
    }

    for (auto& tex : textureDB) UnloadTexture(tex);
    CloseWindow();
    return 0;
}
