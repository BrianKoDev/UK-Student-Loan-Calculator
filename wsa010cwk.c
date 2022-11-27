#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "functions.h"


//export to MATLAB for csv files
//Example for 40k starting salary
//[num,txt,raw]=xlsread('40000.csv');
//plot salary vs interest rate
//plot(num(:,1),num(:,3))
//plot salary vs monthly interest
//plot(num(:,1),num(:,4))
//plot salary vs monthly repayment
//plot(num(:,1),num(:,5))

//Conversation with Simon about variables should be in floats for a higher precision, also about having individual spreadsheets for each salary for better structure


void main(void) {
	
	calculate_loan(); //calculate total loan
	calculate_return(); //calculat total return
	scanf("%*d");

}









