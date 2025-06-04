#include <iostream>
#include "Exam.h"


const char *Exam::printTrimester()
{
	switch(trimester)
	{
	case 1: 
		return "Jan";
		break;

	case 5:
		return "May";
		break;

	case 10:
		return "Oct";
		break;

	default:
		return " ";
	}
}



bool Exam::calculateGPA()
{
	double sum = 0;
	int total_credit_hours = 0;

	if(numOfSubjects <= 0)
		return false;

	else
	{
		for(int i=0; i<numOfSubjects; i++)
		{
			sum = sum + sub[i].getGradePoint() * sub[i].credit_hours;
			total_credit_hours = total_credit_hours + sub[i].credit_hours;
		}

		gpa = sum/(double)total_credit_hours;
	
	return true;
	}
}



Exam::Exam()
{
	trimester = 0;
	year = 0;
	gpa = 0;
	numOfSubjects = 0;
	
	
}



void Exam::print(ostream& out)
{
	out << "\n\n" << printTrimester() << " " << year << " Exam Results: " << endl;

	out << "\n" << numOfSubjects << " subjects taken.";
	out << "\n__________________________________________________________________________________________________________________________________";
	out << "\nSubject Code\t" << setw(70) << left << "Subject Name"  // Widen to 70
		<< setw(12) << right << "Credit Hours"
		<< setw(13) << right << "Grade"
		<< setw(15) << right << "Grade Point";
	out << "\n__________________________________________________________________________________________________________________________________";

	for (int i = 0; i < numOfSubjects; i++)
		sub[i].print(out);

	out << "\nGPA: " << fixed << setprecision(5) << gpa;
	out << "\n\n";
}