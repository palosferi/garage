#ifndef SEARCHBYPLATE_H
#define SEARCHBYPLATE_H

#include "menu.h"
/**
@function searchByPlate egy autot keres meg az autok listaban es kiirja az adatait es a hozza tartozo elkeszult javitasok adatait.
@param carList az autok listara mutato pointer.
@param repairList az javitasok listara mutato pointer.
@return a program futhat-e tovabb vagy be kell zarni (memoriafoglalasi hiba eseten).
**/
bool searchByPlate(CarList *carList, RepairList *repairList);
/**
@function carSearch egy autot keres meg az autok listaban.
@param carList az autok listara mutato pointer.
@param car az auto pointerre mutato pointer amit cim szerint betolt a hivo altal megadott valtozoba, ha sikerult megtalalni az autot.
@return a sikeresseg kodja vagy hibakod.
**/
int carSearch(CarList *carList, Car **car);

#endif //SEARCHBYPLATE_H
