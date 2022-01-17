/************************************************************************************
 *	STUDENT RECORD MANAGEMENT SYSTEM (simplified)								*
 *	It can add, edit, store, delete, sort details of students.						*
 *	Developed and Concept by: Ankush Gautam                                 		*
 ************************************************************************************/

//header files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//structure for setup and saving password
struct setups
{
	char schoolname[32];
	char address[32];
	char programme[32];
	int semester;
	int subs;
	char subject[8][32];	//2d array using DMA

	struct settings
	{
		char username[10];
		char password[10];
	} setting;
} setup;

//structure to define a students identity
struct student
{
	//personal
	char name[24];
	char address[32];
	char phone[14];

	//academic
	int roll;
	float marks[10];
	float percent;
	float total;
	float gpa;
	char grade[2];
} std;

//FUNCTIONS
void firstsetup();
void menu();
void add_record();
void view_record();
void edit_record();
void search_record();
void delete_record();
void sort_record();
int no_of_record();
void gradesheet();
void about();
void login();

//color codes made functions
void black();
void red();
void blue();
void purple();
void green();
void reset();

//helping functions
void header(char*);
char *grade_calc(float);
void inputdetails();
int nameChecker(char*);
int rollChecker(int);

/********************************************************************************************************
 *	MAIN FUNCTION																						*
 ********************************************************************************************************/
int main()
{

	//change the name of title bar of the WINDOW "Student Record Manager"
	system("title Student Record Manager");

	//theme
	system("color f8");

	//if setup is already done , direct the user to the login menu
	FILE *fsetup = fopen("setup.dat","rb");
	if(fsetup == NULL)
	{
		firstsetup();
	}

	//reading subject and school details from the file
	fread(&setup,sizeof(struct setups),1,fsetup);
	fclose(fsetup);

	//if the setup file doesn't have correct details about subjects
	if(setup.subs <= 0)
	{
		firstsetup();
	}

	login();
}

//menu
void menu()
{
	int choice;
	do
	{
		header("STUDENT RECORD MANAGER");	//title of the page

		printf("\n\n\tWELCOME TO THE MAIN MENU\n\t------------------------");

		printf("\n\n\t1.ADD RECORD\n\t2.VIEW RECORD\n\t3.EDIT RECORD\n\t4.SEARCH RECORD");
		printf("\n\t5.DELETE RECORD\n\t6.SORT RECORDS\n\t7.NO OF RECORDS\n\t8.GRADE-SHEET\n\t9.ABOUT\n\t0.EXIT\n");

		green();
		printf("\n\tEnter your choice:\n\t>> ");
		scanf("%d",&choice);
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
				//since its short, so not declaring new function
				header("NO OF RECORDS");

				printf("\n\n\tNO OF RECORDS = %d\n\n\t", no_of_record());

				purple();
				system("pause");
				reset();
				break;
			case 8:
				gradesheet();
				break;
			case 9:
				about();
				break;
			case 0:
				red();
				printf("\n\tExiting...");
				reset();
				exit(0);
		}

	}
	while(choice!=0);
}

void add_record()
{
	header("ADD RECORD");

	FILE *fadd = fopen("record.dat","ab");
	int n = no_of_record();
	int i;
	char yesno;

	green();
	printf("\n\tRecord No. %d",n+1);
	printf("\n\t-------------");
	reset();

	inputdetails();	//asks all the details

	//writing the details in the file "record.dat"
	fwrite(&std,sizeof(struct student),1,fadd);
	fclose(fadd);

	green();
	printf("\n\tRecord successfully stored!\n\t---------------------------\n");
	reset();

	//if user wants to add more record
	printf("\tDo you want to add more?(Press 'y' for Yes)\n\t>> ");
	scanf(" %c", &yesno);

	if (yesno == 'y' || yesno == 'Y')
		add_record();
	else
		menu();
}
void view_record()
{
	header("VIEW RECORD");

	int filesize = no_of_record();
	FILE *fview = fopen("record.dat","rb");

	if(fview == NULL || filesize == 0)
	{
		red();
		printf("\n\tFile doesn't exist! Please ADD RECORD first.\n\n\t");
		purple();
		system("pause");
		menu();
	}

	//reading the file and displaying the content
	printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
	printf("\t------------------------------------------------------------------------------------------\n");
	while (fread(&std, sizeof(std), 1, fview))
	{
		printf("\t %02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", std.roll, std.name, std.total, std.percent, std.gpa, std.grade);
	}
	fclose(fview);

	printf("\n\t");
	purple();
	system("pause");
}

