/*
 *	This is a student record management program. I ve tried to give function to those which i think wud be big blocks
 *	of code. I have used color codes to make this program colorful, the colors are defined in each function .
 */

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#define subs 5 // no of subjects

//global variables
FILE *fp;
int i, j;
char subject[subs][20] = {"Maths", "English", "C Programming", "Digital Logic", "Computer Concept"};

struct settings {
	char username[10];
	char password[10];
	char defaultcolor[12];
} settings;
//structure
struct student {
	int roll;
	char name[24];
	float marks[subs];
	float percent;
	float total;
	float gpa;
	char grade[3];
} std;

//functions declaration
void menu();
void add_record();
void view_record();
void edit_record();
void search_record();
void delete_record();
void sort_record();
int no_of_records();
void about();
void sort_file();
void misc();
void changetheme();
void changeUsername();
//helping funcs
void login();
void input_details();
void title_banner(char*);
char *grade_calc(float);
int nameChecker(char*);
int rollChecker(int);

//colors for design purpose
void black() {
	printf("\033[1;30m");
}
void red() {
	printf("\033[1;31m"); // changes text to red
}
void green() {
	printf("\033[1;32m");
}
void yellow() {
	printf("\033[33m");
}
void reset() {
	//this will be the default font
	printf("%s",settings.defaultcolor);
}
void blue() {
	printf("\033[1;34m");
}
void purple() {
	printf("\033[1;35m");
}
void cyan() {
	printf("\033[1;36m");
}
void white() {
	printf("\033[1;37m");
}

/*
 *	Getting the color which user selected during previous run, stored in color.txt file
 */
void gettheme() {
	FILE *fsettings = fopen("settings.txt", "r");
	char ch;
	int i = 0,filesize= ftell(fsettings);
	if (fsettings == NULL || filesize == 0) {
		strcpy(settings.defaultcolor,"\033[1;30m");	//default is black so if file is not found just apply black color to text
	}
	while(fread(&settings,sizeof(struct settings),1,fsettings));

	fclose(fsettings);
}
///main function
void main() {
	system("color f8");
	gettheme(); //getting previous color which user selected during previous run
//	menu();
	login();
}

//menu to choose either to record, display , delete , edit etc. and also runs all funcs, so calling it in the main() is enough
void menu() {
	int choice;
	do {
		//for design purpose only
		system("cls");
		blue();
		printf("\n\tSTUDENTS RECORD MANAGER\n\n");
		for (i = 0; i < 150; i++)
			printf("%c", 219); //end of design //178,219,176,206 looks good
		reset();

		printf("\n\n\n\tWELCOME TO THE MAIN MENU\n\t------------------------");
		printf("\n\n\t1.ADD RECORD\n\t2.VIEW RECORD\n\t3.EDIT RECORD\n\t4.SEARCH RECORD\n\t5.DELETE RECORD\n\t6.SORT RECORDS");
		printf("\n\t7.NO OF RECORDS\n\t8.ABOUT\n\t9.SETTINGS\n\t0.EXIT\n");
		fflush(stdin);

		green();
		printf("\n\n\tEnter your choice:\n\t>> ");
		scanf("%d", &choice);
		reset();

		switch (choice) {
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
				title_banner("NO OF RECORDS");
				printf("\n\n\tNO OF RECORDS = %d\n\n\t", no_of_records());
				purple();
				system("pause");
				break;
			case 8:
				about();
				break;
			case 9:
				//settings previously named miscellaneous
				misc();
				break;
			case 0:
				exit(0);
		}
	} while (choice != 0);
} //end of menu()

//to add record
void add_record() {
	title_banner("ADD RECORD"); //menu title

	FILE *fadd = fopen("record.txt", "ab"); //opening file to add record
	int n = no_of_records();
	char yesno;

	//adding records using inputdetails() which asks info
	black();
	printf("\n\tRecord No. %d\n\t------------", n + 1);
	reset();
	input_details();
	fwrite(&std, sizeof(std), 1, fadd);
	fclose(fadd);
	green();
	printf("\n\tRecord successfully stored!\n\t---------------------------\n");
	reset();
	//if user wants to add more record

	printf("\tDo you want to add more?(Press 'y' for Yes or 'n' for No)\n\t>> ");
	scanf(" %c", &yesno);

	if (yesno == 'y' || yesno == 'Y')
		add_record();
	else
		menu();
} //end of add record function

