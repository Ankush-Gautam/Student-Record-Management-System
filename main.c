/************************************************************************************
 *	STUDENT RECORD MANAGEMENT SYSTEM												*
 *	It can add, edit, store, delete, sort details of students.						*
 *	Developed by: Ankush Gautam														*
 ************************************************************************************/

// header files
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

// structure for another exam
struct newSetup
{
	char programme[33];
	int semester;
	char examName[33];
	int subs;
	char subject[10][25];
} newExam;

// structure for setup and saving password and theme details
struct setups
{
	char schoolname[33];
	char address[33];
	char programme[33];
	int semester;
	char examName[33];
	int subs;
	char subject[10][25];
	struct settings
	{
		char username[11];
		char password[11];
		char defaultcolor[11];
	} setting;
} setup;

// structure to define student
struct student
{
	// personal
	char name[25];
	char address[33];
	char phone[11];
	// academic
	int roll;
	float marks[10];
	float percent;
	float total;
	float gpa;
	char grade[3];
} std;

// functions declaration
void firstsetup();
void menu();
void add_record();
void view_record();
void edit_record();
void search_record();
void delete_record();
void sort_record();
int no_of_records();
void gradesheet();
void misc(); // settings
void sort_file();
void login();
void about();
void addAnotherSetup();

// color codes made functions
void black();
void red();
void blue();
void cyan();
void yellow();
void purple();
void green();
void reset();

// helping funcs
void gettheme();
void input_details();
void changetheme();
void changeUsername();
void header(char *);
char *grade_calc(float);
int nameChecker(char *);
int rollChecker(int);

/************************************ Main Function ************************************************/
void main()
{
	system("title Student Record Manager"); // names title bar of the WINDOW "Student Record Manager"

	// always run this first for default theme
	system("color f8");
	gettheme(); // getting previous color which user selected during previous run

	// if this file exists than no need to login cause who likes to login again and again and again
	FILE *fsetup = fopen("setup.dat", "rb");

	if (fsetup == NULL)
	{
		firstsetup();
	}

	fread(&setup, sizeof(struct setups), 1, fsetup);
	fclose(fsetup);

	if (setup.subs <= 0)
	{
		firstsetup();
	}

	login();
}

/*	menu to choose either to record, display , delete , edit etc.
 *	and also runs all funcs, so calling it in the main() is enough
 */
void menu()
{
	int choice;
	do
	{
		// header of the App
		header("STUDENT RECORD MANAGER");

		printf("\n\n\n\tWELCOME TO THE MAIN MENU\n\t------------------------\n");

		printf("\n\t1.ADD RECORD");
		printf("\n\t2.VIEW RECORD");
		printf("\n\t3.EDIT RECORD");
		printf("\n\t4.SEARCH RECORD");
		printf("\n\t5.DELETE RECORD");
		printf("\n\t6.SORT RECORDS");
		printf("\n\t7.NO OF RECORDS");
		printf("\n\t8.GRADE-SHEET");
		printf("\n\t9.SETTINGS");
		printf("\n\t0.EXIT\n");

		fflush(stdin);

		green();
		fflush(stdin);
		printf("\n\n\tEnter your choice:\n\t>> ");
		scanf("%d", &choice);
		reset();

		switch (choice)
		{
		case 1:
			add_record();
			break;
		case 2:
			view_record();
			break;
		case 3:
			edit_record();
			break;
		case 4:
			search_record();
			break;
		case 5:
			delete_record();
			break;
		case 6:
			sort_record();
			break;
		case 7:
			// since its short, so not declaring new function
			header("NO OF RECORDS");
			printf("\n\n\tNO OF RECORDS = %d\n\n\t", no_of_records());
			purple();
			system("pause");
			break;
		case 8:
			gradesheet();
			break;
		case 9:
			// settings previously named miscellaneous
			misc();
			break;
		case 0:
			red();
			printf("\n\tExiting...");
			reset();
			exit(0);
		}

	} while (choice != 0);
} // end of menu()

/*	if this is the first time the program is running
 *	I am trying to implement this feature cause not every class or school has same subjects
 *	so i'm trying to make this program as dynamic as possible as a first project i'm trying to as much as i can.
 */
