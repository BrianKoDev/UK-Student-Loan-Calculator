#define _CRT_SECURE_NO_WARNINGS



#include <stdio.h>
#include <math.h>
#include <string.h>
#include "functions.h"

//define fixed values for calculating loan
#define ANNUAL_FEE  9250 //Source: https://www.lboro.ac.uk/students/finance/ug/fees/
#define PLACEMENT_FEE  1850 //Source: https://www.lboro.ac.uk/students/finance/ug/fees/
#define MAX_MAIN_LOAN 8700 //Source: https://www.gov.uk/student-finance-calculator
#define MIN_MAIN_LOAN  4054 //Source: https://www.gov.uk/student-finance-calculator
#define INTEREST_WHILE_STUDYING 0.063 //Source https://www.gov.uk/repaying-your-student-loan/what-you-pay

//define fixed values for calculating return
#define RPI  0.033 //aka annual inflation - Source https://www.gov.uk/repaying-your-student-loan/what-you-pay
#define MAX_ANNUAL_INTEREST  0.03 //Source: https://www.gov.uk/repaying-your-student-loan/what-you-pay
#define ANNUAL_SALARY_INC  0.039 //Source: http://uk.businessinsider.com/uk-pay-wages-payrise-bank-of-england-survey-2018-2
#define SALARY_THRESHOLD  25000 //Source https://www.gov.uk/repaying-your-student-loan/what-you-pay
#define REPAYMENT_RATE  0.09 //Source https://www.gov.uk/repaying-your-student-loan/what-you-pay
#define MAX_LOAN_YR 30 //Source https://www.gov.uk/repaying-your-student-loan/what-you-pay

//define initial salary range
#define INT_SALARY_MIN  20000
#define INT_SALARY_MAX  60000
#define SALARY_STEP  5000


//global variables
int first_payment_yr;
int total_loan = 0;

void calculate_loan() {
	//define initial values
	int num_taught_yr=0;
	int placement_yr=0;
	int annual_income=0;
	int maintenance_loan=0;
	int starting_mon=0;
	int starting_yr=0;
	int graduation_yr=0;
	int total_taught_fee=0;
	int total_placement_fee=0;
	int total_maintenance_loan=0;

	get_userdata(&num_taught_yr,&placement_yr,&maintenance_loan,&annual_income,&starting_mon,&starting_yr); //get user input data on number of taught years, placement years...etc
	calculate_results(&graduation_yr, &total_taught_fee, &total_placement_fee, &total_maintenance_loan, starting_yr, placement_yr, num_taught_yr, maintenance_loan);//calculate total loan
	print_results(starting_mon,graduation_yr,total_taught_fee,total_placement_fee,total_maintenance_loan); //print results to screen
	export_results(maintenance_loan, num_taught_yr, placement_yr, starting_mon, starting_yr, graduation_yr, first_payment_yr, total_taught_fee, total_placement_fee, total_maintenance_loan); //export results to txt file
}

void get_userdata(int*num_taught_yr,int*placement_yr,int*maintenance_loan,int*annual_income,int*starting_mon,int*starting_yr) {
	printf("Please enter the number of taught years in the course: ");
	scanf("%d",num_taught_yr);
	while ((*num_taught_yr > 4) || (*num_taught_yr < 0)) { //Input validation - ensure that number of taught years is at least 1 and less than 4
		printf("Incorrect value, Please re-enter");
		printf("\nPlease enter the number of taught years in the course: ");
		scanf("%d", num_taught_yr);
	}

	printf("Please enter 1 if placement year is to be taken or 0 if not: ");
	scanf("%d", placement_yr);
	while ((*placement_yr != 1) && (*placement_yr != 0)) { //Input validation - ensure placement year is either one year or none
		printf("Incorrect value, Please re-enter");
		printf("\nPlease enter 1 if placement year is to be taken or 0 if not: ");
		scanf("%d", placement_yr);
	}

	printf("Please enter your annual household income: ");
	scanf("%d", annual_income);
	*maintenance_loan = (int)round((-0.1248458180*(*annual_income)) + 11821.616); //reverse engineered from https://www.gov.uk/student-finance-calculator to calcualte maximum mainintnance loan based on annual income
	if (*maintenance_loan > 8700) {
		*maintenance_loan = 8700;
	}
	if (*maintenance_loan < 4054) {
		*maintenance_loan = 4054;
	}

	printf("Please enter the month you are starting your course: ");
	scanf("%d", starting_mon);
	while ((*starting_mon > 12) || (*starting_mon < 0)) {//Input validation  - ensure user enters a valid month
		printf("Incorrect value, Please re-enter");
		printf("\nPlease enter the month you are starting your course: ");
		scanf("%d", starting_mon);
	}

	printf("Please enter the year you are starting your course: ");
	scanf("%d", starting_yr);
	while (*starting_yr < 2000) { //Input validation - ensure user is entering a valid year
		printf("Incorrect value, Please re-enter");
		printf("\nPlease enter the year you are starting your course: ");
		scanf("%d", starting_yr);
	}
}

