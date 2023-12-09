#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "debugmalloc.h"
#include "menu.h"
#include "newCustomer.h"
#include "newCar.h"
#include "repairs.h"
#include "soonExpires.h"

int getDaysFromMonth(int month);
/*a main hivja meg, parameterul kapja az autok es javitasok listat, visszaadja, hogy a program futhat-e tovabb, vagy ki kell leptetni.
Kiirja a 60 napon beluli lejaratu autokat.*/
bool soonExpires(CarList *carList) {
    int existCheck = 0;
    Car* tempCar;
    Date today = getCurrentDate();
    printf("Hamarosan lejaro vizsgaju autok:\n\n");
    for (tempCar = carList -> head; tempCar != NULL; tempCar = tempCar -> next) {
        if(daysDifference(tempCar -> testDate, today) <= 60) {
            printf("%u %s %s %s %s %d/%d/%d\n", tempCar -> owner -> ID, tempCar -> owner -> surName, tempCar -> owner -> firstName,\
                tempCar -> numberPlate, tempCar -> brand, tempCar -> testDate.year, tempCar -> testDate.month, tempCar -> testDate.day);
            ++existCheck;
        }
        if (tempCar == carList -> tail)
            break;
    }
    if (existCheck == 0)
        printf("Nincs hamarosan lejaro vizsgaju auto.\n");
    exitOnly();
    return true;
}

int daysDifference(Date date1, Date date2) {
    int days1 = date1.year * 365;
    int days2 = date2.year * 365;
    days1 += date1.year/4;
    days1 -= date1.year/100;
    days1 += date1.year/400;
    days2 += date2.year/4;
    days2 -= date2.year/100;
    days2 += date2.year/400;
    if((date1.year%4 == 0 && date1.year%100 != 0) || date1.year%400 == 0) {
        if(date1.month < 3 || (date1.month == 2 && date1.day < 29))
            --days1;
    }
    if((date2.year%4 == 0 && date2.year%100 != 0) || date2.year%400 == 0) {
        if(date2.month < 3 || (date2.month == 2 && date2.day < 29))
            --days1;
    }
    days1 += getDaysFromMonth(date1.month);
    days2 += getDaysFromMonth(date2.month);
    days1 += date1.day;
    days2 += date2.day;
    return days1 - days2;
}

int getDaysFromMonth(int month) {
    int days = 0;
    switch (month) {
        case 1: break;
        case 2: days += 31; break;
        case 3: days += 31+28; break;
        case 4: days += 31+28+31; break;
        case 5: days += 31+28+31+30; break;
        case 6: days += 31+28+31+30+31; break;
        case 7: days += 31+28+31+30+31+30; break;
        case 8: days += 31+28+31+30+31+30+31; break;
        case 9: days += 31+28+31+30+31+30+31+31; break;
        case 10: days += 31+28+31+30+31+30+31+31+30; break;
        case 11: days += 31+28+31+30+31+30+31+31+30+31; break;
        case 12: days += 31+28+31+30+31+30+31+31+30+31+30; break;
    }
    return days;
}
