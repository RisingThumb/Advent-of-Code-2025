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
#include <regex>
#include <stdint.h>

#define MAX_FILEPATH_RECORDED   4096
#define MAX_FILEPATH_SIZE       2048



#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

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
uint64_t solution2dot1(string path);
uint64_t solution2dot2(string path);
int solution3dot1(string path);
uint64_t solution3dot2(string path);
int solution4dot1(string path);
int solution4dot2(string path);
int getSetHitcountForPaperRollsIt(vector<string>& grid, int gridWidth);

struct COORDINATE {
    int x;
    int y;
} coordinate;

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
        if(GuiButton((Rectangle){210, 0, 200, 20}, "#01#Calculate for file")) {
            if (filePathCounter > 0) {
                if (active == 0) {
                    outputLabelText = format("{}", solution1dot1(filePaths[0]));
                }
                if (active == 1) {
                    outputLabelText = format("{}", solution1dot2(filePaths[0]));
                }
                if (active == 2) {
                    cout << "Hello?" << endl;
                    outputLabelText = format("{}", solution2dot1(filePaths[0]));
                }
                if (active == 3) {
                    outputLabelText = format("{}", solution2dot2(filePaths[0]));
                }
                if (active == 4) {
                    outputLabelText = format("{}", solution3dot1(filePaths[0]));
                }
                if (active == 5) {
                    outputLabelText = format("{}", solution3dot2(filePaths[0]));
                }
                if (active == 6) {
                    outputLabelText = format("{}", solution4dot1(filePaths[0]));
                }
                if (active == 7) {
                    outputLabelText = format("{}", solution4dot2(filePaths[0]));
                }
            }
        };
        DrawText(outputLabelText.c_str(), screenWidth/2,screenHeight/2 -10, 20, DARKGRAY);
        if (GuiButton((Rectangle){ screenWidth-100, screenHeight-20, 100, 20 }, "#16#COPY")) {
            SetClipboardText(outputLabelText.c_str());
        }
        if(GuiDropdownBox((Rectangle){0, 0, 200, 20}, "Puzzle 1.1;Puzzle 1.2;Puzzle 2.1;Puzzle 2.2;Puzzle 3.1;Puzzle 3.2;Puzzle 4.1;Puzzle 4.2", &active, dropdownOpen)) dropdownOpen = !dropdownOpen;
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

uint64_t solution2dot1(string path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        return 1;
    }
    uint64_t hitCount = 0;
    string line;
    while (getline(file, line, ',')) {
        auto pos = line.find('-');
        string firstId = line.substr(0, pos);
        string secondId = line.substr(pos+1, line.length());
        uint64_t firstIdInt = stoull(firstId);
        uint64_t secondIdInt = stoull(secondId);
        for(uint64_t i = firstIdInt; i <= secondIdInt; i++) {
            string idToCheck = to_string(i);
            if (idToCheck.length() % 2 != 0) continue;
            string halfId = idToCheck.substr(0, idToCheck.length()/2);
            string fullMatch=halfId.append(halfId);
            regex r(fullMatch);
            smatch match;
            bool matched = regex_search(idToCheck, match, r);
            if (matched){
                hitCount+=i;
            }
        }
    }
    return hitCount;
}

uint64_t solution2dot2(string path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        return 1;
    }
    uint64_t hitCount = 0;
    string line;
    while (getline(file, line, ',')) {
        auto pos = line.find('-');
        string firstId = line.substr(0, pos);
        string secondId = line.substr(pos+1, line.length());
        uint64_t firstIdInt = stoull(firstId);
        uint64_t secondIdInt = stoull(secondId);
        for(uint64_t i = firstIdInt; i <= secondIdInt; i++) {
            string idToCheck = to_string(i);
            bool matchFinder = false;
            for (int j = 1; j <= idToCheck.length()/2; j++) {
                // If it doesn't fit into it nicely. So a repeating pattern of 1 digit will
                // A repeating pattern of 2 digits doesn't fit into an odd number
                // A repeating pattern of 3 digits fits nicely into anything like 6...9 etc
                if (idToCheck.length()%j != 0) continue;
                int matchExpects = idToCheck.length()/j;
                string portionToUse = idToCheck.substr(0, j);
                string formattedPortion = format("({}){{{},}}", portionToUse, matchExpects);
                regex r(formattedPortion);
                smatch match;
                matchFinder = regex_search(idToCheck, match, r);
                if (matchFinder) break;

            }
            if (matchFinder){
                hitCount+=i;
            }
        }
    }
    return hitCount;
}

