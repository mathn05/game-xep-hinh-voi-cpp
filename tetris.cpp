/*
 * Ði?u khi?n: A/D (trái/ph?i), W (xoay), S (roi nhanh), ESC (thoát)
 */

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>

using namespace std;

// Kích thu?c b?ng choi
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;
const int BOARD_X = 5;
const int BOARD_Y = 2;

// Các hình Tetris (7 lo?i)
const int SHAPES[7][4][4] = {
    // I - Hình que
    {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
    // O - Hình vuông
    {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}},
    // T - Hình ch? T
    {{0,0,0,0}, {0,1,0,0}, {1,1,1,0}, {0,0,0,0}},
    // S - Hình ch? S
    {{0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0}},
    // Z - Hình ch? Z
    {{0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0}},
    // J - Hình ch? J
    {{0,0,0,0}, {1,0,0,0}, {1,1,1,0}, {0,0,0,0}},
    // L - Hình ch? L
    {{0,0,0,0}, {0,0,1,0}, {1,1,1,0}, {0,0,0,0}}
};

// Màu s?c cho t?ng hình
const int COLORS[7] = {9, 14, 13, 10, 12, 11, 6}; // Cyan, Yellow, Magenta, Green, Red, Blue, Orange

// Bi?n toàn c?c
int board[BOARD_HEIGHT][BOARD_WIDTH] = {0};
int currentShape[4][4];
int currentX, currentY;
int currentType;
int score = 0;
int level = 1;
int linesCleared = 0;
bool gameOver = false;

// KHAI BÁO HÀM (Function Prototypes)
void gotoxy(int x, int y);
void hideCursor();
void setColor(int color);
bool canMove(int x, int y);
void newShape();
void rotate();
void mergeShape();
void clearLines();
void drawBoard();

// Hàm di chuy?n con tr? console
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Hàm ?n con tr?
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// Hàm d?t màu text
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Ki?m tra va ch?m
bool canMove(int x, int y) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(currentShape[i][j]) {
                int newX = x + j;
                int newY = y + i;
                
                if(newX < 0 || newX >= BOARD_WIDTH || newY >= BOARD_HEIGHT) {
                    return false;
                }
                if(newY >= 0 && board[newY][newX]) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Kh?i t?o hình m?i
void newShape() {
    currentType = rand() % 7;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            currentShape[i][j] = SHAPES[currentType][i][j];
        }
    }
    currentX = BOARD_WIDTH / 2 - 2;
    currentY = 0;
    
    // Ki?m tra game over
    if(!canMove(currentX, currentY)) {
        gameOver = true;
    }
}

// Xoay hình
void rotate() {
    int temp[4][4];
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            temp[i][j] = currentShape[3-j][i];
        }
    }
    
    // Ki?m tra xem có xoay du?c không
    int oldShape[4][4];
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            oldShape[i][j] = currentShape[i][j];
            currentShape[i][j] = temp[i][j];
        }
    }
    
    if(!canMove(currentX, currentY)) {
        // Khôi ph?c n?u không xoay du?c
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                currentShape[i][j] = oldShape[i][j];
            }
        }
    }
}

// G?n hình vào b?ng
void mergeShape() {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(currentShape[i][j]) {
                int x = currentX + j;
                int y = currentY + i;
                if(y >= 0) {
                    board[y][x] = currentType + 1;
                }
            }
        }
    }
}

// Xóa hàng d?y
void clearLines() {
    int cleared = 0;
    for(int i = BOARD_HEIGHT - 1; i >= 0; i--) {
        bool full = true;
        for(int j = 0; j < BOARD_WIDTH; j++) {
            if(!board[i][j]) {
                full = false;
                break;
            }
        }
        
        if(full) {
            cleared++;
            for(int k = i; k > 0; k--) {
                for(int j = 0; j < BOARD_WIDTH; j++) {
                    board[k][j] = board[k-1][j];
                }
            }
            for(int j = 0; j < BOARD_WIDTH; j++) {
                board[0][j] = 0;
            }
            i++; // Ki?m tra l?i hàng này
        }
    }
    
    if(cleared > 0) {
        linesCleared += cleared;
        score += cleared * cleared * 100;
        level = linesCleared / 10 + 1;
    }
}