void calculate_results(int*graduation_yr, int*total_taught_fee, int*total_placement_fee, int*total_maintenance_loan,int starting_yr, int placement_yr, int num_taught_yr,  int maintenance_loan) {
	*graduation_yr = starting_yr + placement_yr + num_taught_yr; //calculate graduation year based on number of taught years and placement year
	first_payment_yr = *graduation_yr + 1;
	*total_taught_fee = num_taught_yr * ANNUAL_FEE;
	*total_placement_fee = (int)placement_yr * PLACEMENT_FEE;
	*total_maintenance_loan = (num_taught_yr + placement_yr)*(maintenance_loan);
	total_loan =(int)(*total_maintenance_loan + *total_taught_fee + *total_placement_fee)*(int)pow((1+INTEREST_WHILE_STUDYING),(placement_yr+num_taught_yr));//total loan including compound interest while studying
}
	
void print_results(int starting_mon,int graduation_yr,int total_taught_fee,int total_placement_fee,int total_maintenance_loan) {
	printf("Graduation Date: %d/%d", starting_mon, graduation_yr);
	printf("\nFirst Payment Date: 04/%d",first_payment_yr);
	printf("\nTotal taught fee: %d", total_taught_fee);
	printf("\nTotal placement fee: %d", total_placement_fee);
	printf("\nTotal maintenance loan: %d", total_maintenance_loan);
	printf("\nTotal loan: %d", total_loan);
}

void export_results(int maintenance_loan, int num_taught_yr, int placement_yr, int starting_mon, int starting_yr, int graduation_yr, int first_payment_yr, int total_taught_fee, int total_placement_fee, int total_maintenance_loan) {
	FILE *exportdata = fopen("loancalc.txt", "w"); //create text file and write to it
	fprintf(exportdata, "Annual Fee: %d ",ANNUAL_FEE); 
	fprintf(exportdata, "\nPlacement Fee: %d ",PLACEMENT_FEE); 
	fprintf(exportdata, "\nAnnual Maintenance: %d", maintenance_loan);
	fprintf(exportdata, "\nTaught Years: %d", num_taught_yr);
	fprintf(exportdata, "\nPlacement Year: %d", placement_yr);
	fprintf(exportdata, "\nStart Date: %d/%d", starting_mon, starting_yr);
	fprintf(exportdata, "\nGraduation Date: %d/%d", starting_mon, graduation_yr);
	fprintf(exportdata, "\nFirst Payment Date: 04/%d", first_payment_yr);
	fprintf(exportdata, "\nTaught Fees: %d", total_taught_fee);
	fprintf(exportdata, "\nPlacement Fees: %d", total_placement_fee);
	fprintf(exportdata, "\nMaintenance Loan: %d", total_maintenance_loan);
	fprintf(exportdata, "\nTotal Loan: %d", total_loan);
	fclose(exportdata); //close file to ensure data is saved
}

