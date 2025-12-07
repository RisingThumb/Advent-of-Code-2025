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
uint64_t solution5dot1(string path);
uint64_t solution5dot2(string path);
uint64_t solution6dot1(string path);
uint64_t solution6dot2(string path);
uint64_t solution7dot1(string path);
int64_t solution7dot2(string path);

struct COORDINATE {
    int x;
    int y;
} coordinate;

struct RANGE {
    uint64_t lowerBound;
    uint64_t upperBound;
} range;

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
                if (active == 8) {
                    outputLabelText = format("{}", solution5dot1(filePaths[0]));
                }
                if (active == 9) {
                    outputLabelText = format("{}", solution5dot2(filePaths[0]));
                }
                if (active == 10) {
                    outputLabelText = format("{}", solution6dot1(filePaths[0]));
                }
                if (active == 11) {
                    outputLabelText = format("{}", solution6dot2(filePaths[0]));
                }
                if (active == 12) {
                    outputLabelText = format("{}", solution7dot1(filePaths[0]));
                }
                if (active == 13) {
                    outputLabelText = format("{}", solution7dot2(filePaths[0]));
                }
                
            }
        };
        DrawText(outputLabelText.c_str(), screenWidth/2,screenHeight/2 -10, 20, DARKGRAY);
        if (GuiButton((Rectangle){ screenWidth-100, screenHeight-20, 100, 20 }, "#16#COPY")) {
            SetClipboardText(outputLabelText.c_str());
        }
        if(GuiDropdownBox((Rectangle){0, 0, 200, 20}, "Puzzle 1.1;Puzzle 1.2;Puzzle 2.1;Puzzle 2.2;Puzzle 3.1;Puzzle 3.2;Puzzle 4.1;Puzzle 4.2;Puzzle 5.1;Puzzle 5.2;Puzzle 6.1;Puzzle 6.2;Puzzle 7.1;Puzzle 7.2", &active, dropdownOpen)) dropdownOpen = !dropdownOpen;
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

bool range_comp (RANGE a, RANGE b) {
    return a.upperBound < b.upperBound;
}

u_int64_t solution5dot1(string path) {
    // Parse into a 2D vector(Vector of strings)
    vector<RANGE> ranges;
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        return 1;
    }
    string line;
    while (getline(file, line)) {
        // Newline splits the ranges and food IDs
        if (line.length() <= 1) break;
        auto pos = line.find('-');
        string lowerRange = line.substr(0, pos);
        string upperRange = line.substr(pos+1, line.length());
        uint64_t upperRangeInt = stoull(upperRange);
        uint64_t lowerRangeInt = stoull(lowerRange);

        ranges.push_back((RANGE){lowerRangeInt, upperRangeInt});
    }
    sort(ranges.begin(), ranges.end(), range_comp);
    // Now we need to merge overlapping ranges. Start from the back and work forwards?
    for (int i = ranges.size()-1; i >= 0; i--) {
        RANGE checkRange = ranges.at(i);
        int upperCheck = i+1;
        if (i+1 < ranges.size()) {
            // Check if the upper thing overlaps.
            
            RANGE rangeCheck = ranges.at(i+1);
            bool overlaps = ((checkRange.lowerBound <= rangeCheck.upperBound) && (rangeCheck.lowerBound <= checkRange.upperBound));
            //bool overlaps = false;
            if (overlaps) {
                checkRange.lowerBound = min(checkRange.lowerBound, rangeCheck.lowerBound);
                checkRange.upperBound = max(checkRange.upperBound, rangeCheck.upperBound);
                ranges.erase(ranges.begin()+i+1);
                // We got a copy so we need to replace it
                ranges.erase(ranges.begin()+i);
                ranges.insert(ranges.begin()+i, checkRange);
            }
        }
    }
    // We now have a sorted list of maximally overlapped ranges 
    // We now go through and check every ID to see if it's in any range
    u_int64_t hitCount = 0;
    while (getline(file, line)) {
        uint64_t id = stoull(line);
        bool spoiled = true;
        for (auto i: ranges) {
            if (id >= i.lowerBound && id <= i.upperBound) {
                spoiled = false;
                break;
            }
        }
        if (!spoiled) hitCount++;
    }
    return hitCount;
}