void firstsetup()
{
	header("SETUP");

	FILE *fsetup = fopen("setup.dat", "wb");
	char verifypw[11];

	blue();
	printf("\n\tPlease Take a Moment to Setup.\n");
	printf("\t-------------------------------\n\n");
	reset();

	fflush(stdin);
	printf("\tEnter School Name: ");
	scanf(" %32[^\n]", setup.schoolname);
	fflush(stdin);
	printf("\tEnter Address: ");
	scanf(" %32[^\n]", setup.address);
	fflush(stdin);
	printf("\tEnter Programme (For e.g: BCA, BBA): ");
	scanf(" %32[^\n]", setup.programme);
	printf("\tEnter semester (For e.g: 1,2,3): ");
	scanf("%d", &setup.semester);
	fflush(stdin);
subs_again:
	printf("\tEnter No. Of Subjects: ");
	scanf("%d", &setup.subs);
	if (setup.subs > 10 || setup.subs <= 0)
	{
		red();
		printf("\t(Sorry! 0 < Subject <= 10)\n");
		reset();
		goto subs_again;
	}
	printf("\n");
	for (int i = 0; i < setup.subs; i++)
	{
		fflush(stdin);
		printf("\tEnter Name of subject %d: ", i + 1);
		scanf(" %24[^\n]", setup.subject[i]);
	}

	blue();
	printf("\n\tSIGN UP\n\t---------\n");
	reset();
	printf("\tEnter Username: ");
	scanf(" %10[^\n]", setup.setting.username);
new_pw_again:
	printf("\tEnter Password: ");
	scanf(" %10[^\n]", setup.setting.password);
	printf("\tConfirm Password: ");
	scanf(" %10[^\n]", verifypw);

	// checking if both password matches
	if (strcmp(verifypw, setup.setting.password) != 0)
	{
		red();
		printf("\t(Password doesn't match! Re-enter password.)\n");
		reset();
		goto new_pw_again;
	}

	green();
	printf("\n\tSetup Successful! Remember Your Details.\n\t");

	purple();
	system("pause");
	fwrite(&setup, sizeof(struct setups), 1, fsetup);
	fclose(fsetup);

	menu();
} // end of firstsetup()

// to add record
void add_record()
{
	header("ADD RECORD"); // menu title

	FILE *fadd = fopen("record.dat", "ab"); // opening file to add record
	int n = no_of_records();
	char yesno;

	// adding records using inputdetails() which asks info
	green();
	printf("\n\tRecord No. %d\n\t------------", n + 1);
	reset();
	input_details();
	fwrite(&std, sizeof(std), 1, fadd);
	fclose(fadd);
	green();
	printf("\n\tRecord successfully stored!\n\t---------------------------\n");
	reset();
	// if user wants to add more record

	printf("\tDo you want to add more?(Press 'y' for Yes or 'n' for No)\n\t>> ");
	scanf(" %c", &yesno);

	if (yesno == 'y' || yesno == 'Y')
		add_record();
	else
		menu();

} // end of add record function

// to display the records
void view_record()
{
	header("VIEW RECORD");

	int record_count = no_of_records();
	FILE *fp = fopen("record.dat", "rb");
	// checking if the file has atleast one record,otherwise to print no record found
	if (fp == NULL || record_count == 0)
	{
		red();
		printf("\n\tNo Record Found! Please ADD RECORD first.\n\n\t");
		purple();
		system("pause");
		menu();
	}

	// reading the file and displaying the content
	printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
	printf("\t-------------------------------------------------------------------------------------------\n");
	while (fread(&std, sizeof(std), 1, fp))
	{
		printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", std.roll, std.name, std.total, std.percent, std.gpa, std.grade);
	}

	fclose(fp);
	printf("\n\t");
	purple();
	system("pause");
} // end of view records

// to edit/update records, first copying the records in a temp file and then recopying the temp file data into og file
void edit_record()
{
	header("EDIT RECORD");

	int rollno;
	int found = 0, record_count = no_of_records();

	// creating a new temp file to store new updated data
	FILE *tempfile = fopen("tempfile.dat", "wb");
	FILE *fp = fopen("record.dat", "rb");
	if (fp == NULL || record_count == 0)
	{
		red();
		printf("\n\tFile doesn't exist! Please ADD RECORD first\n\n\t");
		purple();
		system("pause");
		menu();
	}

	// displaying the records, so that user can select which record to edit
	printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
	printf("\t-------------------------------------------------------------------------------------------\n");
	while (fread(&std, sizeof(std), 1, fp))
	{
		printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", std.roll, std.name, std.total, std.percent, std.gpa, std.grade);
	}
	rewind(fp); // to move the cursor at the start to read the file again

	fflush(stdin);
	green();
	printf("\n\n\tEnter the ROLLNUMBER of the Student you want to EDIT: ");
	scanf("%d", &rollno);
	reset();

	while (fread(&std, sizeof(std), 1, fp))
	{
		if (rollno == std.roll)
		{
			header("EDIT RECORD");

			// displaying the match record first to make user clear
			printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
			printf("\t-------------------------------------------------------------------------------------------\n");
			printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", std.roll, std.name, std.total, std.percent, std.gpa, std.grade);

			// display the marks too for more detail view
			printf("\n\n\t%-25sMARKS\n", "SUBJECT");
			printf("\t------------------------------\n");
			for (int j = 0; j < setup.subs; j++)
				printf("\t%-25s%.2f\n", setup.subject[j], std.marks[j]);

			green();
			printf("\n\tEnter New Data\n\t--------------");
			reset();

			// input details	, i didn't use the existing inputdetails() coz it gives trouble during name & rollno Verifying time.
			std.total = 0;
			std.percent = 0;

			printf("\n\n\tPersonal Details\n\t----------------\n");

		name_again:
			fflush(stdin);
			printf("\tEnter FullName: ");
			scanf(" %[^\n]s", std.name);
			// if name includes characters other than alphabets
			if (nameChecker(std.name) == 1)
			{
				red();
				printf("\t(Invalid Name! Please use Alphabets Only.)\n");
				reset();
				goto name_again;
			}
			fflush(stdin);
			printf("\tEnter Address: ");
			scanf(" %[^\n]s", std.address);
			fflush(stdin);
			printf("\tEnter Phone Number: ");
			scanf(" %[^\n]s", std.phone);

			printf("\n\tAcademic Details\n\t----------------");
		rollno_again:
			fflush(stdin);
			printf("\n\tEnter RollNo: ");
			scanf("%d", &std.roll);
			// checking if rollno is a natural number
			if (std.roll <= 0)
			{
				red();
				printf("\t(Invalid Rollnumber!)");
				reset();
				goto rollno_again;
			}
			// checking if rollno already exists in the file
			if ((rollChecker(std.roll) == 1) && (std.roll != rollno))
			{
				// 2nd condition coz user shud be able to enter the previous rollno of that student
				red();
				printf("\t(This Rollnumber already exists!)");
				reset();
				goto rollno_again;
			}

			for (int j = 0; j < setup.subs; j++)
			{
			again_marks:
				fflush(stdin);
				printf("\tEnter marks of %s: ", setup.subject[j]);
				scanf("%f", &std.marks[j]);
				// checking for invalid input
				if (std.marks[j] < 0 || std.marks[j] > 100)
				{
					red();
					printf("\t(Marks cannot be less than 0 or greater than 100!)\n");
					reset();
					goto again_marks;
				}
				std.total += std.marks[j];
			}
			std.percent = std.total / setup.subs;
			std.gpa = std.percent / 25;
			strcpy(std.grade, grade_calc(std.percent));

			fwrite(&std, sizeof(struct student), 1, tempfile);
			found = 1;
		} // end of if(rollno == std.roll)
		else
			fwrite(&std, sizeof(struct student), 1, tempfile);
	}
	fclose(fp);
	fclose(tempfile);

	//	remove("record.dat");
	//	rename("tempfile.dat","record.dat");
	// copying all the data from the tempfile to the main file coz rename&remove not working
	if (found)
	{
		fp = fopen("record.dat", "wb");
		tempfile = fopen("tempfile.dat", "rb");

		while (fread(&std, sizeof(std), 1, tempfile))
		{
			fwrite(&std, sizeof(std), 1, fp);
		}
		fclose(fp);
		fclose(tempfile);
		green();
		printf("\n\tRecord Updated Successfully!\n\t-----------------------------");
		reset();
	}
	else
	{
		red();
		printf("\n\tNo Match Found!\n\t----------------");
		reset();
	}

	printf("\n\t");
	purple();
	system("pause");
} // end of edit record

