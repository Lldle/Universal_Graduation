/**
  ******************************************************************************
  * @file    Universal_Graduation.h
  * @brief   Universal graduation function settings file
  ******************************************************************************
  * @attention
  * You can use it but you should read README file firstly. 
  * Made by Lidle for you ^-^
  ******************************************************************************
  */
#ifndef __Universal_Graduation
#define __Universal_Graduation

#include "main.h"

#define Param_point_count       8

extern float Graduation_Memory_Table [3] [Param_point_count];

extern void Calculate_Graduation (int Cycles_Count);

extern float Get_Value_from_Grad ( float INPUT_VALUE);


#endif /*__Universal_Graduation*/