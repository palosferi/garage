#ifndef REPAIRHISTORY_H
#define REPAIRHISTORY_H

#include "menu.h"
/**
@function repairHistory megjeleniti az elkeszult javitasokat idorendi (csokkeno) sorrendben.
@param repairList a javitasok listara mutato pointer.
@return a program futhat-e tovabb vagy be kell zarni (memoriafoglalasi hiba eseten).
**/
bool repairHistory(RepairList *repairList);

#endif //REPAIRHISTORY_H
