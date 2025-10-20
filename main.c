#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <stdbool.h>
#define PI 3.14159265f


float isItValid(char parametr[], float min, float max, bool checkRange);
bool validationNumbersAfterPoint(float x);
float degreesToRadians(float degree);
float taylorCos(float x, float epsilon);
int correctCalculationForX1X2(float x, float epsilon);

int main() {
    char exitE = 0;

    printf("This program calculates cos(t) using Taylor series and comparing it with normal cos(x).\n");
    printf("You need enter: x1(start value), x2(final value), Dx(step) and E(precision).\n");
    printf("MAX input for x1, x2 and Dx = 1000\n");
    printf("MIN input for x1, x2 and Dx = -1000\n");
    printf("You can put ONLY 3 numbers after point!\n");

    do {
        float x1 = isItValid("x1 (degrees):", -1000.f,1000.f, true);
        float x2 = isItValid("x2 (degrees):", -1000.f,1000.f, true);
        float dx = isItValid("Dx (degrees):", -1000.f,1000.f, true);
        float epsilon = isItValid("E (ONLY 3 numbers after point): ", 1e-5f,1.f, false);


        if (x1 == x2 && dx != 0) {
            printf("Error: if x1 = x2, then step must be dx = 0\n");
            continue;
        }

        if (x1 != x2 && dx == 0) {
            printf("Error: if x1 != x2, then step must be dx != 0\n");
            continue;
        }

        if (x1 < x2 && dx <= 0) {
            printf("Error: if x1 < x2, then step must be dx > 0\n");
            continue;
        }

        if (x1 > x2 && dx >= 0) {
            printf("Error: if x1 > x2, then step must be dx < 0\n");
            continue;
        }

        printf("\n");
        printf("|       x      |    cos(t)    |    cos(x)    | cos(t)-cos(x)|\n");
        printf("|______________|______________|______________|______________|\n");

        if (dx == 0) {
            correctCalculationForX1X2(x1, epsilon);
        } else if (dx > 0) {
            for (float x = x1; x <= x2; x += dx) {
                correctCalculationForX1X2(x, epsilon);
            }
        } else {
            for (float x = x1; x >= x2; x += dx) {
                correctCalculationForX1X2(x, epsilon);
            }
        }

        printf("|______________|______________|______________|______________|\n");
        printf("\nPress 1 and ENTER to end or just ENTER to continue\n");
        exitE=getch();
    } while (exitE != '1');

    return 0;
}


float degreesToRadians(float degrees) {
    return degrees * PI / 180.f;
}

float taylorCos(float x, float epsilon) {
    float sum = 1.f;
    float delta = 1.f;
    float n = 1.f;

    while (fabsf(delta) >= epsilon) {
        delta = delta * (-x * x) / (2.0f * n * (2.0f * n - 1.0f));
        sum += delta;
        n += 1;
    }
    return sum;
}

int correctCalculationForX1X2(float x, float epsilon) {
    float xInRad = degreesToRadians(x);
    float taylor = taylorCos(xInRad, epsilon);
    float standard = cosf(xInRad);
    float difference = taylor - standard;

    return printf("| %12.5f | %12.5f | %12.5f | %12.5f |\n", x, taylor, standard, difference);
}


float isItValid(char parametr[], float min, float max, bool checkRange) {
    float number = 0.f;
    char extra = 0;
    int validInput = 1;
    do {
        printf("\n%s", parametr);
        float result = scanf(" %10f%c", &number, &extra);

        if (result !=2 || extra != '\n') {
            printf("ERROR: Invalid input. Please enter ONLY a number\n");
            while (getchar() != '\n'){}
            continue;
        }
        if (number < min || number > max) {
            printf("ERROR: Your value is not correct. Please enter a new number\n");
            continue;
        }
        if (checkRange && !validationNumbersAfterPoint(number)) {
            printf("ERROR: Your value is out of range(1e-3 to 1e3)\n");
            continue;
        }

        validInput = 0;
    } while (validInput);
    return number;
}

bool validationNumbersAfterPoint(float x) {
    if (x == 0.f) {
        return true;
    }
    float checkX = fabsf(x);
    if (checkX < 1e-3 || checkX > 1e3) {
        return false;
    }
    return true;
}
