#include <iostream>
#include "Subject.h"

#pragma warning(push)
#pragma warning(disable : 4996)


Subject::Subject()
{
	strcpy(subject_code, " ");
	strcpy(subject_name, " ");
	credit_hours = 0;
	marks = 0;
}


const char *Subject::getGrade()
{

	if(marks >= 0 && marks <= 49)
		return "F";
	else if(marks >= 50 && marks <= 54)
		return "C";
	else if(marks >= 55 && marks <= 59)
		return "C+";
	else if(marks >= 60 && marks <= 64)
		return "B-";
	else if(marks >= 65 && marks <= 69)
		return "B";
	else if(marks >= 70 && marks <= 74)
		return "B+";
	else if(marks >= 75 && marks <= 79)
		return "A-";
	else if(marks >= 80 && marks <= 89)
		return "A";
	else if(marks >= 90 && marks <= 100)
		return "A+";
	else
		return "N/A";
}


double Subject::getGradePoint()
{
	if(marks >= 0 && marks <= 49)
		return 0.00;
	else if(marks >= 50 && marks <= 54)
		return 2.00;
	else if(marks >= 55 && marks <= 59)
		return 2.33;
	else if(marks >= 60 && marks <= 64)
		return 2.67;
	else if(marks >= 65 && marks <= 69)
		return 3.00;
	else if(marks >= 70 && marks <= 74)
		return 3.33;
	else if(marks >= 75 && marks <= 79)
		return 3.67;
	else if(marks >= 80 && marks <= 100)
		return 4.00;
	else 
		return -1.0;
}



void Subject::print(ostream& out)
{
	out << "\n"
		<< left << setw(15) << subject_code << "\t"       // Subject code, left-aligned, width 15
		<< left << setw(65) << subject_name               // Subject name, left justified, width 65 (widened)
		<< right << setw(12) << credit_hours              // Credits hours, right-aligned, width 10
		<< right << setw(16) << getGrade()                // Grade, right-aligned, width 10
		<< right << setw(15) << fixed << setprecision(5) << getGradePoint(); // Grade point, right-aligned, 5 decimal places
}
