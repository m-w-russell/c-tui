#include "tui.h"

struct winsize sz;
struct termios tty;
Button_t* selected;


void buttonTestFunction(int buttonId, Cursor_t* cursor) {
    shiftCursor(0, 50, cursor);
    printf("Button %d pushed", buttonId);

}


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

Window_t initWindow(Display_t* display) {
    Window_t newWindow = {display->width - 4, display->height - 4, 0, 0, "Test Window"};
    return newWindow;
}

Button_t initButton(int id, char* text, int x, int y) {
    Button_t newButton = {&buttonTestFunction, id, text, x, y};
    return newButton;

}

void enableRaw() {
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void disableRaw() {
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= (ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &tty); 
}

void enableEcho() {
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void disableEcho() {
    tcgetattr(STDIN_FILENO, &tty);          
    tty.c_lflag &= ~ECHO;                   
    tcsetattr(STDIN_FILENO, TCSANOW, &tty); 
}



void renderWindow(Display_t* display, Cursor_t* cursor, Window_t* window) {
    int windowWidth = window->width;
    int windowHeight = window->height;
    int gap = (display->width - windowWidth) / 2;
    window->x = gap * 2 + 1;
    window->y = gap + 1;

    //Draw blue backdrop
   for (int i = 0; i < display->height; i++) {
        for (int j = 0; j < display->width; j++) {
            printf("\e[38;5;19m\u2588");
        }
        printf("\n");
    }

    //Draw window
    shiftCursor(window->x, window->y, cursor);
    for (int i = 0; i < windowHeight; i++) {
        for (int j = 0; j < windowWidth - (gap * 2); j++) {
            printf("\e[38;5;251m\u2588");
        }
        shiftCursor(gap * 2 + 1, gap + 1 + i, cursor);
    }

    renderTitlebar(window, cursor, gap);
    printf("\e[0;0m");
    shiftCursor(0, 21, cursor);
    shiftCursor(0, 21, cursor);
    renderWindowShadow(windowWidth, windowHeight, display, cursor);
    char* text = "|> Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
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

void renderTitlebar(Window_t* window, Cursor_t* cursor, int gap) {
    shiftCursor(window->x, window->y-1, cursor);
    int titleLen = strlen(window->title);
    printf("\e[48;5;214m\e[38;5;235m");
    int segment = (window->width - gap * 2 - titleLen - 4)/2;
    for(int i = 0; i<segment;i++) {
        printf("=");
    }
    printf("  %s  ", window->title);
    for(int i = 0; i<segment;i++) {
        printf("=");
    }

    if ((window->width - gap * 2 - titleLen - 4) % 2 != 0) {
        printf("=");
    }

/*
    if ((window->width - titleLen - 3 - gap * 2)%2 != 0) {
        printf("=");
    }
  */  
    
}

void writeTextToWindow(char* text, int width, int height, Display_t* display, Cursor_t* cursor) {
    int gap = (display->width - width) / 2;
    shiftCursor(gap * 2 + 2, gap + 2, cursor);
    printf("\e[48;5;251m\e[38;5;235m");
    //printf("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
    Sliced_t rows = sliceStringIntoRows(text, width-6);
    //printf("%s", rows.strings[0]);
    
    for (int i = 0; i < rows.quantity; i++) {
        printf("%s", rows.strings[i]);
        free(rows.strings[i]);
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

void renderButton(Button_t* button, Cursor_t* cursor) {
    shiftCursor(button->x, button->y, cursor);
    printf("| ");
    printf("%s", button->text);
    printf(" |");

}


void shiftCursor(int x, int y, Cursor_t* cursor) {
    printf("\e[%d;%dH", y, x);
    cursor->x = x;
    cursor->y = y;
}

int nextIndex(int size, int current) {
    if (current + 1 > size) {
        return 0;
    } else {
        current++;
        return current;
    }
}

int main() {
    printf("\e[2J\e[H\e[?25l");
    
    ioctl( 0, TIOCGWINSZ, &sz );

    disableEcho();
    
    Display_t disp = initDisplay(sz.ws_col, sz.ws_row);
    Window_t window = initWindow(&disp);
    Cursor_t cursor = initCursor();
    Button_t buttonA = initButton(0, "Test Button", 10, 40);
    Button_t buttonB = initButton(1, "Test Button 2", 30, 40);
    selected = &buttonA;
    renderWindow(&disp, &cursor, &window);
    renderButton(&buttonA, &cursor);
    renderButton(&buttonB, &cursor);
    Button_t* buttons[2] = {&buttonA, &buttonB};

    shiftCursor(0, sz.ws_row + 1, &cursor);

    int exit = 0;
    int current = 0;
    while(1) {
        enableRaw();
        char c;
        c = getchar();  // Read a single character
        
        switch(c) {
            case 'q':
                enableEcho();
                disableRaw();
                exit = 1;
                break;
            case 'f':
                selected->action(selected->buttonId, &cursor);
                break;
            case 'n':
                current = nextIndex(1, current);

                selected = buttons[current];
                break;
        }
        
        if (exit) { // Exit on 'q'
            shiftCursor(0, disp.height, &cursor);
            break;

        }
        disableRaw();
    
    }
    printf("\e[0;0m\n");


}