// to search and display the individual search record
void search_record()
{
	header("SEARCH RECORD");

	char name[24];
	int found = 0;
	int n = no_of_records();

	// opening the file to read data and search
	FILE *fp = fopen("record.dat", "rb");
	if (fp == NULL || n == 0)
	{
		red();
		printf("\n\tFile doesn't exist! Please ADD RECORD first.\n\n\t");
		purple();
		system("pause");
		menu();
	}

	// asking for name to search and compare in the file
	fflush(stdin);
	green();
	printf("\n\n\tEnter the NAME of the Student: ");
	scanf(" %[^\n]s", name);
	reset();

	// reading the file and display the result with is equal to the entered array
	while (fread(&std, sizeof(struct student), 1, fp))
	{
		if (strcmp(name, std.name) == 0)
		{
			found = 1;

			green();
			printf("\n\n\tPersonal Details\n\t----------------\n");
			reset();
			printf("\t%-10s %s\n\t%-10s %s\n\t%-10s %s\n", "NAME", std.name, "ADDRESS", std.address, "PHONE", std.phone);

			green();
			printf("\n\n\tAcademic Details\n\t----------------\n");
			reset();
			printf("\t%-10s %.2f\n\t%-10s %.2f\n\t%-10s %s\n", "TOTAL", std.total, "GPA", std.gpa, "GRADE", std.grade);

			// display the marks too in the search for more detail view
			green();
			printf("\n\n\t%-25sMARKS\n", "SUBJECT");
			printf("\t-------------------------------\n");
			reset();
			for (int j = 0; j < setup.subs; j++)
			{
				printf("\t%-25s%6.2f\n", setup.subject[j], std.marks[j]);
			}
		}
	}
	if (!found)
	{
		red();
		printf("\n\tNo Match Found!\n\t----------------");
		reset();
	}
	printf("\n\t");
	purple();
	system("pause");
} // end of search record()

