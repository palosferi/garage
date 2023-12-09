#ifndef SOONEXPIRES_H
#define SOONEXPIRES_H

#include "menu.h"
/**
@function soonExpires kilistazza a 60 napon beluli lejaratu autokat.
@param carList az autok listara mutato pointer.
@return a program futhat-e tovabb vagy be kell zarni (memoriafoglalasi hiba eseten).
**/
bool soonExpires(CarList *carList);
/**
@function daysDifference kiszamolja a ket parameterul kapott datum kozti napok szamat.
@param date1 az egyik datum, amit atvalt napszamba.
@param date2 a masik datum, amit atvalt napszamba.
@return a ket datum kozti napok szama.
**/
int daysDifference(Date date1, Date date2);

#endif //SOONEXPIRES_H
