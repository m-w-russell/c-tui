#include "tui.h"


Display_t initDisplay(int width, int height) {
    Display_t newDisplay;
    newDisplay.width = width;
    newDisplay.height = height;
    return newDisplay;
}

Cursor_t initCursor() {
    Cursor_t newCursor;
    newCursor.x = 0;
    newCursor.y = 0;
    return newCursor;
}

void renderWindow(Display_t* display, Cursor_t* cursor) {
    int windowWidth = display->width - 4;
    int windowHeight = display->height - 4;
    int gap = (display->width - windowWidth) / 2;
    /*
    for (int i = 0; i < display->height; i++) {
        for (int j = 0; j < display->width; j++) {
            if (j >= gap*2 && j <= windowWidth-1 && i >= gap && i <= windowHeight+1) {
                printf("\e[38;5;251m\u2588");
            } else {
                printf("\e[38;5;21m\u2588");
            }
            cursor->x++;
            
        }
        cursor->y++;
        printf("\n");
        cursor->x = 0;
    }
    */

   for (int i = 0; i < display->height; i++) {
        for (int j = 0; j < display->width; j++) {
            printf("\e[38;5;19m\u2588");
        }
        printf("\n");
    }

    shiftCursor(gap * 2 + 1, gap + 1, cursor);
    for (int i = 0; i < windowHeight; i++) {
        for (int j = 0; j < windowWidth - (gap * 2); j++) {
            printf("\e[38;5;251m\u2588");
        }
        shiftCursor(gap * 2 + 1, gap + 1 + i, cursor);
    }

    shiftCursor(gap * 2 + 1, gap, cursor);
    char windowTitle[] = "Test Window";
    int titleLen = strlen(windowTitle);
    printf("\e[48;5;214m\e[38;5;235m");
    int segmentWidth = (windowWidth-8 - titleLen + 2) / 2;
    for (int i = 0; i<segmentWidth; i++) {
        printf("=");
    }
    printf(" %s ", windowTitle);
    for (int i = 0; i<segmentWidth; i++) {
        printf("=");
    }
    if(titleLen % 2 != 0) {
        printf("=");
    }
    printf("\e[0;0m");
    shiftCursor(0, 21, cursor);
    shiftCursor(0, 21, cursor);
    renderWindowShadow(windowWidth, windowHeight, display, cursor);
    char* text = "Content String";
    writeTextToWindow(text, windowWidth, windowHeight, display, cursor);
    printf("\n");
    
}


void renderWindowShadow(int width, int height, Display_t* display, Cursor_t* cursor) {
    int gap = (display->width - width) / 2;
    for (int i = 0; i<height-1; i++) {
        shiftCursor((gap*2), gap + 1 + i, cursor);
        printf("\e[38;5;232m\u2588");
    }
    shiftCursor((gap*2), height+gap, cursor);
    for(int i = 0; i<width-gap-3; i++) {
        printf("\e[48;5;19m\e[38;5;232m\u2580");
    }
    
}

void writeTextToWindow(char* text, int width, int height, Display_t* display, Cursor_t* cursor) {
    int gap = (display->width - width) / 2;
    shiftCursor(gap * 2 + 2, gap + 2, cursor);
    printf("\e[48;5;251m\e[38;5;235m");
    //printf("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
    Sliced_t rows = sliceStringIntoRows("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", width-6);
    //printf("%s", rows.strings[0]);
    
    for (int i = 0; i < rows.quantity; i++) {
        printf("%s", rows.strings[i]);
        shiftCursor(gap * 2 + 2, gap + 3 + i, cursor);
    }
    

}

Sliced_t sliceStringIntoRows(char* str, int width) {
    int numStr = strlen(str) / width;
    if (strlen(str) % width != 0) {
        numStr++;
    }
    char** strings = (char**)malloc(numStr * sizeof(char*));

    for (int i = 0; i<numStr; i++) {
        char slice[width + 1];
        strncpy(slice, str + (i * width), width);
        slice[width] = '\0';
        strings[i] = malloc(width + 1);
        strcpy(strings[i], slice);

    }

    Sliced_t result;
    result.strings = strings;
    result.quantity = numStr;
    return result;
}


void shiftCursor(int x, int y, Cursor_t* cursor) {
    /*
    int shiftX = x - cursor->x;
    int shiftY = y - cursor->y;
    if (shiftX <= 0) {
        printf("\e[%dD", -shiftX);
    } else {
        printf("\e[%dC", shiftX);
    }

    if (shiftY < 0) {
        printf("\e[%dA", -shiftY);
    } else {

    }
        printf("\e[%dB", shiftY);
    cursor->x += shiftX;
    cursor->y += shiftY;
    */
    printf("\e[%d;%dH", y, x);
    cursor->x = x;
    cursor->y = y;
}

int main() {
    printf("\e[2J\e[H");
    Display_t disp = initDisplay(100, 30);
    Cursor_t cursor = initCursor();
    renderWindow(&disp, &cursor);

    shiftCursor(0, 31, &cursor);
    printf("\e[0;0m\n");
}