// to delete the individual record
void delete_record()
{
	header("DELETE RECORD");

	int rollno;
	char yesno;
	int found = 0, record_count = no_of_records();

	// creating a new temp file to store new updated data
	FILE *tempfile = fopen("tempfile.dat", "wb");
	FILE *fp = fopen("record.dat", "rb");
	if (fp == NULL || record_count == 0)
	{
		red();
		printf("\n\tFile doesn't exist! Please ADD RECORD first.\n\n\t");
		purple();
		system("pause");
		menu();
	}

	// displaying the records, so that user can select which record to delete
	printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
	printf("\t--------------------------------------------------------------------------------------------\n");
	while (fread(&std, sizeof(struct student), 1, fp))
	{
		printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", std.roll, std.name, std.total, std.percent, std.gpa, std.grade);
	}
	rewind(fp); // to move the cursor at the start to read the file again

	fflush(stdin);
	green();
	printf("\n\n\tEnter the ROLLNUMBER of the Student you want to DELETE: ");
	scanf("%d", &rollno);
	reset();

	while (fread(&std, sizeof(struct student), 1, fp))
	{
		if (rollno == std.roll)
			found = 1;
		else
			fwrite(&std, sizeof(struct student), 1, tempfile);
	}
	fclose(fp);
	fclose(tempfile);

	//	remove("record.dat");
	//	rename("tempfile.dat","record.dat");
	// copying all the data from the tempfile to the main file coz rename&remove not working
	if (found)
	{
		fp = fopen("record.dat", "wb");
		tempfile = fopen("tempfile.dat", "rb");

		while (fread(&std, sizeof(struct student), 1, tempfile))
		{
			fwrite(&std, sizeof(struct student), 1, fp);
		}
		fclose(fp);
		fclose(tempfile);
		green();
		printf("\n\tRecord DELETED Successfully!");
		printf("\n\t-----------------------------\n\n");
		reset();
	}
	else
	{
		red();
		printf("\n\tNo Match Found!\n\t----------------\n");
		reset();
	}
	// if user wants to delete more record
	printf("\tDo you want to DELETE Again?(Press 'y' for Yes or 'n' for No)\n\t>> ");
	scanf(" %c", &yesno);

	if (yesno == 'y' || yesno == 'Y')
		delete_record();
	else
		menu();
} // end of delete record()

