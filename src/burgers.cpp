#include <vector>
#include <cmath>
#include "raylib.h"

const int N = 200;
const double L = 1.0;
const double dx = L / N;
const double nu = 0.01;
const double dt = 0.0002;
const double TWO_PI = 2.0 * 3.14159265358979323846;
const int screenW = 800;
const int screenH = 450;
const int stepsPerFrame = 20;


Vector2 toPixel(int i, const std::vector<double>& u) {
    float px = ((float)i / N) * screenW;
    float py = screenH/2.0f - u[i] * (screenH/2.0f * 0.9f);
    return Vector2{ px, py};
}

void initWave(std::vector<double>& u) {
    for (int i = 0; i < N; i++)
    {
        double x = i * dx;
        u[i] = sin(TWO_PI * x);
    }
}

int main() {
    std::vector<double> u(N);
    std::vector<double> uNew(N);
    initWave(u);

    
    InitWindow(screenW, screenH, "Wave simulation");
    SetTargetFPS(60);
    double simTime = 0.0;

    while (!WindowShouldClose()) {
        simTime += stepsPerFrame * dt;
        if (IsKeyPressed(KEY_R)) {
            initWave(u);
            simTime = 0.0;
        }
        for (int s = 0; s < stepsPerFrame; s++) {
            for (int i = 0; i < N; i++) {
                int ip = (i + 1) % N;
            int im = (i - 1 + N) % N;
            double diffusion = (u[ip] - 2.0*u[i] + u[im]) / (dx*dx);
            double dudx;
            if (u[i] > 0.0) {
                dudx = (u[i] - u[im]) / dx;

            } else {
                dudx = (u[ip] - u[i]) /dx;
            }
            uNew[i] = u[i] + dt * ( -u[i] * dudx + nu * diffusion);
            }
            std::swap(u, uNew);
            
        }

            
            
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawLine(0, screenH/2, screenW, screenH/2, LIGHTGRAY);
            for (int i = 0; i < N - 1; i++) {
                 DrawLineV(toPixel(i, u), toPixel(i+1, u), BLUE);
            }
            DrawText(TextFormat("nu = %.4f", nu), 10, 10, 20, BLACK);
            DrawText(TextFormat("t = %.3f", simTime), 10, 35, 20, BLACK);
            EndDrawing();
    }

    CloseWindow();
    return 0;
}