//to display the records
void view_record() {
	title_banner("VIEW RECORD");
	int record_count = no_of_records();
	fp = fopen("record.txt", "rb");
	//checking if the file has atleast one record,otherwise to print no record found
	if (fp == NULL || record_count == 0) {
		red();
		printf("\n\tNo Record Found!! Please ADD RECORD first.\n\n\t");
		purple();
		system("pause");
		menu();
	}

	//reading the file and displaying the content
	printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
	printf("\t-------------------------------------------------------------------------------------------\n");
	while (fread(&std, sizeof(std), 1, fp)) {
		printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", std.roll, std.name, std.total, std.percent, std.gpa, std.grade);
	}

	fclose(fp);
	printf("\n\t");
	purple();
	system("pause");
} //end of view records

//to edit/update records, first copying the records in a temp file and then recopying the temp file data into og file
void edit_record() {
	title_banner("EDIT RECORD");
	int rollno;
	int found = 0, record_count = no_of_records();

	//creating a new temp file to store new updated data
	FILE *tempfile = fopen("tempfile.txt", "wb");
	fp = fopen("record.txt", "rb");
	if (fp == NULL || record_count == 0) {
		red();
		printf("\n\tFile doesn't exists!! Please ADD RECORD first\n\n\t");
		purple();
		system("pause");
		menu();
	}

	//displaying the records, so that user can select which record to edit
	printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
	printf("\t-------------------------------------------------------------------------------------------\n");
	while (fread(&std, sizeof(std), 1, fp)) {
		printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", std.roll, std.name, std.total, std.percent, std.gpa, std.grade);
	}
	rewind(fp); // to move the cursor at the start to read the file again

	fflush(stdin);
	green();
	printf("\n\n\tEnter the ROLLNUMBER of the Student you want to EDIT: ");
	scanf("%d", &rollno);
	reset();

	while (fread(&std, sizeof(std), 1, fp)) {
		if (rollno == std.roll) {
			title_banner("EDIT RECORD");

			//displaying the match record first to make user clear
			printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
			printf("\t-------------------------------------------------------------------------------------------\n");
			printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", std.roll, std.name, std.total, std.percent, std.gpa, std.grade);

			//display the marks too for more detail view

			printf("\n\n\t%-25sMARKS\n", "SUBJECT");
			printf("\t------------------------------\n");
			for (j = 0; j < subs; j++)
				printf("\t%-25s%.2f\n", subject[j], std.marks[j]);

			black();
			printf("\n\tEnter New Data\n\t--------------");
			reset();

			//input details	, i didn't use the existing inputdetails() coz it gives trouble during name & rollno Verifying time.
			std.total = 0;
			std.percent = 0;

rollno_again:
			fflush(stdin);
			printf("\n\tEnter RollNo: ");
			scanf("%d", &std.roll);
			//checking if rollno is a natural number
			if (std.roll <= 0) {
				red();
				printf("\t(Invalid Rollnumber!!)");
				reset();
				goto rollno_again;
			}
			//checking if rollno already exists in the file
			if ((rollChecker(std.roll) == 1) && (std.roll != rollno)) {
				//2nd condition coz user shud be able to enter the previous rollno of that student
				red();
				printf("\t(This Rollnumber already exists!!)");
				reset();
				goto rollno_again;
			}

name_again:
			fflush(stdin);
			printf("\tEnter FullName: ");
			gets(std.name);
			//if name includes characters other than alphabets
			if (nameChecker(std.name) == 1) {
				red();
				printf("\t(Invalid Name!! Please use Alphabets Only.)\n");
				reset();
				goto name_again;
			}

			for (j = 0; j < subs; j++) {
again_marks:
				fflush(stdin);
				printf("\tEnter marks of %s: ", subject[j]);
				scanf("%f", &std.marks[j]);
				//checking for invalid input
				if (std.marks[j] < 0 || std.marks[j] > 100) {
					red();
					printf("\t(Marks cannot be lesser than 0 or greater than 100. Please enter again CAREFULLY!)\n");
					reset();
					goto again_marks;
				}
				std.total += std.marks[j];
			}
			std.percent = std.total / subs;
			std.gpa = std.percent / 25;
			strcpy(std.grade, grade_calc(std.percent));

			fwrite(&std, sizeof(std), 1, tempfile);
			found = 1;
		} //end of if(rollno == std.roll)
		else
			fwrite(&std, sizeof(std), 1, tempfile);
	}
	fclose(fp);
	fclose(tempfile);

	//	remove("record.txt");
	//	rename("tempfile.txt","record.txt");
	//copying all the data from the tempfile to the main file coz rename&remove not working
	if (found) {
		fp = fopen("record.txt", "wb");
		tempfile = fopen("tempfile.txt", "rb");

		while (fread(&std, sizeof(std), 1, tempfile)) {
			fwrite(&std, sizeof(std), 1, fp);
		}
		fclose(fp);
		fclose(tempfile);
		green();
		printf("\n\tRecord Updated Successfully!\n\t-----------------------------");
		reset();
	} else {
		red();
		printf("\n\tNo Match Found!!\n\t----------------");
		reset();
	}

	printf("\n\t");
	purple();
	system("pause");
} //end of edit record