// to sort records according to name or rollno or grade
void sort_record()
{
	int choice, subchoice;

	do
	{
		header("SORT RECORDS");

		struct student *s, temp;
		int n = no_of_records(); // getting no of records

		// opening file to read the data
		FILE *fp = fopen("record.dat", "rb");
		if (fp == NULL || n == 0)
		{
			red();
			printf("\n\tFile doesn't exist! Please ADD RECORD first.\n\n\t");
			purple();
			system("pause");
			menu();
		}

		// allocating memory for struct s and storing file records in s[] as array
		s = (struct student *)calloc(n, sizeof(struct student));
		for (int i = 0; i < n; i++)
		{
			fread(&s[i], sizeof(struct student), 1, fp);
		}

		// sort menu to choose either to arrange acc. to rollno, name or gpa
		printf("\n\t1.Sort by Name\n\t2.Sort by Rollno\n\t3.Sort by GPA\n\t4.Sort by SerialNo.\n\t5.SPREADSHEET\n\t0.Back to Menu\n");
		green();
		fflush(stdin);
		printf("\n\tEnter a choice:\n\t>> ");
		scanf("%d", &choice);
		reset();

		switch (choice)
		{
		case 1:
			header("SORTING BY NAME");
			// sorting according to alphabetical order
			for (int i = 0; i < n; i++)
				for (int j = i + 1; j < n; j++)
				{
					if (strcmp(s[i].name, s[j].name) > 0)
					{
						temp = s[i];
						s[i] = s[j];
						s[j] = temp;
					}
				}  // j forloop
			break; // close
		case 2:
			header("SORTING BY ROLLNO");
			// sorting according to rollno of students
			for (int i = 0; i < n; i++)
				for (int j = i + 1; j < n; j++)
				{
					if (s[i].roll > s[j].roll)
					{
						temp = s[i];
						s[i] = s[j];
						s[j] = temp;
					}
				}  // j forloop
			break; // close
		case 3:
			header("SORTING BY GPA");
			// sorting according to gpa
			for (int i = 0; i < n; i++)
				for (int j = i + 1; j < n; j++)
				{
					if (s[i].gpa < s[j].gpa)
					{
						temp = s[i];
						s[i] = s[j];
						s[j] = temp;
					}
				} // j forloop

			// Displaying the sorted result
			printf("\n\n\tRANK\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
			printf("\t-------------------------------------------------------------------------------------------------\n");
			for (int i = 0; i < n; i++)
			{
				printf("\t %02d\t %02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", i + 1, s[i].roll, s[i].name, s[i].total, s[i].percent, s[i].gpa, s[i].grade);
			}

			printf("\n\t");
			purple();
			system("pause");
			menu();
			break;
		case 4:
			header("SORTING BY SERIALNO.");
			// sorting according to alphabetical order and giving serial number
			for (int i = 0; i < n; i++)
				for (int j = i + 1; j < n; j++)
				{
					if (strcmp(s[i].name, s[j].name) > 0)
					{
						temp = s[i];
						s[i] = s[j];
						s[j] = temp;
					}
				} // j forloop

			// Displaying the sorted result
			printf("\n\n\tSN.\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
			printf("\t---------------------------------------------------------------------------------------------\n");
			for (int i = 0; i < n; i++)
			{
				printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", i + 1, s[i].name, s[i].total, s[i].percent, s[i].gpa, s[i].grade);
			}

			printf("\n\t");
			purple();
			system("pause");
			menu();
			break;
		case 5:
			header("SPREADSHEET");
			// sorting according to gpa
			for (int i = 0; i < n; i++)
				for (int j = i + 1; j < n; j++)
				{
					if (s[i].gpa < s[j].gpa)
					{
						temp = s[i];
						s[i] = s[j];
						s[j] = temp;
					}
				} // j forloop
			// displaying the sorted record

			printf("\n\n\tRANK\tROLL\t%-28s", "NAME");
			for (int i = 0; i < setup.subs; i++)
			{
				printf("\t%.4s.", setup.subject[i]);
			}
			printf("\t%-9s %-11s \tGPA\tGRADE\n\t", "TOTAL", "PERCENT");

			for (int i = 0; i < 140; i++) // for line design
				printf("-");
			printf("\n");
			// displaying all the data including individual marks in each subjects
			for (int i = 0; i < n; i++)
			{
				printf("\t %02d\t %02d\t%-28s%", i + 1, s[i].roll, s[i].name);
				for (int j = 0; j < setup.subs; j++)
				{
					printf("\t%05.1f", s[i].marks[j]);
				}
				printf("\t%-09.2f %6.2f %%\t%.2f\t  %s\n", s[i].total, s[i].percent, s[i].gpa, s[i].grade);
			}
			printf("\n\t");
			purple();
			system("pause");
			menu();
		case 0:
			menu();
			break;
		}
		if (choice >= 1 && choice <= 5)
		{
			// there was a bug which displays the details , so applying this condition(well technically not a bug)
			// displaying the sorted record
			printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
			printf("\t---------------------------------------------------------------------------------------------\n");
			for (int i = 0; i < n; i++)
			{
				printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", s[i].roll, s[i].name, s[i].total, s[i].percent, s[i].gpa, s[i].grade);
			}
			free(s);
			printf("\n\n\t");
			purple();
			system("pause");
		}

	} while (choice != 0);
} // end of sort_records()
void addAnotherRecord()
{

	header("ADD ANOTHER EXAMINATION RECORD");

	char folderName[50] = "md ";
	char fileName[50] = "";
	char appName[50] = "";

	printf("\n\n\tEnter Programme (For e.g: BCA, BBA): ");
	scanf(" %32[^\n]", newExam.programme);
	printf("\tEnter semester (For e.g: 1,2,3): ");
	scanf("%d", &newExam.semester);
	printf("\tEnter Exam Name(without Space): ");
	scanf(" %32[^\n]", newExam.examName);
	fflush(stdin);
new_subs_again:
	printf("\tEnter No. Of Subjects: ");
	scanf("%d", &newExam.subs);
	if (newExam.subs > 10 || newExam.subs <= 0)
	{
		red();
		printf("\t(Sorry! 0 < Subject <= 10)\n");
		reset();
		goto new_subs_again;
	}
	printf("\n");
	for (int i = 0; i < newExam.subs; i++)
	{
		fflush(stdin);
		printf("\tEnter Name of subject %d: ", i + 1);
		scanf(" %24[^\n]", newExam.subject[i]);
	}
	// making a file in new folder
	strcat(folderName, newExam.examName);
	// making folder
	system(folderName);

	strcat(fileName, newExam.examName);
	strcat(fileName, "\\setup.dat");

	FILE *fp = fopen(fileName, "wb");
	fwrite(&newExam, sizeof(struct newSetup), 1, fp);
	fclose(fp);

	green();
	printf("\n\tYou will be directed to the new setup.\n");
	purple();

	printf("%s", appName);
	printf("\n\t");
	system("pause");
	reset();
}
// to make user input details, just declaring a seperate func , maybe used multiple times so
void input_details()
{
	std.total = 0;
	std.percent = 0;

	printf("\n\n\tPersonal Details\n\t----------------\n");
name_again:
	fflush(stdin);
	printf("\tEnter FullName: ");
	scanf(" %[^\n]s", std.name);
	// if name includes characters other than alphabets
	if (nameChecker(std.name) == 1)
	{
		red();
		printf("\t(Invalid Name! Please use Alphabets Only.)\n");
		reset();
		goto name_again;
	}
	else if (nameChecker(std.name) == 2)
	{
		red();
		printf("\t(This Name already exists!)\n");
		reset();
		goto name_again;
	}
	fflush(stdin);
	printf("\tEnter Address: ");
	scanf(" %[^\n]s", std.address);
phone_again:
	printf("\tEnter Phone Number: ");
	scanf(" %[^\n]s", std.phone);
	if (strlen(std.phone) != 10)
	{
		red();
		printf("\t(Phone Number should be 10 Digits!)\n");
		reset();
		goto phone_again;
	}
	printf("\n\tAcademic Details\n\t----------------\n");
rollno_again:
	fflush(stdin);
	printf("\tEnter RollNo: ");
	scanf("%d", &std.roll);
	// checking if rollno is a natural number
	if (std.roll <= 0)
	{
		red();
		printf("\t(Invalid Rollnumber!)\n");
		reset();
		goto rollno_again;
	}
	// checking if rollno already exists in the file
	if ((rollChecker(std.roll)) == 1)
	{
		red();
		printf("\t(This Rollnumber already exists!)\n");
		reset();
		goto rollno_again;
	}

	for (int j = 0; j < setup.subs; j++)
	{
	again_marks:
		fflush(stdin);
		printf("\tEnter marks of %s: ", setup.subject[j]);
		scanf("%f", &std.marks[j]);
		// checking for invalid input
		if (std.marks[j] < 0 || std.marks[j] > 100)
		{
			red();
			printf("\t(Marks cannot be lesser than 0 or greater than 100. Please enter again CAREFULLY!)\n");
			reset();
			goto again_marks;
		}
		std.total += std.marks[j];
	}

	// calculating results
	std.percent = std.total / setup.subs;
	std.gpa = std.percent / 25;
	strcpy(std.grade, grade_calc(std.percent));
} // end of input_details()

// about me and the program
void about()
{
	header("ABOUT");

	char ch;
	// Opening the about file cause everything is written in that file
	FILE *aboutfp = fopen("about.dat", "rb");
	if (aboutfp == NULL)
	{
		red();
		printf("\n\tSorry! The file doesn't exist!\n");
		reset();
	}

	// the texts are written directly in the file so reading from the file
	while ((ch = fgetc(aboutfp)) != EOF)
	{
		printf("%c", ch);
	}
	reset();
	fclose(aboutfp);
	printf("\n\n\t");
	purple();
	system("pause");
}

// acts as a title bar and to reduce redundancy ,'cause i gave headings to all the defined-funcs
void header(char *title)
{
	system("cls");

	blue(); // giving title a red color
	printf("\n\t%s\n\n", title);

	// blue-bar
	for (int i = 0; i < 156; i++)
		printf("%c", 219); // 178,219,176,206,247 looks good
	reset();
	printf("\n");
} // end of header()

// grade calculator to find the equivalent grade (like 90% above = A+)
char *grade_calc(float percent)
{
	if (percent >= 90)
		return "A+";
	else if (percent >= 80)
		return "A";
	else if (percent >= 70)
		return "B+";
	else if (percent >= 60)
		return "B";
	else if (percent >= 50)
		return "C+";
	else if (percent >= 40)
		return "C";
	else if (percent >= 30)
		return "D+";
	else if (percent >= 20)
		return "D";
	else
		return "E";
} // end of grade calc()

// to know the number of records in the file and also returning it sothat it can be used in other funcs to display "NO record found" instead of leaving it blank
int no_of_records()
{
	int n;
	FILE *fp = fopen("record.dat", "rb");
	if (fp == NULL)
	{
		red();
		printf("\n\tFile doesn't exist! Please ADD RECORD first.\n\n\t");
		purple();
		system("pause");
		menu();
	}

	fseek(fp, 0, SEEK_END);		 // moving cursor to last position for total length/size
	n = ftell(fp) / sizeof(std); // since ftell gives the size of file and std gives size of each record. so the division gives no of records
	fclose(fp);

	return n; // returning no of records
} // end of noofrecords()

// just to verify if the name contains Alphabets only, coz C reads all the characters as CHARACTERS
int nameChecker(char *name)
{
	int flag = 0;
	FILE *namefp = fopen("record.dat", "rb");
	struct student temp;

	// also checking if the name exists in the file
	while (fread(&temp, sizeof(struct student), 1, namefp))
	{
		if (strcmp(temp.name, name) == 0)
		{
			fclose(namefp);
			return 2;
		}
	}
	for (int i = 0; name[i] != '\0'; i++)
	{
		if ((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z')) // sorry elon musk and sons
			flag = 0;
		else
			flag = 1;
	}

	return flag;
} // end of nameChecker()

// to check if the rollno already exists in the main file
int rollChecker(int roll)
{
	FILE *rollfp = fopen("record.dat", "rb");
	struct student temp;

	while (fread(&temp, sizeof(struct student), 1, rollfp))
	{
		if (roll == temp.roll)
		{
			fclose(rollfp);
			return 1;
		}
	}

} // end of rollChecker()

// login page
void login()
{
	header("LOGIN PAGE");

	FILE *fsettings = fopen("settings.dat", "rb");
	char userinput[10], pass[10];
	int filesize = ftell(fsettings);

	while (fread(&setup.setting, sizeof(struct settings), 1, fsettings))
		;
	fclose(fsettings);

	fflush(stdin);
	reset();
	printf("\n\n\tEnter Username: ");
	scanf(" %10[^\n]", userinput);
	fflush(stdin);
	printf("\tEnter Password: ");
	scanf(" %10[^\n]", pass);

	if (strcmp(setup.setting.username, userinput) == 0 && strcmp(setup.setting.password, pass) == 0)
	{
		menu();
	}
	else
	{
		red();
		printf("\n\tInvalid Username or password!\n\t");
		printf("-----------------------------\n\t");
		purple();
		system("pause");
		login();
	}
}

void sort_file()
{
	header("SORTING RECORDS INSIDE THE FILE");

	int i, j;
	int n = no_of_records(); // getting no of records
	int choice;
	FILE *fp = fopen("record.dat", "rb");
	if (fp == NULL || n == 0)
	{
		red();
		printf("\n\tFile doesn't exist! Please ADD RECORD first.\n\n\t");
		purple();
		system("pause");
		menu();
	}
	struct student *s, temp;

	// allocating memory for struct s and storing file records in s[] as array
	s = (struct student *)calloc(n, sizeof(struct student));
	for (i = 0; i < n; i++)
	{
		fread(&s[i], sizeof(struct student), 1, fp);
	}
	fclose(fp);
	// sort menu to choose either to arrange acc. to rollno, name or gpa
	printf("\n\t1.Sort by Name\n\t2.Sort by Rollno\n\t3.Sort by GPA\n\t0.Back to Menu\n");
	green();
	printf("\n\tEnter a choice:\n\t>> ");
	scanf("%d", &choice);
	reset();

	if (choice == 1)
	{
		// sorting according to alphabetical order
		for (i = 0; i < n; i++)
			for (j = i + 1; j < n; j++)
			{
				if (strcmp(s[i].name, s[j].name) > 0)
				{
					temp = s[i];
					s[i] = s[j];
					s[j] = temp;
				}
			} // j forloop
	}		  // close
	else if (choice == 2)
	{
		// sorting according to rollno of students
		for (i = 0; i < n; i++)
			for (j = i + 1; j < n; j++)
			{
				if (s[i].roll > s[j].roll)
				{
					temp = s[i];
					s[i] = s[j];
					s[j] = temp;
				}
			} // j forloop
	}		  // close
	else if (choice == 3)
	{
		// sorting according to gpa
		for (i = 0; i < n; i++)
			for (j = i + 1; j < n; j++)
			{
				if (s[i].gpa < s[j].gpa)
				{
					temp = s[i];
					s[i] = s[j];
					s[j] = temp;
				}
			} // j forloop
	}
	else
		menu();

	// making changes in the file
	fp = fopen("record.dat", "wb");
	for (i = 0; i < n; i++)
	{
		fwrite(&s[i], sizeof(struct student), 1, fp);
	}

	free(s);
	fclose(fp);
	green();
	printf("\n\tFILE SUCCESSFULLY REARRANGED!\n\n\t");
	purple();
	system("pause");
}

void misc()
{
	int choice;
	char yesno;
	do
	{
		header("SETTINGS");
		printf("\n\t1.DELETE ALL RECORDS AT ONCE\n\t2.ARRANGE RECORDS (IN FILE)\n\t3.CHANGE THEME\n\t4.CHANGE USERNAME & PASSWORD\n\t5.ADD ANOTHER EXAM RECORD\n\t6.CHANGE SETUP SETTINGS\n\t7.ABOUT\n\t0.Back to Menu\n");

		green();
		printf("\n\tEnter a choice:\n\t>> ");
		scanf("%d", &choice);
		reset();

		switch (choice)
		{
		case 1:
			red();
			printf("\n\tDo you really want to DELETE ALL THE RECORDS?(y/n)\n\t>> ");
			scanf(" %c", &yesno);

			if (yesno == 'y' || yesno == 'Y')
			{
				remove("record.dat");
				red();
				printf("\n\tRecord Deleted Successfully!\n\t--------------------------------");
			}
			else
			{
				green();
				printf("\n\tNot Doing Anything to your precious DATA.\n\t-----------------------------------------");
			}
			printf("\n\t");
			purple();
			system("pause");
			break;
		case 2:
			sort_file();
			break;
		case 3:
			changetheme();
			break;
		case 4:
			changeUsername();
			break;
		case 5:
			addAnotherRecord();
			break;
		case 6:
			red();
			printf("\n\tWarning! Changing setup will remove all the previous records from the program\n");
			reset();

			printf("\n\n\tDo you really want to Continue?(y/n)\n\t>> ");
			scanf(" %c", &yesno);
			if (yesno == 'y' || yesno == 'Y')
			{
				remove("user.dat");
				rename("record.dat", "backup.dat");
				firstsetup();
			}
			else
				menu();
			break;
		case 7:
			about();
			break;
		} // switch
	} while (choice != 0);
}

void changetheme()
{
	header("CHANGE THEME");

	int choice;
	FILE *fsettings = fopen("settings.dat", "wb");

	printf("\n\t1.LIGHT MODE (Default)\n\t2.DARK MODE\n\t\033[1;31m3.RED\n\t\033[1;35m4.PURPLE\n\t\033[1;34m5.BLUE\n\t\033[1;32m6.GREEN\n\t\033[1;30m0.Back to Menu\n");
	green();
	printf("\n\tEnter your choice:\n\t>> ");
	scanf("%d", &choice);

	reset();
	switch (choice)
	{
	case 1:
		system("color f8");
		black();
		strcpy(setup.setting.defaultcolor, "\033[1;30m");
		break;
	case 2:
		system("color 0f");
		break;
	case 3:
		red();
		strcpy(setup.setting.defaultcolor, "\033[1;31m");
		break;
	case 4:
		purple();
		strcpy(setup.setting.defaultcolor, "\033[1;35m");
		break;
	case 5:
		blue();
		strcpy(setup.setting.defaultcolor, "\033[1;34m");
		break;
	case 6:
		green();
		strcpy(setup.setting.defaultcolor, "\033[1;32m");
		break;
	case 0:
		menu();
		break;
	}
	// writing the selected color in the file if the program restart any time(tomorrow,next week, next year)
	fwrite(&setup.setting, sizeof(struct settings), 1, fsettings);
	fclose(fsettings);
}

void changeUsername()
{
	header("CHANGE USERNAME & PASSWORD");

	FILE *fsettings = fopen("settings.dat", "wb");
	char pw[10];
	char ch = fgetc(fsettings);
	if (ch == EOF)
	{
		strcpy(setup.setting.username, "admin");
		strcpy(setup.setting.password, "password");
	}
try_pw:
	fflush(stdin);
	green();
	printf("\n\tEnter Current Password: ");
	scanf(" %[^\n]s", pw);
	reset();
	// if the current password matches then only ask for new username and password
	if (strcmp(pw, setup.setting.password) == 0)
	{
	new_user_again:
		fflush(stdin);
		printf("\n\tEnter New Username: ");
		scanf(" %[^\n]s", setup.setting.username);
		if (strlen(setup.setting.username) > 10 || strlen(setup.setting.username) < 5)
		{
			red();
			printf("\n\t(Username cannot be less than 5 characters or more than 10 characters.)\n");
			goto new_user_again;
			reset();
		}
	new_pw_again:
		fflush(stdin);
		printf("\tEnter New Password: ");
		scanf(" %[^\n]s", setup.setting.password);
		if (strlen(setup.setting.password) > 10 || strlen(setup.setting.password) < 5)
		{
			red();
			printf("\n\t(Password cannot be less than 5 characters or more than 10 characters.)\n");
			reset();
			goto new_pw_again;
		}
	}
	else
	{
		red();
		printf("\tInvalid password! Try Again.\n");
		reset();
		goto try_pw;
	}

	// saving new details in the file for another time
	fwrite(&setup.setting, sizeof(struct settings), 1, fsettings);
	green();
	printf("\n\tUSERNAME & PASSWORD SUCCESSFULLY CHANGED.\n\n\t");
	purple();
	system("pause");
	fclose(fsettings);
}
void gradesheet()
{
	header("GRADE SHEET");

	int rno;
	int found = 0;
	int n = no_of_records();

	// opening the file to read data and search
	FILE *fp = fopen("record.dat", "rb");
	if (fp == NULL || n == 0)
	{
		red();
		printf("\n\tFile doesn't exist! Please ADD RECORD first.\n\n\t");
		purple();
		system("pause");
		menu();
	}

	fflush(stdin);
	green();
	printf("\n\tEnter the ROLLNUMBER of the Student: ");
	scanf("%d", &rno);
	reset();

	// reading the file and display the result with is equal to the entered array
	while (fread(&std, sizeof(struct student), 1, fp))
	{
		if (rno == std.roll)
		{
			found = 1;
			header("GRADE SHEET");

			printf("\n\t%s", strupr(setup.schoolname));
			printf("\n\t%s", strupr(setup.address));
			printf("\n\n\tGRADE-SHEET");

			printf("\n\n\tTHE GRADE(S) SECURED BY \033[1;32m%s\033[1;30m OF %s", strupr(std.name), strupr(std.address));
			printf("\n\tROLL NO: %d\tPROGRAMME: %s\t\tSEMESTER: %d\n\n", std.roll, setup.programme, setup.semester);

			printf("\n\tSN\t%-30sCREDIT\tMARKS\tGRADE\tGRADE-POINT\n", "SUBJECT");
			printf("\t---------------------------------------------------------------------------\n");
			for (int j = 0; j < setup.subs; j++)
			{
				printf("\t%02d\t%-30s\t4.0\t%.2f\t  %s\t   %.2f\n", j + 1, setup.subject[j], std.marks[j], grade_calc(std.marks[j]), std.marks[j] / 25);
			}
			printf("\n\t\t\t\t\t\tGRADE POINT AVERAGE (GPA) : %.2f\n", std.gpa);
			printf("\t---------------------------------------------------------------------------\n");

			printf("\n\t1.One Credit Hour equals 32 Clock Hours.\n\t2.TH : THEORY\tPR : PRACTICAL");
			printf("\n\t3.*@ : Absent\n");
			printf("\n\n\tCHECKED BY:\n\t%s,%s\n", setup.schoolname, setup.address);
			printf("\tDATE OF ISSUE: ");
			system("date /t");
		}
	}
	if (!found)
	{
		red();
		printf("\n\tNo Match Found!\n\t----------------");
		reset();
	}
	printf("\n\t");
	purple();
	system("pause");
	menu();
}

/*
 *	Getting the color which user selected during previous run, stored in color.dat file
 */
void gettheme()
{
	FILE *fsettings = fopen("settings.dat", "rb");
	char ch;
	int i = 0, filesize = ftell(fsettings);
	if (fsettings == NULL || filesize == 0)
	{
		strcpy(setup.setting.defaultcolor, "\033[1;30m"); // default is black so if file is not found just apply black color to text
	}
	while (fread(&setup.setting, sizeof(struct settings), 1, fsettings))
		;

	fclose(fsettings);
}

/********************************************************************************************************
 *	COLOR CODES FUCNTIONS																				*
 ********************************************************************************************************/

void black()
{
	printf("\033[1;30m");
}
void red()
{
	printf("\033[1;31m"); // changes text to red
}
void green()
{
	printf("\033[1;32m");
}
void yellow()
{
	printf("\033[33m");
}
void reset()
{
	// this will be the default font
	printf("%s", setup.setting.defaultcolor);
}
void blue()
{
	printf("\033[1;34m");
}
void purple()
{
	printf("\033[1;35m");
}
void cyan()
{
	printf("\033[1;36m");
}
void white()
{
	printf("\033[1;37m");
}