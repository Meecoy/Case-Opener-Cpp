#include "raylib.h"
#include <vector>
#include <ctime>
#include <cstdlib>

// ========================================
// IF U ON KNO!
// "override" overriding a fucntion from parent's class
// "virtual" function can be overridden later on
// "auto" get type of something
// ========================================

// These should be in some kind of settings / utils / consts file
// cosnts (settings)
const int WIN_W = 800;
const int WIN_H = 600;
const char* TITLE = "Raylib Window";
const int FPS = 60;

enum class Screen {
    TITLE,
    CASE
};

class App {

    public:
        App(int width, int height, const char* title);
        ~App();
    
        void run();
    
    private:
        int winW;
        int winH;
        const char* title;
        Screen currentScreen;

        std::vector<Sprite*> visibleGroup;
    
        void update();
        void render();
    
        void initTitleScreen();
        void updateTitleScreen();
        void renderTitleScreen();
        void unloadTitleScreen();
    
        void initCaseScreen();
        void updateCaseScreen();
        void renderCaseScreen();
        void unloadCaseScreen();
    };

 
App::App(int width, int height, const char* title)
    : winW(width), winH(height), title(title), currentScreen(Screen::TITLE) {
    InitWindow(winW, winH, title);
    SetTargetFPS(FPS);
    initTitleScreen();
}

App::~App() {
    CloseWindow();
}

void App::run() {
    while (!WindowShouldClose()) {
        update();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        render();

        EndDrawing();
    }
}

void App::update() {
    switch (currentScreen) {
        case Screen::TITLE: updateTitleScreen(); break;
        case Screen::CASE: updateCaseScreen(); break;
        default: break;
    }
}

void App::update() {
    switch (currentScreen) {
        case Screen::TITLE: renderTitleScreen(); break;
        case Screen::CASE: renderCaseScreen(); break;
        default: break;
    }
}

void App::initTitleScreen() {
    visibleGroup.clear();
}

void App::initCaseScreen() {
    visibleGroup.clear();
}

void App::unloadTitleScreen() {
    for (Sprite* s : visibleGroup)
    {
        delete s;
    }

    // ??? ==================================
    // for (auto& texture : skinTextureDB)
    // {
    //     UnloadTexture(texture);
    // }
}

void App::unloadCaseScreen() {
    for (Sprite* s : visibleGroup)
    {
        delete s;
    }
}

// Base sprite class
class Sprite
{
    public:
        virtual void update(float dt) {}
        virtual void draw() {}
        virtual bool shouldRemove() { return false; }
        virtual ~Sprite() {}
};

// Item sprite
class Item : public Sprite
{
    public:
        Vector2 pos;
        Vector2 size;
        Texture2D texture;
        Color tint;
        Rectangle rect;
        Color rect_color;
    
        Item(Vector2 p, Vector2 s, Texture2D t, Color c)
        {
            pos = p;
            size = s;
            texture = t;
            tint = c;
            rect = { pos.x, pos.y, size.x, size.y };
            rect_color = { 100, 100, 100, 100 };
        }
    
        void update(float dt) override
        {
            // pass
            // frick c++
        }
    
        void draw() override
        {
            DrawRectangleRec(rect, rect_color);
            DrawTextureEx(texture, pos, 0, 0.25f, tint);
        }
};

// Case item sprite
class CaseItem : public Sprite
{
    public:
        Vector2 pos;
        Vector2 size;
        Texture2D texture;
        Color tint;
        Rectangle rect;
        Color rect_color;
        Vector2 vel;
    
        CaseItem(Vector2 p, Vector2 s, Texture2D t, Color c)
        {
            pos = p;
            size = s;
            texture = t;
            tint = c;
            rect = { pos.x, pos.y, size.x, size.y };
            rect_color = { 100, 100, 100, 100 };
            vel = { 150, 0 };
        }
    
        void update(float dt) override
        {
            pos.x += vel.x * dt;
            rect.x = pos.x;
        }
    
        void draw() override
        {
            DrawRectangleRec(rect, rect_color);
            DrawTextureEx(texture, pos, 0, 0.25f, tint);
        }
    
        bool shouldRemove() override
        {
            return rect.x > WIN_W;
        }
    
        bool isStopped()
        {
            return vel.x == 0;
        }
};

// WORK IN PROGRESS =============================
// int main() {
//     App app(WIN_W, WIN_H, TITLE);
//     app.run();

//     return 0;
// }

int main()
{
    InitWindow(WIN_W, WIN_H, TITLE);

    SetTargetFPS(FPS);

    // Create skin's textures database
    Texture2D skinTextureDB[] =
    {
        LoadTexture("Cases/polityczka/Skins/Chanuka.png"),
        LoadTexture("Cases/polityczka/Skins/Skapiec.png"),
        LoadTexture("Cases/polityczka/Skins/GierekCukierek.png"),
        LoadTexture("Cases/polityczka/Skins/Dewiant.png"),
        LoadTexture("Cases/polityczka/Skins/11l9(premium).png"),
        LoadTexture("Cases/polityczka/Skins/Cziaa.png"),
        LoadTexture("Cases/polityczka/Skins/NiemieckiAgent.png"),
        LoadTexture("Cases/polityczka/Skins/StalowaBrzoza.png"),
    };
    const Texture2D drawnSkinTexture = skinTextureDB[0];
    
    // Sprite groups
    std::vector<Sprite*> visibleGroup;

    // Random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));


    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        BeginDrawing();

        ClearBackground(Color{25, 25, 25, 255});

        // Update sprites
        for (int i = 0; i < visibleGroup.size(); )
        {
            visibleGroup[i]->update(dt);

            // Remove if out of screen
            if (visibleGroup[i]->shouldRemove())
            {
                delete visibleGroup[i];
                visibleGroup.erase(visibleGroup.begin() + i);
            } else
            {
                i++;
            }
        }

        // Add case items when SPACE is pressed
        // if (IsKeyPressed(KEY_SPACE)) // imo to powinno dzialac, ale nie wiem
        if (IsKeyPressed(32)) // ???
        {
            for (int i = 0; i < 82; i++)
            {
                Texture2D texture = (i == 78) ? drawnSkinTexture : skinTextureDB[rand() % 8];
                CaseItem* ci = new CaseItem({ WIN_W + i * -150.0f, 400 }, { 128, 128 }, texture, { 200, 200, 200, 64 });
                visibleGroup.push_back(ci);
            }
        }

        // Check if 5th sprite is a CaseItem and has stopped, then add Item
        if (visibleGroup.size() > 4)
        {
            // "visibleGroup[4]" is a pointer
            // So we're using dynamic_cast to conver that pointer to a pointer to a "CaseItem"
            CaseItem* ci = dynamic_cast<CaseItem*>(visibleGroup[4]);
            if (ci && ci->isStopped())
            {
                Item* item = new Item({ 150, 150 }, { 128, 128 }, drawnSkinTexture, { 200, 200, 200, 64 });
                visibleGroup.push_back(item);
            }
        }

        EndDrawing();
    }

    // Delete, unload everythinh
    for (Sprite* s : visibleGroup)
    {
        delete s;
    }

    for (auto& texture : skinTextureDB)
    {
        UnloadTexture(texture);
    }

    CloseWindow();

    return 0;
}