//to search and display the individual search record
void search_record() {
	title_banner("SEARCH RECORD");
	char name[24];
	int found = 0;
	int n = no_of_records();

	//opening the file to read data and search
	fp = fopen("record.txt", "rb");
	if (fp == NULL || n == 0) {
		red();
		printf("\n\tFile doesn't exists!! Please ADD RECORD first.\n\n\t");
		purple();
		system("pause");
		menu();
	}

	fflush(stdin);
	green();
	printf("\n\n\tEnter the NAME of the Student: ");
	gets(name);
	reset();
	//reading the file and display the result with is equal to the entered array
	while (fread(&std, sizeof(std), 1, fp)) {
		if (strcmp(name, std.name) == 0) {
			found = 1;

			printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
			printf("\t-------------------------------------------------------------------------------------------\n");
			printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", std.roll, std.name, std.total, std.percent, std.gpa, std.grade);

			//display the marks too in the search for more detail view

			printf("\n\n\t%-25sMARKS\n", "SUBJECT");
			printf("\t-------------------------------\n");
			for (j = 0; j < subs; j++) {
				printf("\t%-25s%6.2f\n", subject[j], std.marks[j]);
			}
		}
	}
	if (!found) {
		red();
		printf("\n\tNo Match Found!!\n\t----------------");
		reset();
	}
	printf("\n\t");
	purple();
	system("pause");
} //end of search record()

//to delete the individual record
void delete_record() {
	title_banner("DELETE RECORD");
	int rollno;
	char yesno;
	int found = 0, record_count = no_of_records();
	//creating a new temp file to store new updated data
	FILE *tempfile = fopen("tempfile.txt", "wb");
	fp = fopen("record.txt", "rb");
	if (fp == NULL || record_count == 0) {
		red();
		printf("\n\tFile doesn't exists!! Please ADD RECORD first.\n\n\t");
		purple();
		system("pause");
		menu();
	}

	//displaying the records, so that user can select which record to delete

	printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
	printf("\t-------------------------------------------------------------------------------------------\n");
	while (fread(&std, sizeof(std), 1, fp)) {
		printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", std.roll, std.name, std.total, std.percent, std.gpa, std.grade);
	}
	rewind(fp); // to move the cursor at the start to read the file again

	fflush(stdin);
	green();
	printf("\n\n\tEnter the ROLLNUMBER of the Student you want to DELETE: ");
	scanf("%d", &rollno);
	reset();

	while (fread(&std, sizeof(std), 1, fp)) {
		if (rollno == std.roll)
			found = 1;
		else
			fwrite(&std, sizeof(std), 1, tempfile);
	}
	fclose(fp);
	fclose(tempfile);

	//	remove("record.txt");
	//	rename("tempfile.txt","record.txt");
	//copying all the data from the tempfile to the main file coz rename&remove not working
	if (found) {
		fp = fopen("record.txt", "wb");
		tempfile = fopen("tempfile.txt", "rb");

		while (fread(&std, sizeof(std), 1, tempfile)) {
			fwrite(&std, sizeof(std), 1, fp);
		}
		fclose(fp);
		fclose(tempfile);
		green();
		printf("\n\tRecord DELETED Successfully!");
		printf("\n\t-----------------------------\n\n");
		reset();
	} else {
		red();
		printf("\n\tNo Match Found!!\n\t----------------\n");
		reset();
	}
	//if user wants to delete more record

	printf("\tDo you want to DELETE Again?(Press 'y' for Yes or 'n' for No)\n\t>> ");
	scanf(" %c", &yesno);

	if (yesno == 'y' || yesno == 'Y')
		delete_record();
	else
		menu();
} //end of delete record()

