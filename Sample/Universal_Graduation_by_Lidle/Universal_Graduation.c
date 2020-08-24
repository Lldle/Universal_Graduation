/**
  ******************************************************************************
  * @file    Universal_Graduation.c
  * @brief   Universal graduation function file
  ******************************************************************************
  * @attention
  * You can use it but you should read README file firstly. 
  * Made by Lidle for you ^-^
  * 
  *  EMAIL:  shkitun-pavel@yandex.ru
  * 
  ******************************************************************************
  */

#include "Universal_Graduation.h"

// Graduation array
float Graduation_Memory_Table [ 3 ] [ Param_point_count ] = {                             //
-1000, 0,         0.5,           1,         1.5,           2,        2.5,           3,    // This data are used for tests.
   -5, 0,           1,           3,           6,           9,         14,          16,    // You will save your grad table here. 
   -5, 0, 0.297301779, 0.672728432, 1.321586352, 2.856189949, 8.32207129, 45.15943348     //  
};                                                                                        


// Function calculates correction values. 30 cycles is more than enough.
void Calculate_Graduation(int Cycles_Count)
{
  float Bezier_VALUE = 0;
  float Recalculated_Input_Value = 0;
  
  for (int Grad_Counter = 0;                                                               //
           Grad_Counter < Param_point_count;                                               // Setting the adjustment values first approximation.
           Grad_Counter++)                                                                 //
  {                                                                                        //
   Graduation_Memory_Table[2][Grad_Counter] = Graduation_Memory_Table[1][Grad_Counter];    //
  }                                                                                        //
  
  float * Bezier_input_value_Low    = &Graduation_Memory_Table[0][0];
  float * Bezier_input_value_Middle = &Graduation_Memory_Table[0][1];
  float * Bezier_input_value_High   = &Graduation_Memory_Table[0][2];
  
  //Imaginary values calc
        Graduation_Memory_Table[0][0] = 0;
        Graduation_Memory_Table[1][0] = 0;
        Graduation_Memory_Table[2][0] = 0;
  
  for (int Im_Counter = 1;
           Im_Counter < Param_point_count;
           Im_Counter++)  
  {
        Graduation_Memory_Table[0][0] -= Graduation_Memory_Table[0][Im_Counter];
        Graduation_Memory_Table[1][0] -= Graduation_Memory_Table[1][Im_Counter];
        Graduation_Memory_Table[2][0] -= Graduation_Memory_Table[2][Im_Counter];
  }
  
        Graduation_Memory_Table[0][0] /= Param_point_count;
        Graduation_Memory_Table[1][0] /= Param_point_count;
        Graduation_Memory_Table[2][0] /= Param_point_count;
  
  //Iterational corr calc
  float * Bezier_Polinom_value_Zero_Pointer       = &Graduation_Memory_Table[1][0];
  float * Bezier_Polinom_Ref_value_First_Pointer  = &Graduation_Memory_Table[1][1];
  float * Bezier_Polinom_value_First_Pointer      = &Graduation_Memory_Table[2][1];
  float * Bezier_Polinom_value_Second_Pointer     = &Graduation_Memory_Table[1][2];
  
  for (int Grad_Counter = 1; 
           Grad_Counter < (Param_point_count - 1); 
           Grad_Counter++)
  {
    int Calc_Cycle_Counter = Cycles_Count;
    
    while(Calc_Cycle_Counter > 0)
    {
       Recalculated_Input_Value = (*Bezier_input_value_Middle - *Bezier_input_value_Low) / 
                                  (*Bezier_input_value_High   - *Bezier_input_value_Low);

       Bezier_VALUE =   (*Bezier_Polinom_value_Zero_Pointer  ) * (1 - Recalculated_Input_Value) * (1 - Recalculated_Input_Value) * (1 - Recalculated_Input_Value) +
                    3 * (*Bezier_Polinom_value_First_Pointer ) * (1 - Recalculated_Input_Value) * (1 - Recalculated_Input_Value) * (    Recalculated_Input_Value) +
                    3 * (*Bezier_Polinom_value_First_Pointer ) * (1 - Recalculated_Input_Value) * (    Recalculated_Input_Value) * (    Recalculated_Input_Value) +  
                        (*Bezier_Polinom_value_Second_Pointer) * (    Recalculated_Input_Value) * (    Recalculated_Input_Value) * (    Recalculated_Input_Value);
       
       *Bezier_Polinom_value_First_Pointer = *Bezier_Polinom_value_First_Pointer - 
                                            (Bezier_VALUE - (*Bezier_Polinom_Ref_value_First_Pointer));
       Calc_Cycle_Counter--;
    }
    
    Bezier_input_value_Low++;    
    Bezier_input_value_Middle++; 
    Bezier_input_value_High++;   
    Bezier_Polinom_value_Zero_Pointer++;       
    Bezier_Polinom_Ref_value_First_Pointer++;  
    Bezier_Polinom_value_First_Pointer++;      
    Bezier_Polinom_value_Second_Pointer++;     
  }
}

