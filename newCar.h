#ifndef NEWCAR_H
#define NEWCAR_H

#include "menu.h"
/**
@function newCar uj autot hoz letre.
@param customerList az ugyfelek listara mutato pointer.
@param carList az autok listara mutato pointer.
@return a program futhat-e tovabb vagy be kell zarni (memoriafoglalasi vagy fajlmegnyitasi hiba eseten).
**/
bool newCar(CustomerList *customerList, CarList *carList);
/**
@function readNumberPlate beolvassa a rendszamot.
@param readInSuccess cim szerint atadja a hivonak, hogy sikeres-e a beolvasas.
@return a beolvasott rendszam vagy hiba eseten NULL pointer.
**/
char *readNumberPlate(int *readInSuccess);
/**
@function writeToCarFile beirja az autok fajlba az autok listajat.
@param carList az autok listara mutatoo pointer.
@return sikeres volt-e a fajlbeolvasas.
**/
bool writeToCarFile(CarList *carList);
/**
@function insertCustomerAtEnd az autok listajanak vegere beszurja az uj autot.
@param carList az autok listara mutato pointer.
@param car az uj autora mutato pointer.
@return az uj autok listaja vagy hiba eseten NULL pointer.
**/
CarList *insertCarAtEnd(CarList *carList, Car *car);
/**
@function numberOfCars az autok lista elemeit szamolja meg.
@param carList az autok listara mutato pointer.
@return az autok lista elemeinek szama.
**/
unsigned int numberOfCars(const CarList *carList);
/**
@function numberOfCarsPerCustomer az ugyfel autok tombjenek elemeit szamolja meg.
@param customer az ugyfelre mutato pointer.
@return az ugyfelhez tartozo autok szama.
**/
int numberOfCarsPerCustomer(Customer *customer);
/**
@function readInPlateFromFile beolvassa a rendszamot a parameterkent kapott fajlbol, ';' karakterig vagy Enterig olvas.
@param fp a fajlra mutato pointer, amibol beolvas.
@return a beolvasott rendszam.
**/
char *readInPlateFromFile(FILE *fp);
/**
@function readCarListFromFile beolvassa a fajlban tarolt autok lista elemeinek adatait es elkesziti a duplan lancolt listat.
@return az elkeszult listara mutato pointer vagy hiba eseten NULL pointer.
**/
CarList *readCarListFromFile(const CustomerList *customerList);
/**
@function freeCarList felszabaditja az autok lista minden elemet, elemek rendszamat, markajat, javitaslistajat es az autok listat.
@param carList az autok listara mutato pointer.
**/
void freeCarList(CarList *carList);

#endif // NEWCAR_H
