#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "menu.h"
#include "newCustomer.h"
#include "newCar.h"
#include "searchByPlate.h"
#include "deleteCar.h"

int carDelete(CarList *carList);
/*a main hivja meg, parameterul kapja az autok es javitasok listat, visszaadja, hogy a program futhat-e tovabb, vagy ki kell leptetni.
Meglevo aktiv autot deaktival.*/
bool deleteCar(CarList *carList) {
    if(carList -> head == NULL) {
        printf("Nincs auto az adatbazisban.\n");
        exitOnly();
        return true;
    }
    int carDeleteSuccess = carDelete(carList);
    while(carDeleteSuccess != 2) {
        if(carDeleteSuccess == -1)
            return false;
        else if(carDeleteSuccess == 0)
            return true;
        carDeleteSuccess = carDelete(carList);
    }
    return true;
}

int carDelete(CarList *carList) {
    Car *searchedCar = NULL;
    int carSearchSuccess = carSearch(carList, &searchedCar);
    while(carSearchSuccess != 2) {
        if(carSearchSuccess == -1)
            return -1;
        else if(carSearchSuccess == 0)
            return 0;
        carSearchSuccess = carSearch(carList, &searchedCar);
    }
    if(searchedCar -> active == 0) {
        printf("Az auto mar torolve van.\n");
        return 0;
    }
    searchedCar -> active = 0;
    if(writeToCarFile(carList)) {
        printf("Sikeresen torolt auto.\n");
        exitOnly();
        return 2;
    }
    else {
        fileOpenError();
        return -1;
    }
}
