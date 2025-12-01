#include "random"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"                         // Required for: IMGUI controls
#undef RAYGUI_IMPLEMENTATION                // Avoid including raygui implementation again

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif


#include <ctime>
#include "vector"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <assert.h>
#include <format>

#define MAX_FILEPATH_RECORDED   4096
#define MAX_FILEPATH_SIZE       2048



#define MIN(a,b) (((a)<(b))?(a):(b))

using namespace std;


const int screenWidth = 450;
const int screenHeight = 450;
int active = 0;
bool dropdownOpen = false;
int filePathCounter = 0;
char *filePaths[MAX_FILEPATH_RECORDED] = { 0 }; // We will register a maximum of filepaths
string outputLabelText = "";

void UpdateDrawFrame(void);
int solution1dot1(string path);
int solution1dot2(string path);

void UpdateDrawFrame(void) {
    // Update
    if (IsFileDropped())
    {
        FilePathList droppedFiles = LoadDroppedFiles();

        for (int i = 0, offset = filePathCounter; i < (int)droppedFiles.count; i++)
        {
            if (filePathCounter < (MAX_FILEPATH_RECORDED - 1))
            {
                TextCopy(filePaths[i], droppedFiles.paths[i]);
                if (filePathCounter == 0) {
                    filePathCounter ++;
                }
                
            }
        }

        UnloadDroppedFiles(droppedFiles);    // Unload filepaths from memory
    }


    // Draw
    BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        if (filePathCounter == 0) DrawText("Drop your files to this window!", 100, 40, 20, DARKGRAY);
        else {
            for (int i = 0; i < filePathCounter; i++)
            {
                if (i%2 == 0) DrawRectangle(0, 25 + 40*i, screenWidth, 40, Fade(LIGHTGRAY, 0.5f));
                else DrawRectangle(0, 25 + 40*i, screenWidth, 40, Fade(LIGHTGRAY, 0.3f));

                DrawText(filePaths[i], 120, 40 + 40*i, 10, GRAY);
            }

            DrawText("Drop new files...", 100,25+ 40*filePathCounter, 20, DARKGRAY);
        }
        if(GuiDropdownBox((Rectangle){0, 0, 200, 20}, "Puzzle 1.1;Puzzle 1.2", &active, dropdownOpen)) dropdownOpen = !dropdownOpen;
        if(GuiButton((Rectangle){210, 0, 200, 20}, "#01#Calculate for file")) {
            if (filePathCounter > 0) {
                if (active == 0) {
                    outputLabelText = format("Solution: {}", solution1dot1(filePaths[0]));
                }
                if (active == 1) {
                    outputLabelText = format("Solution: {}", solution1dot2(filePaths[0]));
                }
            }
        };
        DrawText(outputLabelText.c_str(), 100,85, 20, DARKGRAY);
    EndDrawing();


}

int solution1dot1(string path) {
    int hits0 = 0;
    int dialPos = 50;
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        return 1;
    }
    string line;
    while (getline(file, line)) {
        string adjustedLine = line.substr(1, line.length());
        int adjust = stoi(adjustedLine);
        if (line.at(0) == 'R') {
            dialPos += adjust;
        }
        if (line.at(0) == 'L') {
            dialPos -= adjust;
        }
        while (dialPos < 0) dialPos += 100;
        while (dialPos > 99) dialPos -= 100;
        if (dialPos == 0) {
            hits0 += 1;
        }
        //cout << line << endl;
    }
    file.close();
    return hits0;
}

int solution1dot2(string path) {
    int hits0 = 0;
    int dialPos = 50;
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        return 1;
    }
    string line;
    while (getline(file, line)) {
        int tmpPreviousDialPos = dialPos;
        string adjustedLine = line.substr(1, line.length());
        int adjust = stoi(adjustedLine);
        int direction = 0;
        if (line.at(0) == 'R') {
            direction = 1;
        }
        if (line.at(0) == 'L') {
            direction = -1;
        }
        for (int i = 0; i < adjust; i++) {
            dialPos += direction;

            if (dialPos == 100) dialPos = 0;
            if (dialPos == -1) dialPos = 99;

            if (dialPos == 0) hits0 += 1;
        }
    }
    file.close();
    return hits0;
}

void testsSolution1() {
        printf("%d\n", solution1dot2("part12TestInput.txt"));
    assert(
        solution1dot2("part12TestInput.txt") == 10
    );
    printf("%d\n", solution1dot2("part12TestInput2.txt"));
    assert(
        solution1dot2("part12TestInput2.txt") == 6
    );
    printf("%d\n", solution1dot2("part12TestInput3.txt"));
    assert(
        solution1dot2("part12TestInput3.txt") == 2
    );
    printf("%d\n", solution1dot2("part12TestInput4.txt"));
    assert(
        solution1dot2("part12TestInput4.txt") == 2
    );
    printf("%d\n", solution1dot2("part12TestInput5.txt"));
    assert(
        solution1dot2("part12TestInput5.txt") == 6
    );
    printf("%d\n", solution1dot2("part12TestInput6.txt"));
    assert(
        solution1dot2("part12TestInput6.txt") == 6
    );
    printf("%d\n", solution1dot2("part12TestInput7.txt"));
    assert(
        solution1dot2("part12TestInput7.txt") == 3
    );
    printf("%d\n", solution1dot2("part12TestInput8.txt"));
    assert(
        solution1dot2("part12TestInput8.txt") == 1
    );
    printf("%d\n", solution1dot2("part12TestInput9.txt"));
    assert(
        solution1dot2("part12TestInput9.txt") == 3
    );
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    srand(clock());
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Advent of Code 2025");

    // Allocate space for the required file paths
    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++)
    {
        filePaths[i] = (char *)RL_CALLOC(MAX_FILEPATH_SIZE, 1);
    }


    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    // Main game loop
    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
    #else
        while (!WindowShouldClose())    // Detect window close button or ESC key
        {
            //----------------------------------------------------------------------------------
            UpdateDrawFrame();
        }
    #endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

