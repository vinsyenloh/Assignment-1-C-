#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<limits>
#include<cctype> // for isdigit()
#include<cstring> // for strcpy, strcat
#include<algorithm>
#include"List.h"
#include"Student.h"

using namespace std;

#pragma warning(push)
#pragma warning(disable : 4996)

//Function Prototype
bool CreateStuList(const char *, List *);
bool duplicate(const List&, Student);
bool DeleteStudent(List *, char *);
bool PrintList(List, int); 
bool InsertExamResult(const char *, List *);
bool PrintStatistic(List *);
bool FilterStudent(List, List *, char *, int, int) ;
bool UpdateIDandPhone(List *);
bool FindPotentialFirstClass(List, List *, char *);
int menu();
void mainFunction();

int main() {
    
    mainFunction();
    return 0;
}

bool CreateStuList(const char* fileName, List* list) {

    int numRecordRead = 0, count = 0;
    ifstream readFile(fileName);
    Student student;

    if (!readFile) {
        cout << "\n\nNo " << fileName << " file found." << endl;
        return false;
    }
    else {

        while (!readFile.eof()) {
            string line;

            // ?? Student Id
            getline(readFile, line, '=');
            readFile >> student.id;
            readFile.ignore();

            // detect whether is empty line or not
            if (line.empty())
                continue;

            // ?? Student Name
            getline(readFile, line, '=');
            readFile.getline(student.name, sizeof(student.name));

            // Remove leading spaces
            int startIndex = 0;
            while (student.name[startIndex] != '\0' && isspace(student.name[startIndex])) {
                startIndex++;  // Ignore spaces
            }

            // Copy the part after removing spaces
            memmove(student.name, student.name + startIndex, strlen(student.name + startIndex) + 1);


            // ?? Course
            getline(readFile, line, '=');
            readFile >> student.course;
            readFile.ignore();

            // ?? Phone Number
            getline(readFile, line, '=');
            readFile >> student.phone_no;
            readFile.ignore();

            if (!duplicate(*list, student)) {

                list->insert(student); //no duplicate then store the info into linked list

                numRecordRead++;

            }
            else {
                count++; //duplicate count++
            }
        }
        cout << "\n\t\t    " << numRecordRead << " records have been successfully read.\n" << endl;
        cout << "\t\t    " << count << " duplicate records have been ingnored.\n" << endl;

        readFile.close();
        return true;
    }

    return true;
}

bool duplicate(const List& list, Student item) {
    Node* newNode = list.head;

    while (newNode != NULL) {
        //If duplicate node is found
        if (newNode->item.compareID(item))
            return true;

        newNode = newNode->next;
    }
    return false;
}


bool DeleteStudent(List* list, char* id) {
    Student student;
    if (list->empty()) {
        cout << "Student list is empty." << endl;
        return false;
    }

    for (int i = 1; i <= list->size(); i++) {
        list->get(i, student);
        if (strcmp(id, student.id) == 0) {
            list->remove(i);
            return true;
        }
    }

    return false;
}