uint64_t get_joltage(string line, int batterycount = 12) {
    int removals_left = line.length() - batterycount;
    vector<char> stack = {};

    for (int i =0; i < line.length(); i++) {
        char digit = line.at(i);
        while (removals_left > 0 && stack.size() > 0 && stack.back() < digit) {
            stack.pop_back();
            removals_left--;
        }
        stack.push_back(digit);
    }
    string a;
    for (int i = 0; i<batterycount; i++) {
        a.push_back(stack.at(i));
    }
    return stoull(a);
}

int solution3dot1(string path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        return 1;
    }
    string line;
    int totaljoltage = 0;
    while (getline(file, line)) {
        totaljoltage += get_joltage(line, 2);
    }
    return totaljoltage;
}


uint64_t solution3dot2(string path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        return 1;
    }
    string line;
    uint64_t totaljoltage = 0;
    while (getline(file, line)) {
        totaljoltage += get_joltage(line);
    }
    return totaljoltage;
}

bool am_i_paper_roll(int x, int y, vector<string>& grid) {
    return grid.at(y).at(x) == '@';
}

int solution4dot1(string path) {
    // Parse into a 2D vector(Vector of strings)
    vector<string> grid;
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        return 1;
    }
    string line;
    int gridWidth = 0;
    while (getline(file, line)) {
        gridWidth = line.length();
        grid.push_back(line);
    }
    int hitCount = getSetHitcountForPaperRollsIt(grid, gridWidth);
    return hitCount;
}

int solution4dot2(string path) {
    // Parse into a 2D vector(Vector of strings)
    vector<string> grid;
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        return 1;
    }
    string line;
    int gridWidth = 0;
    while (getline(file, line)) {
        gridWidth = line.length();
        grid.push_back(line);
    }
    int hitCount = 0;
    int hitCountThisIter = 1;
    while (hitCountThisIter > 0) {
        hitCountThisIter = getSetHitcountForPaperRollsIt(grid, gridWidth);
        hitCount += hitCountThisIter;
    }
    return hitCount;
}

int getSetHitcountForPaperRollsIt(vector<string>& grid, int gridWidth) {
    vector<COORDINATE> coordsHit;
    int hitCount = 0;
    for (int x = 0; x< gridWidth; x++) {
        for (int y = 0; y<grid.size(); y++) {
            if (!am_i_paper_roll(x, y, grid)) continue; // Don't count blank tiles
            int adjacencyCount = 0;
            for (int i = -1; i< 2; i++) {
                for (int j = -1; j<2; j++) {
                    if (x+i < 0 || x+i >= gridWidth) {
                        continue;
                    }
                    if (y+j < 0 || y+j >= grid.size()) {
                        continue;
                    }
                    if (i == 0 && j == 0) continue;
                    if (am_i_paper_roll(x+i, y+j, grid)) {
                        adjacencyCount += 1;
                    }
                }
            }
            if (adjacencyCount < 4) {
                COORDINATE coord = {x, y};
                coordsHit.push_back(coord);
                hitCount ++;
            }
        }
    }
    for (auto coord: coordsHit) {
        grid.at(coord.y)[coord.x] = '.';
    }
    return hitCount;
}


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    srand(clock());

    // uint64_t b = solution3dot2("part3Input.txt");
    // cout << to_string(b) << endl;


    // uint64_t b3 = solution2dot2("part2Input.txt");
    // string output3 = to_string(b3);
    // cout << output3 << endl;

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