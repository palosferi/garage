#ifndef REPAIRS_H
#define REPAIRS_H

#include "menu.h"
/**
@function repairs uj javitast vesz fel vagy meglevot keszkent jelol.
@param carList az autok listara mutato pointer.
@param repairList a javitasok listara mutato pointer.
@return a program futhat-e tovabb vagy be kell zarni (memoriafoglalasi vagy fajlmegnyitasi hiba eseten).
**/
bool repairs(CarList *carList, RepairList *repairList);
/**
@function insertRepairAtEnd a javitasok listajanak vegere beszurja az uj javitast.
@param repairList a javitasok listara mutato pointer.
@param newRepair az uj javitasra mutato pointer.
@return az uj javitasok listara mutato pointer.
**/
RepairList *insertRepairAtEnd(RepairList *repairList, Repair *newRepair);
/**
@function numberOfRepairs a javitasok lista elemeit szamolja meg.
@param repairList a javitasok listara mutato pointer.
@return a javitasok lista elemeinek szama.
**/
unsigned int numberOfRepairs(const RepairList *repairList);
/**
@function numberOfRepairsPerCar az auto javitasok tombjenek elemeit szamolja meg.
@param car az autora mutato pointer.
@return az autohoz tartozo javitasok szama.
**/
int numberOfRepairsPerCar(Car *car);
/**
@function getCurrentDate a mai datumot szamolja ki.
@return a mai datum, Date strukturaba toltve.
**/
Date getCurrentDate();
/**
@function readRepairListFromFile beolvassa a fajlban tarolt javitasok lista elemeinek adatait es elkesziti a duplan lancolt listat.
@param carList az autok listara mutato pointer.
@return az elkeszult listara mutato pointer vagy hiba eseten NULL pointer.
**/
RepairList *readRepairListFromFile(const CarList *carList);
/**
@function freeRepairList felszabaditja a javitasok lista minden elemet es a javitasok listat.
@param repairList a javitasok listara mutato pointer.
**/
void freeRepairList(RepairList *repairList);

#endif //REPAIRS_H