u_int64_t solution5dot2(string path) {
    // Parse into a 2D vector(Vector of strings)
    vector<RANGE> ranges;
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        return 1;
    }
    string line;
    while (getline(file, line)) {
        // Newline splits the ranges and food IDs
        if (line.length() <= 1) break;
        auto pos = line.find('-');
        string lowerRange = line.substr(0, pos);
        string upperRange = line.substr(pos+1, line.length());
        uint64_t upperRangeInt = stoull(upperRange);
        uint64_t lowerRangeInt = stoull(lowerRange);

        ranges.push_back((RANGE){lowerRangeInt, upperRangeInt});
    }
    sort(ranges.begin(), ranges.end(), range_comp);
    // Now we need to merge overlapping ranges. Start from the back and work forwards?
    for (int i = ranges.size()-1; i >= 0; i--) {
        RANGE checkRange = ranges.at(i);
        int upperCheck = i+1;
        if (i+1 < ranges.size()) {
            // Check if the upper thing overlaps.
            
            RANGE rangeCheck = ranges.at(i+1);
            bool overlaps = ((checkRange.lowerBound <= rangeCheck.upperBound) && (rangeCheck.lowerBound <= checkRange.upperBound));
            //bool overlaps = false;
            if (overlaps) {
                checkRange.lowerBound = min(checkRange.lowerBound, rangeCheck.lowerBound);
                checkRange.upperBound = max(checkRange.upperBound, rangeCheck.upperBound);
                ranges.erase(ranges.begin()+i+1);
                // We got a copy so we need to replace it
                ranges.erase(ranges.begin()+i);
                ranges.insert(ranges.begin()+i, checkRange);
            }
        }
    }
    // We now have a sorted list of maximally overlapped ranges 
    // We now go through and check every ID to see if it's in any range
    u_int64_t hitCount = 0;
    for (auto r: ranges) {
        hitCount += r.upperBound - r.lowerBound +1;
    }
    return hitCount;
}

u_int64_t solution6dot1(string path) {
    vector<vector<u_int64_t>> columns;
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        return 1;
    }
    string line;
    while (getline(file, line)) {
        if (line.at(0) == '*' || line.at(0) == '+') break;
        string tmp;
        stringstream ss(line);
        int i = 0;
        bool needToCreateVectors = (columns.size() == 0);
        while (getline(ss, tmp, ' ')) {
            if (tmp.length() == 0) continue;
            if (needToCreateVectors) {
                vector<u_int64_t> column;
                columns.push_back(column);
            }
            u_int64_t val = stoull(tmp);
            columns.at(i).push_back(val);
            i+=1;
            
        }
    }
    string tmp;
    stringstream ss(line);
    int i = 0;
    u_int64_t sumOfSums = 0;
    while (getline(ss, tmp, ' ')) {
        u_int64_t sum = 0;
        if (tmp.length() == 0) continue;
        bool isMult = false;
        if (tmp.at(0) == '*') isMult = true;
        if (isMult) sum = 1;
        for (auto j: columns.at(i)) {
            if (isMult) sum *= j;
            else sum+=j;
        }
        sumOfSums += sum;
        i +=1;
    }
    // Do multiplication and addition here
    return sumOfSums;
}

