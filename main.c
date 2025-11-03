#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <stdbool.h>
#define PI 3.14159265f


float isItValid(char textOutput[], float min, float max, bool checkRange);
bool validationNumbersAfterPoint(float x);
float roundTo5Decimals(float x);
float degreesToRadians(float degree);
float taylorCos(float x, float epsilon);
void correctCalculationForX1X2(float x, float epsilon);

int main() {
    char exitE = 0;

    printf("This program calculates cos(t) using Taylor series and comparing it with normal cos(x).\n");
    printf("You need enter: x1(start value), x2(final value), Dx(step) and E(precision).\n");

    do {
        float x1 = isItValid("x1 (Max = 1000; Min = -1000; degrees; Only 5 numbers after point):", -1e+3f,1e+3f, true);
        float x2 = isItValid("x2 (Max = 1000; Min = -1000; degrees; Only 5 numbers after point):", -1e+3f,1e+3f, true);
        float dx = isItValid("Dx (Max = 1000; Min = -1000; degrees; Only 5 numbers after point):", -1e+3f,1e+3f, true);

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

        float epsilon = isItValid("E (min: 1e-5; max: 1e-1): ", 1e-5f,1e-1f, false);


        printf("\n");
        printf("|       x      |    cos(t)    |    cos(x)    |  Difference  |\n");
        printf("|______________|______________|______________|______________|\n");

        if (dx == 0) {
            correctCalculationForX1X2(x1, epsilon);
        } else if (dx > 0) {
            int steps = (int)roundf((x2 - x1) / dx);
            for (int i = 0; i <= steps; i++) {
                float x = x1 + i * dx;
                correctCalculationForX1X2(x, epsilon);
            }
        } else {
            int steps = (int)roundf((x1 - x2) / (-dx));
            for (int i = 0; i <= steps; i++) {
                float x = x1 + i * dx;
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

float roundTo5Decimals(float x) {
    return roundf(x * 100000.0f) / 100000.0f;
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

void correctCalculationForX1X2(float x, float epsilon) {
    float xInRad = degreesToRadians(x);
    float taylor = taylorCos(xInRad, epsilon);
    float standard = cosf(xInRad);
    float difference = fabsf(taylor - standard);
    printf("| %12.5f | %12.5f | %12.5f | %12.5f |\n", x, taylor, standard, difference);
}

float isItValid(char textOutput[], float min, float max, bool checkRange) {
    float number = 0.f;
    char extra = 0;
    int validInput = 1;
    do {
        printf("\n%s", textOutput);
        unsigned short result = scanf(" %11f%c", &number, &extra);

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
            printf("ERROR: Your value after point is too small. Min possible number after point 1e-5\n");
            continue;
        }
        validInput = 0;
    } while (validInput);

    if (checkRange) {
        return roundTo5Decimals(number);
    }
    return number;
}

bool validationNumbersAfterPoint(float x) {
    float firstPart = 0.f;
    float secondPart = modff(fabsf(x), &firstPart);

    if (x == 0.f) {
        return true;
    }
    if (firstPart != 0.f) {
        return true;
    }
    if (secondPart < 1e-5f && secondPart != 0.f) {
        return false;
    }
    return true;
}