// Function returns output grad value
float Get_Value_from_Grad ( float INPUT_VALUE)
{
  float   Recalculated_Input_Value_1 = 0;
  float   Recalculated_Input_Value_2 = 0;
  
  float * Input_Polinom_Table_Value = &Graduation_Memory_Table[0][0];

  while (INPUT_VALUE > *Input_Polinom_Table_Value)
  {
       Input_Polinom_Table_Value++;
  }
  //lamers fix
  if (Input_Polinom_Table_Value >= &Graduation_Memory_Table[0][Param_point_count - 1]) return Graduation_Memory_Table [ 1 ] [ Param_point_count - 2];
  
  
  //recalculated input value 1 lower middle
  float * Bezier_input_value_1_High = Input_Polinom_Table_Value + 1;
  float * Bezier_input_value_1_Low  = Input_Polinom_Table_Value - 1;

  Recalculated_Input_Value_1 = (INPUT_VALUE                - *Bezier_input_value_1_Low) / 
                               (*Bezier_input_value_1_High - *Bezier_input_value_1_Low);
  
  //Bezier polinom values
  float * Bezier_Polinom_value_1_Zero_Pointer   = Input_Polinom_Table_Value - 1 + Param_point_count;
  float * Bezier_Polinom_value_1_First_Pointer  = Input_Polinom_Table_Value + 2 * Param_point_count;
  float * Bezier_Polinom_value_1_Second_Pointer = Input_Polinom_Table_Value + 1 + Param_point_count;
  
  //recalculated input value 2 higher middle
  Input_Polinom_Table_Value--;
  
  float * Bezier_input_value_2_High = Input_Polinom_Table_Value + 1;
  float * Bezier_input_value_2_Low  = Input_Polinom_Table_Value - 1;

  Recalculated_Input_Value_2 = (INPUT_VALUE - *Bezier_input_value_2_Low) / 
                               (*Bezier_input_value_2_High - *Bezier_input_value_2_Low);
  
  //Bezier polinom values
  float * Bezier_Polinom_value_2_Zero_Pointer   = Input_Polinom_Table_Value - 1 + Param_point_count;
  float * Bezier_Polinom_value_2_First_Pointer  = Input_Polinom_Table_Value + 2 * Param_point_count;
  float * Bezier_Polinom_value_2_Second_Pointer = Input_Polinom_Table_Value + 1 + Param_point_count;
  
  float Bezier_VALUE_1 =     (*Bezier_Polinom_value_1_Zero_Pointer  ) * (1 - Recalculated_Input_Value_1) * (1 - Recalculated_Input_Value_1) * (1 - Recalculated_Input_Value_1) +
                         3 * (*Bezier_Polinom_value_1_First_Pointer ) * (1 - Recalculated_Input_Value_1) * (1 - Recalculated_Input_Value_1) * (    Recalculated_Input_Value_1) +
                         3 * (*Bezier_Polinom_value_1_First_Pointer ) * (1 - Recalculated_Input_Value_1) * (    Recalculated_Input_Value_1) * (    Recalculated_Input_Value_1) +  
                             (*Bezier_Polinom_value_1_Second_Pointer) * (    Recalculated_Input_Value_1) * (    Recalculated_Input_Value_1) * (    Recalculated_Input_Value_1);
  
  float Bezier_VALUE_2 =     (*Bezier_Polinom_value_2_Zero_Pointer  ) * (1 - Recalculated_Input_Value_2) * (1 - Recalculated_Input_Value_2) * (1 - Recalculated_Input_Value_2) +
                         3 * (*Bezier_Polinom_value_2_First_Pointer ) * (1 - Recalculated_Input_Value_2) * (1 - Recalculated_Input_Value_2) * (    Recalculated_Input_Value_2) +
                         3 * (*Bezier_Polinom_value_2_First_Pointer ) * (1 - Recalculated_Input_Value_2) * (    Recalculated_Input_Value_2) * (    Recalculated_Input_Value_2) +  
                             (*Bezier_Polinom_value_2_Second_Pointer) * (    Recalculated_Input_Value_2) * (    Recalculated_Input_Value_2) * (    Recalculated_Input_Value_2);

  return ((Bezier_VALUE_1 + Bezier_VALUE_2) / 2);
}