//to edit/update records, first copying the records in a temp file and then recopying the temp file data into og file
void edit_record()
{
	header("EDIT RECORD");

	int rollno,j;
	int found = 0, record_count = no_of_record();

	//creating a new temp file to store new updated data
	FILE *tempfile = fopen("tempfile.dat", "wb");
	FILE *fedit = fopen("record.dat", "rb");

	if (fedit == NULL || record_count == 0)
	{
		red();
		printf("\n\tFile doesn't exist! Please ADD RECORD first\n\n\t");
		purple();
		system("pause");
		menu();
	}

	//displaying the records, so that user can select which record to edit
	printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
	printf("\t-------------------------------------------------------------------------------------------\n");
	while (fread(&std, sizeof(std), 1, fedit))
	{
		printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", std.roll, std.name, std.total, std.percent, std.gpa, std.grade);
	}
	rewind(fedit); // to move the cursor at the start to read the file again

	fflush(stdin);
	green();
	printf("\n\n\tEnter the ROLLNUMBER of the Student you want to EDIT: ");
	scanf("%d", &rollno);
	reset();

	while (fread(&std, sizeof(std), 1, fedit))
	{
		if (rollno == std.roll)
		{
			header("EDIT RECORD");

			printf("\n\tNAME: %s \t ADDRESS: %s \t PHONE: %s\n",std.name,std.address,std.phone);

			//displaying the match record first to make user clear
			printf("\n\n\tROLL\tTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "GPA");
			printf("\t---------------------------------------------------------\n");
			printf("\t%02d\t%06.2f\t\t%6.2f %%\t%-14.2f%s\n", std.roll, std.total, std.percent, std.gpa, std.grade);

			//display the marks too in the search for more detail view
			printf("\n\n\t%-30s MARKS\n", "SUBJECT");
			printf("\t------------------------------------\n");
			for (j = 0; j < setup.subs; j++)
			{
				printf("\t%-30s%6.2f\n", setup.subject[j], std.marks[j]);
			}

			green();
			printf("\n\tEnter New Data\n\t--------------");
			reset();

			//input details	, i didn't use the existing inputdetails() coz it gives trouble during name & rollno Verifying time.
			std.total = 0;
			std.percent = 0;

			printf("\n\n\tPersonal Details\n\t----------------\n");

name_again:
			fflush(stdin);
			printf("\tEnter FullName: ");
			scanf(" %[^\n]s", std.name);
			//if name includes characters other than alphabets
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
			//checking if rollno is a natural number
			if (std.roll <= 0)
			{
				red();
				printf("\t(Invalid Rollnumber!)");
				reset();
				goto rollno_again;
			}
			//checking if rollno already exists in the file
			if ((rollChecker(std.roll) == 1) && (std.roll != rollno))
			{
				//2nd condition coz user shud be able to enter the previous rollno of that student
				red();
				printf("\t(This Rollnumber already exists!)");
				reset();
				goto rollno_again;
			}

			for (j = 0; j < setup.subs; j++)
			{
again_marks:
				fflush(stdin);
				printf("\tEnter marks of %s: ", setup.subject[j]);
				scanf("%f", &std.marks[j]);
				//checking for invalid input
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

			fwrite(&std, sizeof(std), 1, tempfile);
			found = 1;
		}
		else
			fwrite(&std, sizeof(std), 1, tempfile);
	}
	fclose(fedit);
	fclose(tempfile);

	if (found)
	{
		fedit = fopen("record.dat", "wb");
		tempfile = fopen("tempfile.dat", "rb");

		while (fread(&std, sizeof(std), 1, tempfile))
		{
			fwrite(&std, sizeof(std), 1, fedit);
		}
		fclose(fedit);
		fclose(tempfile);

		green();
		printf("\n\tRecord Updated Successfully.\n\t-----------------------------");
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
} //end of edit record

//to search and display the individual search record
void search_record()
{
	header("SEARCH RECORD");

	char name[24];
	int j,found = 0;
	int n = no_of_record();

	//opening the file to read data and search
	FILE *fp = fopen("record.dat", "rb");
	if (fp == NULL || n == 0)
	{
		red();
		printf("\n\tFile doesn't exist! Please ADD RECORD first.\n\n\t");
		purple();
		system("pause");
		menu();
	}

	//asking for name to search and compare in the file
	fflush(stdin);
	green();
	printf("\n\n\tEnter the NAME of the Student: ");
	scanf(" %[^\n]s", name);
	reset();

	//reading the file and display the result with is equal to the entered array
	while (fread(&std, sizeof(std), 1, fp))
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

			//display the marks too in the search for more detail view
			green();
			printf("\n\n\t%-30s MARKS\n", "SUBJECT");
			printf("\t------------------------------------\n");
			reset();
			for (j = 0; j < setup.subs; j++)
			{
				printf("\t%-30s%6.2f\n", setup.subject[j], std.marks[j]);
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
} //end of search record()

//to delete the individual record
void delete_record()
{
	header("DELETE RECORD");

	int rollno;
	char yesno;
	int found = 0, record_count = no_of_record();

	//creating a new temp file to store new updated data
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

	//displaying the records, so that user can select which record to delete
	printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
	printf("\t--------------------------------------------------------------------------------------------\n");
	while (fread(&std, sizeof(std), 1, fp))
	{
		printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", std.roll, std.name, std.total, std.percent, std.gpa, std.grade);
	}
	rewind(fp); // to move the cursor at the start to read the file again

	fflush(stdin);
	green();
	printf("\n\n\tEnter the ROLLNUMBER of the Student you want to DELETE: ");
	scanf("%d", &rollno);
	reset();

	while (fread(&std, sizeof(std), 1, fp))
	{
		if (rollno == std.roll)
			found = 1;
		else
			fwrite(&std, sizeof(std), 1, tempfile);
	}
	fclose(fp);
	fclose(tempfile);

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
		printf("\n\tRecord DELETED Successfully.");
		printf("\n\t-----------------------------\n\n");
		reset();
	}
	else
	{
		red();
		printf("\n\tNo Match Found!\n\t----------------\n");
		reset();
	}

	//if user wants to delete more record
	printf("\tDo you want to DELETE Again?(Press 'y' for Yes)\n\t>> ");
	scanf(" %c", &yesno);

	if (yesno == 'y' || yesno == 'Y')
		delete_record();
	else
		menu();

} //end of delete record()

//to sort records according to name or rollno or grade
void sort_record()
{
	int i,j,choice, subchoice;

	do
	{
		header("SORT RECORDS");

		struct student *s, temp;
		int n = no_of_record(); //getting no of records

		//opening file to read the data
		FILE *fp = fopen("record.dat", "rb");
		if (fp == NULL || n == 0)
		{
			red();
			printf("\n\tFile doesn't exist! Please ADD RECORD first.\n\n\t");
			purple();
			system("pause");
			menu();
		}

		//allocating memory for struct s and storing file records in s[] as array
		s = (struct student *)calloc(n, sizeof(struct student));
		for (i = 0; i < n; i++)
		{
			fread(&s[i], sizeof(struct student), 1, fp);
		}

		//sort menu to choose either to arrange acc. to rollno, name or gpa
		printf("\n\t1.Sort by Name\n\t2.Sort by Rollno\n\t3.Sort by GPA\n\t4.SPREADSHEET\n\t0.Back to Menu\n");
		green();
		fflush(stdin);
		printf("\n\tEnter a choice:\n\t>> ");
		scanf("%d", &choice);
		reset();

		switch (choice)
		{
			case 1:
				header("SORTING BY NAME");
				//sorting according to alphabetical order
				for (i = 0; i < n; i++)
					for (j = i + 1; j < n; j++)
					{
						if (strcmp(s[i].name, s[j].name) > 0)
						{
							temp = s[i];
							s[i] = s[j];
							s[j] = temp;
						}
					}  //j forloop
				break; //close
			case 2:
				header("SORTING BY ROLLNO");

				//sorting according to rollno of students
				for (i = 0; i < n; i++)
					for (j = i + 1; j < n; j++)
					{
						if (s[i].roll > s[j].roll)
						{
							temp = s[i];
							s[i] = s[j];
							s[j] = temp;
						}
					}  //j forloop
				break; //close
			case 3:
				header("SORTING BY GPA");

				//sorting according to gpa
				for (i = 0; i < n; i++)
					for (j = i + 1; j < n; j++)
					{
						if (s[i].gpa < s[j].gpa)
						{
							temp = s[i];
							s[i] = s[j];
							s[j] = temp;
						}
					} //j forloop

				//Displaying the sorted result
				printf("\n\n\tRANK\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
				printf("\t-------------------------------------------------------------------------------------------------\n");
				for (i = 0; i < n; i++)
				{
					printf("\t %02d\t %02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", i + 1, s[i].roll, s[i].name, s[i].total, s[i].percent, s[i].gpa, s[i].grade);
				}

				free(s);
				printf("\n\t");
				purple();
				system("pause");
				fclose(fp);
				menu();
				break;
			case 4:
				header("SPREADSHEET");

				//sorting according to gpa
				for (i = 0; i < n; i++)
					for (j = i + 1; j < n; j++)
					{
						if (s[i].gpa < s[j].gpa)
						{
							temp = s[i];
							s[i] = s[j];
							s[j] = temp;
						}
					} //j forloop

				//displaying the sorted record
				printf("\n\n\tRANK\tROLL\t%-28s", "NAME");
				for (i = 0; i < setup.subs; i++)
				{
					printf("\t%.4s.", setup.subject[i]);
				}
				printf("\t%-9s %-11s \tGPA\tGRADE\n\t", "TOTAL", "PERCENT");

				//for line design
				for (i = 0; i < 140; i++)
					printf("-");
				printf("\n");

				//displaying all the data including individual marks in each subjects
				for (i = 0; i < n; i++)
				{
					printf("\t %02d\t %02d\t%-28s%", i + 1, s[i].roll, s[i].name);
					for (j = 0; j < setup.subs; j++)
					{
						printf("\t%05.1f", s[i].marks[j]);
					}
					printf("\t%-09.2f %6.2f %%\t%.2f\t  %s\n", s[i].total, s[i].percent, s[i].gpa, s[i].grade);
				}

				printf("\n\t");
				purple();
				system("pause");
				fclose(fp);
				menu();
			case 0:
				menu();
				break;
		}
		if (choice == 1 || choice == 2)
		{

			//displaying the sorted record
			printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
			printf("\t-----------------------------------------------------------------------------------------\n");
			for (i = 0; i < n; i++)
			{
				printf("\t %02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", s[i].roll, s[i].name, s[i].total, s[i].percent, s[i].gpa, s[i].grade);
			}
			free(s);
			fclose(fp);
			printf("\n\t");
			purple();
			system("pause");
		}

	}while (choice != 0);
} //end of sort_records()

//return total number of records in the program
int no_of_record()
{

	int n;
	FILE *fp = fopen("record.dat","rb");

	if(fp == NULL)
	{
		red();
		printf("\n\tFile doesn't exist! Please ADD RECORD first.\n\n\t");
		purple();
		system("pause");
		menu();
	}

	fseek(fp,0,SEEK_END);
	n = ftell(fp)/sizeof(struct student);

	return n;
}
void gradesheet()
{
	header("GRADE-SHEET");

	int rno,j;
	int found = 0;
	int n = no_of_record();

	//opening the file to read data and search
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

	//reading the file and display the result with is equal to the entered array
	while (fread(&std, sizeof(std), 1, fp))
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
			for (j = 0; j < setup.subs; j++)
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

//about me and the program
void about()
{
	header("ABOUT");

	char ch;
	//Opening the about file cause everything is written in that file
	FILE *aboutfp = fopen("about.dat", "rb");
	if (aboutfp == NULL)
	{
		red();
		printf("\n\tSorry! The file doesn't exist!\n");
		reset();
	}

	//the texts are written directly in the file so reading from the file
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

//this function only runs if the program is running for the first time
void firstsetup()
{
	header("SETUP");

	FILE *fsetup = fopen("setup.dat","wb");

	int i;
	char verifypw[10];

	blue();
	printf("\n\n\tPlease Take a Moment to Setup.\n");
	printf("\t------------------------------\n\n");
	reset();

	printf("\tEnter School Name: ");
	scanf(" %[^\n]s", setup.schoolname);
	printf("\tEnter Address: ");
	scanf(" %[^\n]s", setup.address);
	printf("\tEnter Programme (For e.g: BCA, BBA): ");
	scanf(" %[^\n]s", setup.programme);
	printf("\tEnter semester (For e.g: 1,2,3): ");
	scanf("%d", &setup.semester);

subs_again:
	fflush(stdin);
	printf("\tEnter No. Of Subjects: ");
	scanf("%d", &setup.subs);

	//if user try to input more than 8 subjects
	if (setup.subs > 8 || setup.subs <= 0)
	{
		red();
		printf("\t(Sorry! 0 < Subject <= 8)\n");
		reset();
		goto subs_again;
	}
	printf("\n");

	//asking for Names of all the subjects and storing it
	for (i = 0; i < setup.subs; i++)
	{
		fflush(stdin);
		printf("\tEnter Name of subject %d: ", i + 1);
		scanf(" %[^\n]", setup.subject[i]);
	}

	blue();
	printf("\n\n\tSIGN UP\n\t-------\n\n");
	reset();

	printf("\tEnter Username: ");
	scanf(" %[^\n]", setup.setting.username);
	printf("\tEnter Password: ");
	scanf(" %[^\n]", setup.setting.password);
new_pw_again:
	printf("\tConfirm Password: ");
	scanf(" %[^\n]", verifypw);

	//checking if both password matches
	if (strcmp(verifypw, setup.setting.password) != 0)
	{
		red();
		printf("\t(Password doesn't match! Re-enter password.)\n");
		reset();
		goto new_pw_again;
	}

	//writing all the details of the setup in the file "setup.dat"
	fwrite(&setup,sizeof(struct setups),1,fsetup);
	fclose(fsetup);

	green();
	printf("\n\tSET UP SUCCESSFUL! Remember Your Details.\n\t-----------------------------------------\n\t");

	purple();
	system("pause");
	reset();

	menu();
}

//login page
void login()
{
	header("LOGIN PAGE");

	char userinput[10], pass[10];
	char user_code[10];

	printf("\n\n\tEnter Username: ");
	scanf(" %[^\n]", userinput);
	fflush(stdin);
	printf("\tEnter Password: ");
	scanf(" %[^\n]", pass);

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

/********************************************************************************************************
 *	HELPING FUNCTIONS																					*
 ********************************************************************************************************/

//acts as a title bar for all the main functions of this project
void header(char *title)
{
	int i;
	system("cls");

	blue(); //giving title a blue color
	printf("\n\t%s\n\n", title);

	//blue-bar
	for (i = 0; i < 158; i++)
		printf("%c", 219); //ascii characters 178,219,176,206,247 looks good
	reset();
	printf("\n");

} //end of header()

//to make user input details, just declaring a seperate func , maybe used multiple times
void inputdetails()
{
	int j;
	std.total = 0;
	std.percent = 0;
	

	printf("\n\n\tPersonal Details\n\t----------------\n");
name_again:
	fflush(stdin);
	printf("\tEnter FullName: ");
	scanf(" %[^\n]s", std.name);
	//if name includes characters other than alphabets
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
	//phone_again:
	printf("\tEnter Phone Number: ");
	scanf(" %[^\n]s", std.phone);

	printf("\n\tAcademic Details\n\t----------------\n");
rollno_again:
	fflush(stdin);
	printf("\tEnter RollNo: ");
	scanf("%d", &std.roll);
	//checking if rollno is a natural number
	if (std.roll <= 0)
	{
		//just limiting rollnumbers to 999 cause which class wud have 1000 students
		red();
		printf("\t(Invalid Rollnumber!)\n");
		reset();
		goto rollno_again;
	}
	//checking if rollno already exists in the file
	if ((rollChecker(std.roll)) == 1)
	{
		red();
		printf("\t(This Rollnumber already exists!)\n");
		reset();
		goto rollno_again;
	}

	//asking for marks in all the subjects
	for (j = 0; j < setup.subs; j++)
	{
again_marks:
		fflush(stdin);
		printf("\tEnter marks of %s: ", setup.subject[j]);
		scanf("%f", &std.marks[j]);
		//checking for invalid input
		if (std.marks[j] < 0 || std.marks[j] > 100)
		{
			red();
			printf("\t(Marks cannot be less than 0 or greater than 100!)\n");
			reset();
			goto again_marks;
		}
		std.total += std.marks[j];
	}

	//calculating results
	std.percent = std.total / setup.subs;
	std.gpa = std.percent / 25;
	strcpy(std.grade, grade_calc(std.percent));
} //end of input_details()

//grade calculator to find the equivalent grade (like 90% above = A+)
char *grade_calc(float percent)
{
	if (percent >= 90)
		return "A+";
	else if (percent >= 80 && percent < 90)
		return "A";
	else if (percent >= 70 && percent < 80)
		return "B+";
	else if (percent >= 60 && percent < 70)
		return "B";
	else if (percent >= 50 && percent < 60)
		return "C+";
	else if (percent >= 40 && percent < 50)
		return "C";
	else if (percent >= 20 && percent < 40)
		return "D";
	else
		return "E";
} //end of grade calc()

//just to verify if the name contains Alphabets only, coz C reads all the characters as CHARACTERS
int nameChecker(char *name)
{
	int i,flag = 0;
	FILE *namefp = fopen("record.dat", "rb");
	struct student temp;

	//also checking if the name exists in the file
	while (fread(&temp, sizeof(struct student), 1, namefp))
	{
		if (strcmp(temp.name, name) == 0)
		{
			fclose(namefp);
			return 2;
		}
	}
	for (i = 0; name[i] != '\0'; i++)
	{
		if ((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z')) //sorry elon musk and sons
			flag = 0;
		else
			flag = 1;
	}

	return flag;
} //end

//to check if the rollno already exists in the main file
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
} //end of rollChecker()


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
void reset()
{
	printf("\033[1;30m");//this will be the default font color
}
void blue()
{
	printf("\033[1;34m");
}
void purple()
{
	printf("\033[1;35m");
}
void white()
{
	printf("\033[1;37m");
}