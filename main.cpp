#include <iostream>
#include <sstream>
#include <raylib.h>
#include <vector>


#define FPS 60
#define WIDTH 500
#define HEIGHT 500

#define PIZZA_WIDTH 18
#define PIZZA_HEIGHT 26

#define SPEEDUP 0.9

typedef struct {
    Texture texture = LoadTexture("res/gfx/pizza.png");
    Vector2 position = (Vector2) { (float)GetRandomValue(0, WIDTH-PIZZA_WIDTH), (float)GetRandomValue(0, HEIGHT-PIZZA_HEIGHT) };
} Pizza;

int main() 
{
    InitWindow(WIDTH, HEIGHT, "Collect Pizza");
    InitAudioDevice();
    SetTargetFPS(FPS);
    Music musicStream = LoadMusicStream("res/sfx/background.mp3");

    Vector2 fontSizeExplaination = MeasureTextEx(GetFontDefault(), "Click on the pizzas to earn points", 22, 1);
    Vector2 fontSizeTextPoints;
    Vector2 mousePos = GetMousePosition();

    double spawnTime = 5.;
    double lastSpawnTime = GetTime();
    std::vector<Pizza> pizzas;
    std::stringstream output;

    int points = 0;

    SetMusicVolume(musicStream, 0.1);
    PlayMusicStream(musicStream);
    while (!WindowShouldClose())
    {
        UpdateMusicStream(musicStream);
        if (GetTime() - lastSpawnTime >= spawnTime)
        {
            pizzas.push_back(Pizza {});
            lastSpawnTime = GetTime();
            if (spawnTime > 0.5) spawnTime *= SPEEDUP;
        }

        if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
        {
            mousePos = GetMousePosition();
            
            for (int i = 0; i < pizzas.size(); i++)
            {
                Pizza p = pizzas[i];
                if ((p.position.x <= mousePos.x && p.position.x + PIZZA_WIDTH >= mousePos.x) && (p.position.y <= mousePos.y && p.position.y + PIZZA_HEIGHT >= mousePos.y))
                {
                    pizzas.erase(pizzas.begin() + i);
                    points++;
                    output.str(std::string());
                    output << points << " Points so far";
                    fontSizeTextPoints = MeasureTextEx(GetFontDefault(), output.str().c_str(), 30, 1);
                }
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText(output.str().c_str(), WIDTH/2-fontSizeTextPoints.x/2, HEIGHT/2-fontSizeTextPoints.y/2, 30, BLACK);
            DrawText("Click on the pizzas to earn points", WIDTH/2-fontSizeExplaination.x/2, HEIGHT/4-fontSizeExplaination.y/2, 22, BLACK);

            for (Pizza p : pizzas)
                DrawTexture(p.texture, p.position.x, p.position.y, WHITE);

        EndDrawing();
    }
    return 0;
}