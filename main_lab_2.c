/*
 * Lab 2
 * Author: Charles Ganu
 * Date conducted: 2/12/2025
 * Description: Mid-level UI with persistent LCD output
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "MyRio.h"
#include "T1.h"


#define BUFSIZE 60
#define DEL 8
#define ENT 10
#define MINUS 45
#define DOT 46
#define UP 91
#define DN 93
#define ZERO 48
#define NINE 57

/* Prototypes */
int printf_lcd(const char *format, ...);
int putchar_lcd(int c);
char getkey(void);
int getchar_keypad(void);
char *fgets_keypad(char *buf, int buflen);

int main(int argc, char **argv) {
    NiFpga_Status status = MyRio_Open();
    if (MyRio_IsNotSuccess(status)) return status;

    char first_input[BUFSIZE], second_input[BUFSIZE];

    // First input
    printf_lcd("\fEnter 1st Number:");  // Clear screen only once
    if (fgets_keypad(first_input, sizeof(first_input)) != NULL) {
        printf_lcd("\nYou entered_1st: %s", first_input);  // \n for new line
    } else {
        printf_lcd("\nNo 1st input.");
    }

    // Second input
    printf_lcd("\nEnter 2nd Number:");  // No \f to retain previous text
    printf_lcd("\nEnter 2nd Number:");
        if (fgets_keypad(second_input, sizeof(second_input)) != NULL) {  //Corrected
            printf_lcd("\nYou entered_2nd: %s", second_input);
        } else {
            printf_lcd("\nNo 2nd input.");
        }

        MyRio_Close();
        return 0;
    }

char *fgets_keypad(char *buf, int buflen) {
    if (buflen < 1) return NULL;
    int n = 0;
    int c;

    while (n < buflen - 1) {
        c = getchar_keypad();
        if (c == EOF) break;  // Stop when buffer empty
        buf[n++] = (char)c;
    }

    buf[n] = '\0';
    return (n == 0) ? NULL : buf;
}

int getchar_keypad(void) {
    static char buffer[BUFSIZE];
    static int len = 0, pos = 0;
    static enum { Input, Output } state = Input;
    static int dotExists = 0;

    if (state == Output) {
        if (pos < len) return buffer[pos++];  // Return next char
        else {
            // Reset for new input
            state = Input;
            pos = len = 0;
            dotExists = 0;
            return EOF;  // Signal end of buffer
        }
    } else {
        while (1) {
            char ch = getkey();

            if (ch == ENT) {  // Handle ENTER
                if (len == 0) return EOF;
                state = Output;
                pos = 0;
                return buffer[pos++];  // Return first char
            } else if (ch == DEL) {  // Handle backspace
                if (len > 0) {
                    if (buffer[len - 1] == DOT) dotExists = 0;
                    len--;
                    printf_lcd("\b \b");  // Visual deletion
                }
            } else if (ch == MINUS) {  // Allow '-' only at start
                if (len == 0) {
                    buffer[len++] = ch;
                    putchar_lcd(ch);
                }
            } else if (ch == DOT) {  // Allow single decimal
                if (!dotExists) {
                    buffer[len++] = ch;
                    putchar_lcd(ch);
                    dotExists = 1;
                }
            } else if (ch >= ZERO && ch <= NINE) {  // Numbers
                if (len < BUFSIZE - 1) {
                    buffer[len++] = ch;
                    putchar_lcd(ch);
                }
            }
        }
        return EOF;
    }
}