// V? b?ng choi
void drawBoard() {
    // V? vi?n
    setColor(15);
    for(int i = 0; i < BOARD_HEIGHT + 2; i++) {
        gotoxy(BOARD_X - 2, BOARD_Y + i);
        cout << "||";
        gotoxy(BOARD_X + BOARD_WIDTH * 2, BOARD_Y + i);
        cout << "||";
    }
    for(int i = 0; i < BOARD_WIDTH + 2; i++) {
        gotoxy(BOARD_X - 2 + i * 2, BOARD_Y + BOARD_HEIGHT + 1);
        cout << "==";
    }
    
    // V? các ô dã g?n
    for(int i = 0; i < BOARD_HEIGHT; i++) {
        for(int j = 0; j < BOARD_WIDTH; j++) {
            gotoxy(BOARD_X + j * 2, BOARD_Y + i);
            if(board[i][j]) {
                setColor(COLORS[board[i][j] - 1]);
                cout << "[]";
            } else {
                setColor(8);
                cout << ". ";
            }
        }
    }
    
    // V? hình dang roi
    setColor(COLORS[currentType]);
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(currentShape[i][j]) {
                int x = currentX + j;
                int y = currentY + i;
                if(y >= 0) {
                    gotoxy(BOARD_X + x * 2, BOARD_Y + y);
                    cout << "[]";
                }
            }
        }
    }
    
    // Hi?n th? thông tin
    setColor(15);
    gotoxy(BOARD_X + BOARD_WIDTH * 2 + 5, BOARD_Y);
    cout << "TETRIS GAME";
    gotoxy(BOARD_X + BOARD_WIDTH * 2 + 5, BOARD_Y + 2);
    cout << "Score: " << score << "   ";
    gotoxy(BOARD_X + BOARD_WIDTH * 2 + 5, BOARD_Y + 3);
    cout << "Level: " << level << "   ";
    gotoxy(BOARD_X + BOARD_WIDTH * 2 + 5, BOARD_Y + 4);
    cout << "Lines: " << linesCleared << "   ";
    
    gotoxy(BOARD_X + BOARD_WIDTH * 2 + 5, BOARD_Y + 7);
    cout << "Controls:";
    gotoxy(BOARD_X + BOARD_WIDTH * 2 + 5, BOARD_Y + 8);
    cout << "A - Left";
    gotoxy(BOARD_X + BOARD_WIDTH * 2 + 5, BOARD_Y + 9);
    cout << "D - Right";
    gotoxy(BOARD_X + BOARD_WIDTH * 2 + 5, BOARD_Y + 10);
    cout << "W - Rotate";
    gotoxy(BOARD_X + BOARD_WIDTH * 2 + 5, BOARD_Y + 11);
    cout << "S - Drop";
    gotoxy(BOARD_X + BOARD_WIDTH * 2 + 5, BOARD_Y + 12);
    cout << "ESC - Quit";
}

// Hàm chính
int main() {
    srand(time(0));
    hideCursor();
    
    // Thi?t l?p console
    system("title TETRIS GAME");
    system("mode con: cols=60 lines=30");
    
    newShape();
    
    int dropTime = 500; // Th?i gian roi (ms)
    clock_t lastDrop = clock();
    
    while(!gameOver) {
        // X? lý input
        if(_kbhit()) {
            char key = _getch();
            if(key == 27) { // ESC
                break;
            } else if(key == 'a' || key == 'A') {
                if(canMove(currentX - 1, currentY)) {
                    currentX--;
                }
            } else if(key == 'd' || key == 'D') {
                if(canMove(currentX + 1, currentY)) {
                    currentX++;
                }
            } else if(key == 'w' || key == 'W') {
                rotate();
            } else if(key == 's' || key == 'S') {
                while(canMove(currentX, currentY + 1)) {
                    currentY++;
                }
                score += 2;
            }
        }
        
        // Roi t? d?ng
        if(clock() - lastDrop > dropTime - (level - 1) * 50) {
            if(canMove(currentX, currentY + 1)) {
                currentY++;
            } else {
                mergeShape();
                clearLines();
                newShape();
            }
            lastDrop = clock();
        }
        
        drawBoard();
        Sleep(50);
    }
    
    // Game Over
    setColor(12);
    gotoxy(BOARD_X + BOARD_WIDTH - 4, BOARD_Y + BOARD_HEIGHT / 2);
    cout << "GAME OVER!";
    gotoxy(BOARD_X + BOARD_WIDTH - 5, BOARD_Y + BOARD_HEIGHT / 2 + 1);
    cout << "Final Score: " << score;
    
    setColor(15);
    gotoxy(0, BOARD_Y + BOARD_HEIGHT + 3);
    system("pause");
    
    return 0;
}
