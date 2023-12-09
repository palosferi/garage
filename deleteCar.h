#ifndef DELETECAR_H
#define DELETECAR_H

#include "menu.h"
/**
@function deleteCar deaktival egy autot.
@param carList az autok listara mutato pointer.
@return a program futhat-e tovabb vagy be kell zarni (memoriafoglalasi vagy fajlmegnyitasi hiba eseten).
**/
bool deleteCar(CarList *carList);

#endif //DELETECAR_H