void calculate_return() {
	//initialising arrays to hold generated data
	int mon[400];
	int year[400];
	int salary[400];
	int salary_abv_threshold[400];
	float interest_rate[400];
	int mon_interest[400];
	int mon_repayment[400];
	int remaining_bal[400];
	int initial_salary;

	for (initial_salary = INT_SALARY_MIN; initial_salary < INT_SALARY_MAX; initial_salary = initial_salary + SALARY_STEP) { //loop to calculate values for each of the starting salaries
		
		int exitflag = 0; //flag to exit while loop if 30 years are passed or loan has been repayed
		int current_row = 1;

		//initialise starting values
		mon[0] = 04;
		year[0] = first_payment_yr;
		salary[0] = initial_salary;
		interest_rate[0] = (float)(RPI + (((0.00015*salary[0]) - 3.75) / 100));
		if (interest_rate[0] >(RPI + 0.03)) { //ensure interest rate does not exceed threshold
			interest_rate[0] = (float)(RPI + 0.03);
		}
		if (interest_rate[0] <(RPI)) { //ensure interest rate is not threshold
			interest_rate[0] = (float)(RPI);
		}

		mon_interest[0] = (int)round((total_loan * interest_rate[0]) / 12);
		
		if (salary[0] > SALARY_THRESHOLD) {//calculate interest rate for first year
			salary_abv_threshold[0] = salary[0] - SALARY_THRESHOLD;
			mon_repayment[0] = (int)round(((salary_abv_threshold[0] * REPAYMENT_RATE) / 12));
		}
		else { //if salary is not above threshold then no repayment is required
			salary_abv_threshold[0] = 0;
			mon_repayment[0] = 0;
		}

		remaining_bal[0] = total_loan - mon_repayment[0] + mon_interest[0]; //calculate remaining balance

		//loop until balance is 0 or maximum loan year reached
		while ((current_row < (MAX_LOAN_YR * 12)) && (exitflag == 0)) {
			if (mon[current_row - 1] < 12) { //check if a year is completed
				mon[current_row] = mon[current_row - 1] + 1; //increase month by 1
				year[current_row] = year[current_row - 1]; //year stays the same
				salary[current_row] = salary[current_row - 1]; //salary remains the same
			}
			else {
				year[current_row] = year[current_row - 1] + 1;//increase year by 1
				mon[current_row] = 01;//restart month
				salary[current_row] = (int)(round(salary[current_row - 1] * (1 + ANNUAL_SALARY_INC)));//calcualte annual salary increase
			}

			interest_rate[current_row] = (float)(RPI + (((0.00015*salary[current_row]) - 3.75) / 100));
			if (interest_rate[current_row] >(RPI + 0.03)) { //ensure interest rate does not exceed threshold
				interest_rate[current_row] = (float)(RPI + 0.03);
			}
			if (interest_rate[current_row] <(RPI)) { //ensure interest rate is not threshold
				interest_rate[current_row] = (float)(RPI);
			}

			mon_interest[current_row] = (int)((remaining_bal[current_row-1]) * (interest_rate[current_row])) / 12;

			//check if current salary is greater then threshold, if so calcualte monthly payment
			if (salary[current_row] > SALARY_THRESHOLD) {
				salary_abv_threshold[current_row] = salary[current_row] - SALARY_THRESHOLD; //calculate salary above threshold
				mon_repayment[current_row] = (int)((salary_abv_threshold[current_row] * REPAYMENT_RATE) / 12);
			}
			else { //if salary is smaller than threshold make payment 0
				salary_abv_threshold[current_row] = 0;
				mon_repayment[current_row] = 0;
			}
			
			//check if current monthly payment exceeds the remaining balance
			if ((mon_repayment[current_row]) < (remaining_bal[current_row - 1])) {
				remaining_bal[current_row] = remaining_bal[current_row - 1] - mon_repayment[current_row] + mon_interest[current_row];
			}
			else {//if repayment is completed - exit loop
				remaining_bal[current_row] = 0;
				mon_repayment[current_row] = remaining_bal[current_row - 1];
				exitflag = 1;
			}

			//Export data to CSV file
			int *a[7] = { mon,salary,salary_abv_threshold,interest_rate,mon_interest,mon_repayment,remaining_bal }; //convert 1D array into a 2D array
			//WARNING C4133 - This is because that the interest rate is a float, however this value is not used in any of the data calculations
			char filename[100];
			sprintf(filename, "%d.csv", initial_salary); //generate file name based on salary

			int i, j;
			int n = 7, m = current_row + 1;

			FILE *csvexport; //initialise file
			csvexport = fopen(filename, "w+");
			fprintf(csvexport, "Date (mm/yy), Salary (£), Salary Above Threshold (£), Interest Rate (%%), Monthly Interest (£), Monthly Repayment (£), Balance (£)\n");

			for (i = 0; i < m; i++) {
				for (j = 0; j < n; j++) {

					if (j == 0) {
						fprintf(csvexport, "%d/%d ", a[j][i], year[i]); //merge month and year to one field
						fprintf(csvexport, ",");
							
					}
					else if (j == 3) {
						fprintf(csvexport, "%f ", (interest_rate[i]*100)); //for interest as float
						fprintf(csvexport, ",");
					}
					else {
						fprintf(csvexport, "%d ", a[j][i]); //for all other integer data
						fprintf(csvexport, ",");
						}
					
				}
				fprintf(csvexport, "\n");
			}
			fclose(csvexport);//close file to ensure all data is aved
			current_row = current_row + 1; //increase row by 1 to print the next row
		}

		//find cumulative interest
		int total_interest = 0;
		int k;
		for (k = 0; k < (current_row - 1); k++) {
			total_interest = total_interest + mon_interest[k];
		}

		//print data to screen
		printf("\n");
		printf("\nInitial Salary: %d", salary[0]);
		printf("\nFinal Salary: %d", salary[current_row-1]);
		printf("\nTotal Loan: %d",total_loan);
		printf("\nLast Payment Date: %d/%d",mon[current_row-1],year[current_row-1]);
		printf("\nClosing Balance: %d",remaining_bal[current_row-1]);
		printf("\nTotal Interest: %d",total_interest);
		printf("\nTotal Paid: %d", (total_interest + total_loan));
		printf("\n");

		//export data to text file
		FILE *exportdata = fopen("loancalc.txt", "a");
		fprintf(exportdata, "\n");
		fprintf(exportdata, "\nInitial Salary: %d", salary[0]);
		fprintf(exportdata, "\nFinal Salary: %d", salary[current_row - 1]);
		fprintf(exportdata, "\nTotal Loan: %d", total_loan);
		fprintf(exportdata, "\nLast Payment Date: %d/%d", mon[current_row - 1], year[current_row - 1]);
		fprintf(exportdata, "\nClosing Balance: %d", remaining_bal[current_row - 1]);
		fprintf(exportdata, "\nTotal Interest: %d", total_interest);
		fprintf(exportdata, "\nTotal Paid: %d", (total_interest + total_loan));
		fprintf(exportdata, "\n");
		fclose(exportdata);

	}
}