//to sort records according to name or rollno or grade
void sort_record() {
	title_banner("SORT RECORDS");

	int choice,subchoice;
	struct student *s, temp;
	int n = no_of_records(); //getting no of records

	//opening file to read the data
	fp = fopen("record.txt", "rb");
	if (fp == NULL || n == 0) {
		red();
		printf("\n\tFile doesn't exists!! Please ADD RECORD first.\n\n\t");
		purple();
		system("pause");
		menu();
	}

	//allocating memory for struct s and storing file records in s[] as array
	s = (struct student *)calloc(n, sizeof(struct student));
	for (i = 0; i < n; i++) {
		fread(&s[i], sizeof(struct student), 1, fp);
	}

	//sort menu to choose either to arrange acc. to rollno, name or gpa
	printf("\n\t1.Sort by Name\n\t2.Sort by Rollno\n\t3.Sort by GPA\n\t4.Sort by Subject (HIGHEST)\n\t5.Sort by SerialNo.\n\t6.SPREADSHEET\n\t0.Back to Menu\n");
	green();
	printf("\n\tEnter a choice:\n\t>> ");
	scanf("%d", &choice);
	reset();

	switch(choice) {
		case 1:
			title_banner("SORTING BY NAME");
			//sorting according to alphabetical order
			for (i = 0; i < n; i++)
				for (j = i + 1; j < n; j++) {
					if (strcmp(s[i].name, s[j].name) > 0) {
						temp = s[i];
						s[i] = s[j];
						s[j] = temp;
					}
				} //j forloop
			break;	  //close
		case 2:
			title_banner("SORTING BY ROLLNO");
			//sorting according to rollno of students
			for (i = 0; i < n; i++)
				for (j = i + 1; j < n; j++) {
					if (s[i].roll > s[j].roll) {
						temp = s[i];
						s[i] = s[j];
						s[j] = temp;
					}
				} //j forloop
			break; //close
		case 3:
			title_banner("SORTING BY GPA");
			//sorting according to gpa
			for (i = 0; i < n; i++)
				for (j = i + 1; j < n; j++) {
					if (s[i].gpa < s[j].gpa) {
						temp = s[i];
						s[i] = s[j];
						s[j] = temp;
					}
				} //j forloop
			break;
		case 4:
			do {
				title_banner("SORT BY SUBJECT (HIGHEST)");

				printf("\n\n");
				for(i=0; i<subs; i++) {
					printf("\t%d.%s\n",i+1,subject[i]);
				}
				printf("\t0.Back to Menu\n");
				printf("\n\tEnter your choice:\n\t>> ");
				\
				scanf("%d",&subchoice);

				switch(subchoice) {
					case 1:
						title_banner(subject[0]);
						//sorting according to subject[0]
						for (i = 0; i < n; i++)
							for (j = i + 1; j < n; j++) {
								if (s[i].marks[0] < s[j].marks[0]) {
									temp = s[i];
									s[i] = s[j];
									s[j] = temp;
								}
							} //j forloop
						printf("\n\n\tROLL\t%-30sMath\tEng.\tCpro\tDig.\tComp\t\tTOTAL\t\tPERCENT\t\t%-8s\tGRADE\n\t", "NAME", "GPA");
						for(i=0; i<150; i++)	//for line design
							printf("-");
						printf("\n");
						//displaying all the data including individual marks in each subjects
						for (i = 0; i < n; i++) {
							printf("\t%02d\t%-30s\033[1;32m%05.1f\t\033[1;30m%05.1f\t%05.1f\t%05.1f\t%05.1f\t\t%6.2f\t\t%6.2f %%\t%-10.2f\t %s\n", s[i].roll, s[i].name,s[i].marks[0],s[i].marks[1],s[i].marks[2],s[i].marks[3],s[i].marks[4], s[i].total, s[i].percent, s[i].gpa, s[i].grade);
						}
						printf("\n\t");
						purple();
						system("pause");
						break;
					case 2:
						title_banner(subject[1]);
						//sorting according to subject[0]
						for (i = 0; i < n; i++)
							for (j = i + 1; j < n; j++) {
								if (s[i].marks[1] < s[j].marks[1]) {
									temp = s[i];
									s[i] = s[j];
									s[j] = temp;
								}
							} //j forloop
						printf("\n\n\tROLL\t%-30sMath\tEng.\tCpro\tDig.\tComp\t\tTOTAL\t\tPERCENT\t\t%-8s\tGRADE\n\t", "NAME", "GPA");
						for(i=0; i<150; i++)	//for line design
							printf("-");
						printf("\n");
						//displaying all the data including individual marks in each subjects
						for (i = 0; i < n; i++) {
							printf("\t%02d\t%-30s%05.1f\t\033[1;32m%05.1f\033[1;30m\t%05.1f\t%05.1f\t%05.1f\t\t%6.2f\t\t%6.2f %%\t%-10.2f\t %s\n", s[i].roll, s[i].name,s[i].marks[0],s[i].marks[1],s[i].marks[2],s[i].marks[3],s[i].marks[4], s[i].total, s[i].percent, s[i].gpa, s[i].grade);
						}
						printf("\n\t");
						purple();
						system("pause");
						break;
					case 3:
						title_banner(subject[2]);
						//sorting according to subject[0]
						for (i = 0; i < n; i++)
							for (j = i + 1; j < n; j++) {
								if (s[i].marks[2] < s[j].marks[2]) {
									temp = s[i];
									s[i] = s[j];
									s[j] = temp;
								}
							} //j forloop
						printf("\n\n\tROLL\t%-30sMath\tEng.\tCpro\tDig.\tComp\t\tTOTAL\t\tPERCENT\t\t%-8s\tGRADE\n\t", "NAME", "GPA");
						for(i=0; i<150; i++)	//for line design
							printf("-");
						printf("\n");
						//displaying all the data including individual marks in each subjects
						for (i = 0; i < n; i++) {
							printf("\t%02d\t%-30s%05.1f\t%05.1f\t\033[1;32m%05.1f\033[1;30m\t%05.1f\t%05.1f\t\t%6.2f\t\t%6.2f %%\t%-10.2f\t %s\n", s[i].roll, s[i].name,s[i].marks[0],s[i].marks[1],s[i].marks[2],s[i].marks[3],s[i].marks[4], s[i].total, s[i].percent, s[i].gpa, s[i].grade);
						}
						printf("\n\t");
						purple();
						system("pause");
						break;
					case 4:
						title_banner(subject[3]);
						//sorting according to subject[0]
						for (i = 0; i < n; i++)
							for (j = i + 1; j < n; j++) {
								if (s[i].marks[3] < s[j].marks[3]) {
									temp = s[i];
									s[i] = s[j];
									s[j] = temp;
								}
							} //j forloop
						printf("\n\n\tROLL\t%-30sMath\tEng.\tCpro\tDig.\tComp\t\tTOTAL\t\tPERCENT\t\t%-8s\tGRADE\n\t", "NAME", "GPA");
						for(i=0; i<150; i++)	//for line design
							printf("-");
						printf("\n");
						//displaying all the data including individual marks in each subjects
						for (i = 0; i < n; i++) {
							printf("\t%02d\t%-30s%05.1f\t%05.1f\t%05.1f\t\033[1;32m%05.1f\033[1;30m\t%05.1f\t\t%6.2f\t\t%6.2f %%\t%-10.2f\t %s\n", s[i].roll, s[i].name,s[i].marks[0],s[i].marks[1],s[i].marks[2],s[i].marks[3],s[i].marks[4], s[i].total, s[i].percent, s[i].gpa, s[i].grade);
						}
						printf("\n\t");
						purple();
						system("pause");
						break;
					case 5:
						title_banner(subject[4]);
						//sorting according to subject[0]
						for (i = 0; i < n; i++)
							for (j = i + 1; j < n; j++) {
								if (s[i].marks[4] < s[j].marks[4]) {
									temp = s[i];
									s[i] = s[j];
									s[j] = temp;
								}
							} //j forloop
						printf("\n\n\tROLL\t%-30sMath\tEng.\tCpro\tDig.\tComp\t\tTOTAL\t\tPERCENT\t\t%-8s\tGRADE\n\t", "NAME", "GPA");
						for(i=0; i<150; i++)	//for line design
							printf("-");
						printf("\n");
						//displaying all the data including individual marks in each subjects
						for (i = 0; i < n; i++) {
							printf("\t%02d\t%-30s%05.1f\t%05.1f\t%05.1f\t%05.1f\t\033[1;32m%05.1f\033[1;30m\t\t%6.2f\t\t%6.2f %%\t%-10.2f\t %s\n", s[i].roll, s[i].name,s[i].marks[0],s[i].marks[1],s[i].marks[2],s[i].marks[3],s[i].marks[4], s[i].total, s[i].percent, s[i].gpa, s[i].grade);
						}
						printf("\n\t");
						purple();
						system("pause");
						break;
				}

			} while(subchoice!=0);	//end of do while
			menu();
			break;
		case 5:
			title_banner("SORTING BY SERIALNO.");
			//sorting according to alphabetical order and giving serial number
			for (i = 0; i < n; i++)
				for (j = i + 1; j < n; j++) {
					if (strcmp(s[i].name, s[j].name) > 0) {
						temp = s[i];
						s[i] = s[j];
						s[j] = temp;
					}
				} //j forloop

			//Displaying the sorted result
			printf("\n\n\tSN.\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
			printf("\t---------------------------------------------------------------------------------------------\n");
			for (i = 0; i < n; i++) {
				printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", i + 1, s[i].name, s[i].total, s[i].percent, s[i].gpa, s[i].grade);
			}

			printf("\n\t");
			purple();
			system("pause");
			menu();
			break;
		case 6:
			title_banner("SPREADSHEET");
			//sorting according to gpa
			for (i = 0; i < n; i++)
				for (j = i + 1; j < n; j++) {
					if (s[i].gpa < s[j].gpa) {
						temp = s[i];
						s[i] = s[j];
						s[j] = temp;
					}
				} //j forloop
			//displaying the sorted record

			printf("\n\n\tROLL\t%-30sMath\tEng.\tCpro\tDig.\tComp\t\tTOTAL\t\tPERCENT\t\t%-8s\tGRADE\n\t", "NAME", "GPA");
			for(i=0; i<150; i++)	//for line design
				printf("-");
			printf("\n");
			//displaying all the data including individual marks in each subjects
			for (i = 0; i < n; i++) {
				printf("\t%02d\t%-30s%05.1f\t%05.1f\t%05.1f\t%05.1f\t%05.1f\t\t%6.2f\t\t%6.2f %%\t%-10.2f\t %s\n", s[i].roll, s[i].name,s[i].marks[0],s[i].marks[1],s[i].marks[2],s[i].marks[3],s[i].marks[4], s[i].total, s[i].percent, s[i].gpa, s[i].grade);
			}
			printf("\n\t");
			purple();
			system("pause");
			menu();
			break;
		case 0:
			menu();
			break;
	}
	//displaying the sorted record
	printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\t%-12sGRADE\n", "NAME", "GPA");
	printf("\t---------------------------------------------------------------------------------------------\n");
	for (i = 0; i < n; i++) {
		printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%-14.2f%s\n", s[i].roll, s[i].name, s[i].total, s[i].percent, s[i].gpa, s[i].grade);
	}

	printf("\n\n\t");
	purple();
	system("pause");
} //end of sort_records()

//to make user input details, just declaring a seperate func , maybe used multiple times so
void input_details() {
	std.total = 0;
	std.percent = 0;

rollno_again:
	fflush(stdin);
	printf("\n\tEnter RollNo: ");
	scanf("%d", &std.roll);
	//checking if rollno is a natural number
	if (std.roll <= 0 || std.roll > 999) {	//just limiting rollnumbers to 999 cause which class wud have 1000 students
		red();
		printf("\t(Invalid Rollnumber!!)");
		reset();
		goto rollno_again;
	}
	//checking if rollno already exists in the file
	if ((rollChecker(std.roll)) == 1) {
		red();
		printf("\t(This Rollnumber already exists!!)");
		reset();
		goto rollno_again;
	}

name_again:
	fflush(stdin);
	printf("\tEnter FullName: ");
	gets(std.name);
	//if name includes characters other than alphabets
	if (nameChecker(std.name) == 1) {
		red();
		printf("\t(Invalid Name!! Please use Alphabets Only.)\n");
		reset();
		goto name_again;
	} else if (nameChecker(std.name) == 2) {
		red();
		printf("\t(This Name already exists!!)\n");
		reset();
		goto name_again;
	}

	for (j = 0; j < subs; j++) {
again_marks:
		fflush(stdin);
		printf("\tEnter marks of %s: ", subject[j]);
		scanf("%f", &std.marks[j]);
		//checking for invalid input
		if (std.marks[j] < 0 || std.marks[j] > 100) {
			red();
			printf("\t(Marks cannot be lesser than 0 or greater than 100. Please enter again CAREFULLY!)\n");
			reset();
			goto again_marks;
		}
		std.total += std.marks[j];
	}
	std.percent = std.total / subs;
	std.gpa = std.percent / 25;
	strcpy(std.grade, grade_calc(std.percent));
} //end of input_details()

//about me and the program
void about() {
	title_banner("ABOUT");

	char ch;
	//Opening the about file cause everything is written in that file
	FILE *aboutfp = fopen("about.txt", "rb");
	if (aboutfp == NULL) {
		red();
		printf("\n\tSorry! The file doesn't exists!!\n");
		reset();
	}

	//the texts are written directly in the file
	while ((ch = fgetc(aboutfp)) != EOF) {
		printf("%c", ch);
	}
	reset();
	fclose(aboutfp);
	printf("\n\n\t");
	purple();
	system("pause");
}
//acts as a title bar and to reduce redundancy ,'cause i gave headings to all the defined-funcs
void title_banner(char *title) {
	//for design purpose only
	system("cls");

	blue(); //giving title a red color
	printf("\n\t%s\n\n", title);

	for (i = 0; i < 150; i++)
		printf("%c", 176); //178,219,176,206,247 looks good
	reset();
	printf("\n");
} //end of titlebanner()

//grade calculator to find the equivalent grade (like 90% above = A+)
char *grade_calc(float percent) {
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

// to know the number of records in the file and also returning it sothat it can be used in other funcs to display "NO record found" instead of leaving it blank
int no_of_records() {
	int n;
	fp = fopen("record.txt", "rb");
	if (fp == NULL) {
		red();
		printf("\n\tFile doesn't exists!! Please ADD RECORD first.\n\n\t");
		purple();
		system("pause");
		menu();
	}

	fseek(fp, 0, SEEK_END);		 //moving cursor to last position for total length/size
	n = ftell(fp) / sizeof(std); // since ftell gives the size of file and std gives size of each record. so the division gives no of records

	fclose(fp);
	return n; //returning no of records
} // end of noofrecords()

//just to verify if the name contains Alphabets only, coz C reads all the characters as CHARACTERS
int nameChecker(char *name) {
	int flag = 0;
	FILE *namefp = fopen("record.txt", "r");
	struct student temp;

	//also checking if the name exists in the file
	while (fread(&temp, sizeof(struct student), 1, namefp)) {
		if (strcmp(temp.name, name) == 0) {
			fclose(namefp);
			return 2;
		}
	}
	for (i = 0; name[i] != '\0'; i++) {
		if ((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z')) //sorry elon musk and sons
			flag = 0;
		else
			flag = 1;
	}

	return flag;
} //end of nameChecker()

//to check if the rollno already exists in the main file
int rollChecker(int roll) {
	FILE *rollfp = fopen("record.txt", "r");
	struct student temp;

	while (fread(&temp, sizeof(struct student), 1, rollfp)) {
		if (roll == temp.roll) {
			fclose(rollfp);
			return 1;
		}
	}
} //end of rollChecker()

//login page
void login() {
	FILE *fsettings = fopen("settings.txt","r");
	title_banner("LOGIN PAGE");
	char userinput[10], pass[10];
	int filesize = ftell(fsettings);

	//if file is not created or empty
	if(fsettings == NULL || filesize == 0) {
		strcpy(settings.username, "admin");
		strcpy(settings.password, "password");
		strcpy(settings.defaultcolor,"\033[1;30m");
	}
	//reading stored username and password
	while(fread(&settings,sizeof(struct settings),1,fsettings));
	fclose(fsettings);

	fflush(stdin);
	reset();
	printf("\n\n\tEnter Username: ");
	gets(userinput);
	fflush(stdin);
	printf("\tEnter Password: ");
	gets(pass);

	if (strcmp(settings.username, userinput) == 0 && strcmp(settings.password, pass) == 0) {
		menu();
	} else {
		red();
		printf("\n\tInvalid Username or password!! (Hint: admin & password)\n\t");
		printf("-----------------------------\n\t");
		purple();
		system("pause");
		login();
	}
}

void sort_file() {
	title_banner("SORTING RECORDS INSIDE THE FILE");
	int n = no_of_records(); //getting no of records
	int choice;
	fp = fopen("record.txt", "rb");
	if (fp == NULL || n == 0) {
		red();
		printf("\n\tFile doesn't exists!! Please ADD RECORD first.\n\n\t");
		purple();
		system("pause");
		menu();
	}
	struct student *s, temp;

	//allocating memory for struct s and storing file records in s[] as array
	s = (struct student*)calloc(n, sizeof(struct student));
	for (i = 0; i < n; i++) {
		fread(&s[i], sizeof(struct student), 1, fp);
	}
	fclose(fp);
	//sort menu to choose either to arrange acc. to rollno, name or gpa
	printf("\n\t1.Sort by Name\n\t2.Sort by Rollno\n\t3.Sort by GPA\n\t0.Back to Menu\n");
	green();
	printf("\n\tEnter a choice:\n\t>> ");
	scanf("%d", &choice);
	reset();

	if (choice == 1) {
		//sorting according to alphabetical order
		for (i = 0; i < n; i++)
			for (j = i + 1; j < n; j++) {
				if (strcmp(s[i].name, s[j].name) > 0) {
					temp = s[i];
					s[i] = s[j];
					s[j] = temp;
				}
			} //j forloop
	}		  //close
	else if (choice == 2) {
		//sorting according to rollno of students
		for (i = 0; i < n; i++)
			for (j = i + 1; j < n; j++) {
				if (s[i].roll > s[j].roll) {
					temp = s[i];
					s[i] = s[j];
					s[j] = temp;
				}
			} //j forloop
	}		  //close
	else if (choice == 3) {
		//sorting according to gpa
		for (i = 0; i < n; i++)
			for (j = i + 1; j < n; j++) {
				if (s[i].gpa < s[j].gpa) {
					temp = s[i];
					s[i] = s[j];
					s[j] = temp;
				}
			} //j forloop
	} else
		menu();

	fp = fopen("record.txt", "wb");
	for (i = 0; i < n; i++) {
		fwrite(&s[i], sizeof(struct student), 1, fp);
	}
	fclose(fp);
	green();
	printf("\n\tFILE SUCCESSFULLY REARRANGED!\n\n\t");
	purple();
	system("pause");
}

void misc() {
	int choice;
	char yesno;
	do {
		title_banner("SETTINGS");
		printf("\n\t1.DELETE ALL RECORDS AT ONCE\n\t2.ARRANGE RECORDS (IN FILE)\n\t3.CHANGE THEME\n\t4.CHANGE USERNAME & PASWORD\n\t0.Back to Menu\n");

		green();
		printf("\n\tEnter a choice:\n\t>> ");
		scanf("%d", &choice);
		reset();

		switch (choice) {
			case 1:
				red();
				printf("\n\tDo you really want to DELETE ALL THE RECORDS?(y/n)\n\t>> ");
				scanf(" %c", &yesno);

				if (yesno == 'y' || yesno == 'Y') {
					remove("record.txt");
					red();
					printf("\n\tRecord Deleted Successfully!\n\n\t");
				} else {
					green();
					printf("\n\tNot Doing Anything to your precious DATA.\n");
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
		} //switch
	} while (choice != 0);
}

void changetheme() {
	title_banner("CHANGE THEME");
	int choice;
	FILE *fsettings = fopen("settings.txt", "w");

	printf("\n\t1.LIGHT MODE (Default)\n\t2.DARK MODE\n\t\033[1;31m3.RED\n\t\033[1;35m4.PURPLE\n\t\033[1;34m5.BLUE\n\t\033[1;32m6.GREEN\n\t\033[1;30m0.Back to Menu\n");
	green();
	printf("\n\tEnter your choice:\n\t>> ");
	scanf("%d", &choice);

	reset();
	switch (choice) {
		case 1:
			system("color f8");
			black();
			strcpy(settings.defaultcolor, "\033[1;30m");
			break;
		case 2:
			system("color 0f");
			break;
		case 3:
			red();
			strcpy(settings.defaultcolor, "\033[1;31m");
			break;
		case 4:
			purple();
			strcpy(settings.defaultcolor, "\033[1;35m");
			break;
		case 5:
			blue();
			strcpy(settings.defaultcolor, "\033[1;34m");
			break;
		case 6:
			green();
			strcpy(settings.defaultcolor, "\033[1;32m");
			break;
		case 0:
			menu();
			break;
	}
	//writing the selected color in the file if the program restart any time(tomorrow,next week, next year)
	fwrite(&settings,sizeof(struct settings),1,fsettings);
	fclose(fsettings);
}

void changeUsername() {
	title_banner("CHANGE USERNAME & PASSWORD");
	FILE *fsettings = fopen("settings.txt","w");
	char pw[10];

try_pw:
	fflush(stdin);
	green();
	printf("\n\tEnter Current Password: ");
	gets(pw);
	reset();
	if(strcmp(pw,settings.password)==0) {
new_user_again:
		fflush(stdin);
		printf("\n\tEnter New Username: ");
		gets(settings.username);
		if(strlen(settings.username) > 10 || strlen(settings.username) < 5) {
			\
			red();
			printf("\n\t(Username cannot be less than 5 characters or more than 10 characters.)\n");
			goto new_user_again;
			reset();
		}
new_pw_again:
		fflush(stdin);
		printf("\tEnter New Password: ");
		gets(settings.password);
		if(strlen(settings.password) > 10 || strlen(settings.password) < 5) {
			red();
			printf("\n\t(Username cannot be less than 5 characters or more than 10 characters.)\n");
			reset();
			goto new_pw_again;

		}
	} else {
		red();
		printf("\tInvalid password!! Try Again.\n");
		reset();
		goto try_pw;
	}

	fwrite(&settings,sizeof(struct settings),1,fsettings);
	green();
	printf("\n\tUSERNAME & PASSWORD SUCCESSFULLY CHANGED.\n\n\t");
	purple();
	system("pause");
	fclose(fsettings);
}