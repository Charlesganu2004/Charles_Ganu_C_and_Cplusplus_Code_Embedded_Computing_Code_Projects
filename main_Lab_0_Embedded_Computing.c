/*
 * Lab 0
 * Author: Charles Ganu
 * Date conducted: 1/22/2025
 * Description: Calculator UI with persistent LCD output
 */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "MyRio.h"
#include "T1.h"

/* Prototypes */
int printf_lcd(const char *format, ...);
char* strpbrk(const char *s1, const char *s2);
char* strchr(const char *s, int c);   // first occurrence of c in s
//char* strrchr(const char *s, int c);  // last occurrence of c in s
char* fgets_keypad(char *buf, int bufflen);
double double_in(char *prompt);

int main(int argc, char **argv)
{
	NiFpga_Status status;

	status = MyRio_Open();
	if (MyRio_IsNotSuccess(status)) {
	}
	double double_in(char *prompt);
	double velocity = double_in("Enter Vel:");
	double pressure = double_in("Enter Pres:");

	printf_lcd("\fVelocity = %f\nPressure = %f", velocity, pressure);



	status = MyRio_Close();
	return status;
}

double double_in(char *prompt) {
    char buffer[40];
    double value;
    int err = 1;

    while (err) {
        printf_lcd("\v%s", prompt);

        if (!fgets_keypad(buffer, sizeof(buffer))) {
            printf_lcd("\f\nShort. Try Again");
            continue;
        }

        // Trim newline
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[--len] = '\0';
        }

        // Input validation checks
        if (len == 0) {
            printf_lcd("\f\nShort. Try Again.");
            continue;
        }

        if (strpbrk(buffer, "[]")) {
            printf_lcd("\f\nBad Key. Try Again.");
            continue;
        }
        if (strstr(buffer, "..")){
        	printf_lcd("\f\nBad Key. Try Again");
        }
        if (strchr(buffer + 1, '-')) {
            printf_lcd("\f\nBad Key. Try Again.");
            continue;
        }

       // if (strchr(buffer, '.') != strrchr(buffer, '.')) {
        //    printf_lcd("\f\nBad Key. Try Again.");
       //     continue;
      //  }

        if (sscanf(buffer, "%lf", &value) != 1) {
            printf_lcd("\f\nInvalid Number");
            continue;
        }

        err = 0;
    }
    return value;
}