bool PrintList(List list, int source) {
    ofstream writeFile;
    if (source == 2) {
        writeFile.open("student_result.txt"); 

        //check whether the file can be opened and written to
        if (!writeFile.is_open()) {
            cout << "Failed to create student_result.txt." << endl;
            return false;
        }
    }

    if (list.empty())
        return false;

    Node* cur = list.head;
    int studentCount = 1;

    while (cur != NULL) {
        ostream& out = (source == 1) ? cout : writeFile;

        out << "\n************************************************************ STUDENT "
            << studentCount++ << " ***********************************************************\n";

        cur->item.print(out);  // Print student info (name, ID, etc.)

        out << "\n-------------------------------------------------------- PAST EXAM RESULT: ------------------------------------------------------- \n";

        // Print all exam trimesters (if any exist)
        if (cur->item.exam_cnt > 0) {
            for (int i = 0; i < cur->item.exam_cnt; i++) {
                cur->item.exam[i].print(out);  // Print each exam
            }
        }

        out << "\n************************************************************ STUDENT "
            << (studentCount - 1) << " ***********************************************************\n\n";

        cur = cur->next;
    }

    if (source == 2) {
        cout << "\nThe student list has been printed in the file." << endl;
        writeFile.close();
    }
    return true;
}
bool InsertExamResult(const char* fileName, List* list) {
    ifstream readFile(fileName);
    if (!readFile) {
        cout << "\n\nNo " << fileName << " file found." << endl;
        return false;
    }

    char studentID[12];
    char subjectName[256];
    Student student;

    while (readFile >> studentID) {  // Better way to check for input
        // Find student in list
        int studentIndex = -1;
        for (int i = 1; i <= list->size(); i++) {
            list->get(i, student);
            if (strcmp(student.id, studentID) == 0) {
                studentIndex = i;
                break;
            }
        }

        if (studentIndex == -1) {
            cout << "Student with ID " << studentID << " not found in the list.\n";
            // Skip the rest of this student's record
            readFile.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Ensure we do not exceed max exam storage (10)
        if (student.exam_cnt >= 10) {
            cout << "Student " << studentID << " already has 10 exam records. Skipping.\n";
            // Skip the rest of this student's record
            readFile.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        int x = student.exam_cnt; // Use current exam count
        int totalCredit = 0; // Reset for each student

        if (!(readFile >> student.exam[x].trimester >> student.exam[x].year >> student.exam[x].numOfSubjects)) {
            cout << "Error reading exam header for student " << studentID << endl;
            continue;
        }
        for (int i = 0; i < student.exam[x].numOfSubjects; i++) {

            // Check array bounds to prevent overflow
            // - Maximum 6 subjects per exam
            if (i > sizeof(student.exam[x].sub) / sizeof(Subject)) {
                cout << "Number of subjects exceeds limit! Maximum "
                    << sizeof(student.exam[x].sub) / sizeof(Subject)
                    << " subjects allowed." << endl;
                break;
            }

            if (!(readFile >> student.exam[x].sub[i].subject_code)) {
                cout << "Error reading subject code for student " << studentID << endl;
                break;
            }

            if (!(readFile >> subjectName)) {
                cout << "Error reading subject name for student " << studentID << endl;
                break;
            }

            // Replace underscores with spaces
            for (int j = 0; subjectName[j] != '\0'; j++) {
                if (subjectName[j] == '_') subjectName[j] = ' ';
            }

            strncpy(student.exam[x].sub[i].subject_name, subjectName, sizeof(student.exam[x].sub[i].subject_name) - 1);
            student.exam[x].sub[i].subject_name[sizeof(student.exam[x].sub[i].subject_name) - 1] = '\0';

            if (!(readFile >> student.exam[x].sub[i].credit_hours >> student.exam[x].sub[i].marks)) {
                cout << "Error reading credit hours or marks for student " << studentID << endl;
                break;
            }

            totalCredit += student.exam[x].sub[i].credit_hours;
        }

        student.exam[x].calculateGPA();
        student.totalCreditsEarned += totalCredit;
        student.exam_cnt++; // Increment trimester exam count
        student.calculateCurrentCGPA(); // Update CGPA after all changes

        // Set the updated student back into the list
        if (!list->set(studentIndex, student)) {
            cout << "Failed to update student " << studentID << " in the list.\n";
        }
    }

    return true;
}

bool PrintStatistic(List *list) {
    struct CourseStats {
        const char* course;
        int count;
    };

    CourseStats courses[] = {
        {"CS", 0},
        {"IA", 0},
        {"IB", 0},
        {"CN", 0},
        {"CT", 0}
    };

    Node* cur = list->head;

    if (list->empty()) {
        cout << "Empty list" << endl;
        return false;
    }

    int totalStudents = 0;
    double sumCGPA = 0.0;
    double totalSubjects = 0.0;
    double totalCreditHours = 0.0;
    int totalSemesters = 0;

    // Calculate statistics
    while (cur != NULL) {
        Student student = cur->item;
        totalStudents++;
        sumCGPA += student.current_cgpa;

        for (int i = 0; i < student.exam_cnt; i++) {
            totalSubjects += student.exam[i].numOfSubjects;
            
            for (int j = 0; j < student.exam[i].numOfSubjects; j++) {
                totalCreditHours += student.exam[i].sub[j].credit_hours;
            }
            
            totalSemesters++;
        }

        for (auto& course : courses) {
            if (strcmp(student.course, course.course) == 0) {
                course.count++;
                break;
            }
        }

        cur = cur->next;
    }

    // Calculate averages
    double avgCGPA = (totalStudents > 0) ? sumCGPA / totalStudents : 0.0;
    double avgSubjectsPerSemester = (totalSemesters > 0) ? totalSubjects / totalSemesters : 0.0;
    double avgCreditHoursPerSemester = (totalSemesters > 0) ? totalCreditHours / totalSemesters : 0.0;

    // Display header information
    cout << "Total Students: " << totalStudents << endl;
    for (const auto& course : courses) {
        if (course.count > 0) {
            cout << "\t" << course.course << " Students - " << course.count << endl;
        }
    }
    cout << endl;

    // Use Student::print for the statistics output
    if (totalStudents > 0) {
        // Create a temporary student to call the print function
        Student tempStudent;
        tempStudent.print(cout, avgCGPA, avgSubjectsPerSemester, avgCreditHoursPerSemester);
    }

    return true;
}

bool FilterStudent(List list1, List* list2, char* course, int year, int totalcredit) {
    if (list1.empty()) {
        cout << "Source list is empty" << endl;
        return false;
    }

    

    if (!list2->empty()) {
        cout << "Destination list is not empty" << endl;
        return false;
    }

    Node* cur = list1.head;
    bool found = false;

    while (cur != NULL) {
        Student student = cur->item;

        // Extract year from student ID (first 2 digits represent year enrolled)
        int enrolledYear = 2000 + atoi(string(student.id, 2).c_str());

        // Check all three conditions
        if (strcmp(student.course, course) == 0 &&
            enrolledYear == year &&
            student.totalCreditsEarned >= totalcredit) {

            list2->insert(student);
            found = true;
        }

        cur = cur->next;
    }

    return found;
}

bool UpdateIDandPhone(List* list) {
    if (list->empty()) {
        cout << "Empty List" << endl;
        return false;
    }

    Node* cur = list->head;
    int index = 1;

    while (cur != nullptr) {
        Student student = cur->item;

        // Check if ID starts with 'B' - if so, skip updating this student
        if (student.id[0] == 'B') {
            cur = cur->next;
            index++;
            continue;
        }
        

        // Update ID based on course
        char newID[20] = "";
        strcat(newID, "B"); // Add prefix 'B'
        strcat(newID, student.course); // Add course code
        strcat(newID, student.id); // Add original ID
        strcpy(student.id, newID);

        // Update phone number
        char newPhone[13] = "";
        int firstDigit = student.phone_no[0] - '0'; // Convert char to int

        // Determine prefix based on first digit
        if (firstDigit % 2 == 1) { // Odd
            strcat(newPhone, "01");
        }
        else { // Even
            strcat(newPhone, "02");
        }

        // Copy digits (skip dashes)
        for (int i = 0; student.phone_no[i] != '\0'; i++) {
            if (isdigit(student.phone_no[i])) {
                char digit[2] = { student.phone_no[i], '\0' };
                strcat(newPhone, digit);
            }
        }

        strcpy(student.phone_no, newPhone);
        
        //Check whether there are duplicate student IDs in the list.
        if (!duplicate(*list, student)) {
            // Update the student in the list
            list->set(index, student);
        }
       
        cur = cur->next;
        index++;
    }

    return true;
}

bool FindPotentialFirstClass(List list1, List* list2, char* course) {
    if (list1.empty()) {
        cout << "Source list is empty" << endl;
        return false;
    }

    if (!list2->empty()) {
        cout << "Destination list is not empty" << endl;
        return false;
    }

    Node* cur = list1.head;
    bool found = false;

    while (cur != nullptr) {
        Student student = cur->item;
        // Skip if not the requested course or has taken less than 3 exams
        if (strcmp(student.course, course) != 0 || student.exam_cnt < 3) {
            cur = cur->next;
            continue;
        }

        int highGpaCount = 0;
        bool meetsCriteria = true;

        // Check each exam
        for (int i = 0; i < student.exam_cnt; i++) {
            // Calculate total credits for this trimester
            int trimesterCredits = 0;
            for (int j = 0; j < student.exam[i].numOfSubjects; j++) {
                trimesterCredits += student.exam[i].sub[j].credit_hours;
            }
          
            if (student.exam[i].gpa >= 3.75 && trimesterCredits >= 12) {
                highGpaCount++;
            }
            else if (student.exam[i].gpa < 3.5) {
                meetsCriteria = false;
                break;
            }
        }

        // If meets all criteria
        if (meetsCriteria && highGpaCount >= 3) {
            list2->insert(student);
            found = true;
        }

        cur = cur->next;
    }

    if (!found) {
        cout << "\nThere is no student in " << course
            << " that has potential to get first class" << endl << endl;
    }

    return found;
}

int menu() {
    int choice = 0;

    while (true) { // Loop until we get valid input
        system("cls");

        // Display the menu options
        cout << "--------------------------------------------------------------------------" << endl;
        cout << "                                   MENU: " << endl;
        cout << "--------------------------------------------------------------------------" << endl;
        cout << "\t\t\t1. Create student list" << endl;
        cout << "\t\t\t2. Delete Student" << endl;
        cout << "\t\t\t3. Print student list" << endl;
        cout << "\t\t\t4. Insert exam result" << endl;
        cout << "\t\t\t5. Print Exam Statistic" << endl;
        cout << "\t\t\t6. Filter Student" << endl;
        cout << "\t\t\t7. Update Student's ID and Phone" << endl;
        cout << "\t\t\t8. Find Potential First Class Student" << endl;
        cout << "\t\t\t9. Exit" << endl;
        cout << "--------------------------------------------------------------------------" << endl;
        cout << "\nEnter your Choice (1-9): ";

        // Check if input is an integer
        if (cin >> choice) {
            // Check if within valid range
            if (choice >= 1 && choice <= 9) {
                system("cls");
                return choice;
            }
            else {
                cout << "\nInvalid choice! Please enter a number between 1 and 9." << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        else {
            cout << "\nInvalid input! Please enter a number." << endl;
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
        }

        // Pause before showing menu again
        cout << "\n\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void mainFunction() {
    List studentList;
    List filteredStudents; // Renamed from list2 for clarity
    const char* studentFile = "student.txt";
    const char* examFile = "exam.txt";
    int option = 0;

    do {
        option = menu();

        switch (option) {
        case 1: { // Create student list
            cout << "--------------------------------------------------------------------------" << endl;
            cout << "\n========================== Create Student List ==========================\n" << endl;
            cout << "--------------------------------------------------------------------------" << endl;
            if (CreateStuList(studentFile, &studentList)) {
                cout << "\nStudent list created successfully!\n\n";
            }
            else {
                cout << "\nFailed to create student list.\n\n";
            }
            system("pause");
            break;
        }

        case 2: { // Delete student
            cout << "--------------------------------------------------------------------------" << endl;
            cout << "\n============================= Delete Student =============================\n" << endl;
            cout << "--------------------------------------------------------------------------" << endl;
            char id[10];
            cout << "\nEnter student ID to delete: ";
            cin >> id;

            if (DeleteStudent(&studentList, id)) {
                cout << "\nStudent ID: " << id << " deleted successfully.\n" << endl;
            }
            else {
                cout << "\nFailed to delete student ID: " << id << "\n" << endl;
            }
            system("pause");
            break;
        }

        case 3: { // Print student list
            cout << "----------------------------------------------------------------------------------------------------------------------------------" << endl;
            cout << "\n====================================================== Display Student List ======================================================\n" << endl;
            cout << "----------------------------------------------------------------------------------------------------------------------------------" << endl;
            int source = 0;
            while (true) {
                cout << "\nDisplay to screen (1) or file (2): ";
                if (cin >> source && (source == 1 || source == 2)) break;
                cout << "\nInvalid input! Please enter 1 or 2.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (!PrintList(studentList, source)) {
                cout << "\nSorry, no student list available." << endl;
            }
            cout << "\n";
            system("pause");
            break;
        }

        case 4: { // Insert exam results
            cout << "--------------------------------------------------------------------------" << endl;
            cout << "\n=========================== Insert Exam Results ==========================\n" << endl;
            cout << "--------------------------------------------------------------------------" << endl;
            if (InsertExamResult(examFile, &studentList)) {
                cout << "\nExam results inserted successfully!\n" << endl;
            }
            else {
                cout << "\nFailed to insert exam results.\n" << endl;
            }
            system("pause");
            break;
        }

        case 5: { // Print statistics
            cout << "--------------------------------------------------------------------------" << endl;
            cout << "\n============================ Student Statistics ==========================\n" << endl;
            cout << "--------------------------------------------------------------------------" << endl;
            PrintStatistic(&studentList);
            system("pause");
            break;
        }

        case 6: { // Filter students
            cout << "----------------------------------------------------------------------------------------------------------------------------------" << endl;
            cout << "\n========================================================== Filter Students =======================================================\n" << endl;
            cout << "----------------------------------------------------------------------------------------------------------------------------------" << endl;
            
            string course;
            int year, minCredits;

            cout << "\nEnter course (CS/IA/IB/CN/CT): ";
            cin >> course;
            cout << "\nEnter year: ";
            cin >> year;
            cout << "\nEnter minimum credits: ";
            cin >> minCredits;

            //convert course to uppercase
            char courseArray[3];
            transform(course.begin(),course.end(),course.begin(), ::toupper);
            strcpy(courseArray, course.c_str());

            // Clear previous results
            while (!filteredStudents.empty()) {
                filteredStudents.remove(1);
            }

            if (FilterStudent(studentList, &filteredStudents, courseArray, year, minCredits)) {
                cout << "\n=== Filter Results ===\n";
                PrintList(filteredStudents, 1);
            }
            else {
                cout << "\nNo matching students found.\n";
            }
            system("pause");
            break;
        }

        case 7: { // Update ID and phone
            cout << "----------------------------------------------------------------------------------------------------------------------------------" << endl;
            cout << "\n===================================================== Update Student IDs/Phones ==================================================\n" << endl;
            cout << "----------------------------------------------------------------------------------------------------------------------------------" << endl;
            if (UpdateIDandPhone(&studentList)) {
                cout << "\nUpdates completed successfully!\n";
                PrintList(studentList, 1);
            }
            else {
                cout << "\nFailed to complete updates.\n";
            }
            system("pause");
            break;
        }

        case 8: { // Find first class students
            cout << "----------------------------------------------------------------------------------------------------------------------------------" << endl;
            cout << "\n===================================================== Find Potential First Class ==================================================\n" << endl;
            cout << "----------------------------------------------------------------------------------------------------------------------------------" << endl;
            string course;
            cout << "\nEnter course (CS/IA/IB/CN/CT): ";
            cin >> course;

            //convert course to uppercase
            char courseArray[3];
            transform(course.begin(), course.end(), course.begin(), ::toupper);
            strcpy(courseArray, course.c_str());

            // Clear previous results
            while (!filteredStudents.empty()) {
                filteredStudents.remove(1);
            }

            if (FindPotentialFirstClass(studentList, &filteredStudents, courseArray)) {
                cout << "\n=== First Class Candidates ===\n";
                PrintList(filteredStudents, 1);
            }
            system("pause");
            break;
        }

        case 9: // Exit
            cout << "\nExiting program...\n";
            break;

        default:
            cout << "\nInvalid option! Please try again.\n";
            system("pause");
        }
    } while (option != 9);
}