u_int64_t solution6dot2(string path) {
    vector<string> lines;
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        return 1;
    }
    // Cannot rely on the column width. Need to determine it PER column?
    string line;
    while (getline(file, line)) {
        if (line.at(0) == '*' || line.at(0) == '+') break;
        lines.push_back(line);
    }
    stringstream symbollinestream(line);
    u_int64_t sumOfSums = 0;
    vector<int> sizes;
    int width = 0;
    char multiplier = line.at(0);
    for (int i = 1; i< line.length(); i++) {
        
        if (line.at(i) == ' ') width +=1;
        if (line.at(i) == '*' || line.at(i) == '+') {
            sizes.push_back(width);
            width = 0;
        }
        if (i+1 >= line.length()) sizes.push_back(width+1);
    }
    int runningSize = 0;
    for (int i = 0; i<sizes.size(); i++) {
        int columnWidth = sizes.at(i);
        if (columnWidth == 0) {
            break;
        }
        bool mult = (line.at(i+runningSize) == '*');
        assert(line.at(i+runningSize) != ' ');
        vector<u_int64_t> numbers;
        for (int j = 0; j<columnWidth; j++) {
            string strNum = "";
            for (int k = 0; k < lines.size(); k++) {
                strNum += lines.at(k).at(j+runningSize+i);
            }
            numbers.push_back(stoull(strNum));
        }
        u_int64_t sum = 0;
        if (mult) {
            sum = 1;
            for (auto num: numbers) sum*=num;
        }
        else {
            for (auto num: numbers) sum+=num;
        }
        sumOfSums += sum;

        // At end
        runningSize += columnWidth;
    }

    // Do multiplication and addition here
    return sumOfSums;
}

u_int64_t solution7dot1(string path) {
    vector<string> lines;
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        return 1;
    }
    // Cannot rely on the column width. Need to determine it PER column?
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    u_int64_t splitCount = 0;
    for (int i = 0; i < lines.size()-1; i++ ){
        string lineToWorkOn = lines[i];
        string lineBelowToMutate = lines[i+1];
        for(int x = 0; x < lineToWorkOn.length(); x++) {
            if (lineToWorkOn[x] == '.') continue;
            if (lineToWorkOn[x] == '^') continue;
            if (lineToWorkOn[x] = 'S' or lineToWorkOn[x] =='|') {
                if (lineBelowToMutate[x] == '.') {
                    lineBelowToMutate[x] = '|';
                }
                if (lineBelowToMutate[x] == '^') {
                    splitCount += 1;
                    if (x-1 >= 0) {
                        lineBelowToMutate[x-1] = '|';
                    }
                    if (x+1 < lineToWorkOn.length()) {
                        lineBelowToMutate[x+1] = '|';
                    }
                }
            }
        }
        lines[i+1] = lineBelowToMutate;
    }
    return splitCount;
}

int64_t solution7dot2(string path) {
    vector<string> lines;
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        return 1;
    }
    // Cannot rely on the column width. Need to determine it PER column?
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    // Convert into an int form. -1 is splitters
    vector<vector<int64_t>> intLines;
    for (auto line: lines) {
        vector<int64_t> intLine;
        for (int i =0; i < line.length(); i++) {
            if (line[i] == '.') intLine.push_back(0);
            if (line[i] == '^') intLine.push_back(-1);
            if (line[i] == 'S') intLine.push_back(1);
        }
        intLines.push_back(intLine);
    }
    for (int i = 0; i < intLines.size()-1; i++ ){
        vector<int64_t> intLine = intLines[i];
        vector<int64_t> intLineBelow = intLines[i+1];
        for (int x = 0; x< intLine.size(); x++) {
            if (intLine[x] <= 0) continue;
            if (intLine[x] > 0) {
                if (intLineBelow[x] == -1) {
                    intLineBelow[x-1] += intLine[x];
                    intLineBelow[x+1] += intLine[x];
                }
                else {
                    intLineBelow[x] += intLine[x];
                }
            }
        }
        intLines[i+1] = intLineBelow;
    }
    
    int64_t count = 0;
    for (auto num : intLines[intLines.size()-1]) {
        count += num;
    }
    return count;
}


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    srand(clock());

    // int64_t c = solution7dot2("part7Input.txt");
    // cout << to_string(c) << endl;

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