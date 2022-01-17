#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define subjects 5

//global variables
int n=0, record_count=0;
char *subject[] = {"Maths","English","Digital Logic","Computer Concept","C Programming"};

//structure
typedef struct{
	int rollno;
	char name[20];
	float total;
	float percent;
	char grade[3];
	float gpa;
	float marks[subjects];
}student;

//functions declaration
void login_page();
void menu();
void add_record();
void edit();
void delete_record();
void display();
void search();
int no_of_record();
void sort_menu();
void alphabetical_sort();
void sort_by_rollno();
void sort_by_gpa();
//auxiliary functions
void about();
void line();
char *grade_calc(float);
int nameChecker(char*);
int	counter(); 

//main function
int main()
{
	//font color
	system("color 3");
	
//	login_page();	
					/**comenting it out just for now 
					and adding menu function to not
 					login again and again**/	
								
	menu();	//temporary call to be removed if login_page is uncommented
	
	return 0;
}

//line function
void line()
{
	printf("------------------------------------------------------------------------------------------------\n");
}

//login page
void login_page()
{
	char username[10], password[10], userid[] = "admin", passcode[] = "ag";
	
    tryagain: //if user name and password does not match
    line();
    printf("\tLOGIN\n");
    line();
    
    fflush(stdin);
    printf("\n\tEnter username : ");
    scanf("%s",username);
    printf("\tEnter password : ");
	scanf("%s",password);
	
    if(strncmp(username,userid,10)==0 && strncmp(password,passcode,10)==0)	//strncmp helps to control buffer overflow , useful for security purpose
    {
    	system("cls");
        menu();
    }
    else
    {
        printf("\n\tInvalid Login!\n\t");
        system("pause");
        system("cls");
        goto tryagain;
    }
}

//menu function
void menu()
{
	int choice;
	time_t t;
	time(&t);
	system("cls");
	do{
		line();
		printf("\n\tWELCOME TO THE MAIN MENU\t\t\t\t%s\n",ctime(&t));
		line();
		printf("\n\t1.ADD RECORD");
		printf("\n\t2.DISPLAY");
		printf("\n\t3.EDIT");
		printf("\n\t4.DELETE");
		printf("\n\t5.NO OF RECORDS");
		printf("\n\t6.SEARCH");
		printf("\n\t7.SORT RECORDS");
		printf("\n\t8.ABOUT");
		printf("\n\t0.EXIT");
		
		printf("\n\n\tEnter your choice : ");
		scanf("%d",&choice);
		fflush(stdin);
		
		system("cls");
		switch(choice)
		{	
			case 1:
				add_record();
				break;
			case 2:
				display();
				break;
			case 3:
				edit();
				break;
			case 4:
				delete_record();
				break;
			case 5:
				no_of_record();
				break;
			case 6:
				search();
				break;
			case 7:
				sort_menu();
				break;
			case 8:
				about();
				break;
		}
		
	}while(choice != 0);
}

//add student function
void add_record()
{
	FILE *fp = fopen("student_record.txt","a+");
	student std,temp;
	
	system("cls");
	line();
	printf("\tADD STUDENT RECORD MENU\n");
	line();
	
	int j; char yesno;
	std.total = 0;std.percent = 0;
	
	printf("\tRecord No. %d",(counter()+1));
	rollno:

	printf("\n\tEnter Rollno : ");
	scanf("%d",&std.rollno);
		fflush(stdin);
	//if user input strings or other data type
	if(std.rollno <= 0)	// because if user input character in %d(integer data type), it returns 0
	{
		printf("\tInvalid Rollnumber!!\n");
		goto rollno;		
	}	
	//checking if the rollno, user is trying already exists in the record
	while(fread(&temp,sizeof(temp),1,fp))
	{
		if(std.rollno == temp.rollno)
		{
			printf("\tRollnumber already exists!");
			goto rollno;
		}
	}
	name:

	printf("\tEnter FULL Name : ");
	scanf("%[^\n]s",std.name);
		fflush(stdin);
	//if user inputs other than alphabets
	if(nameChecker(std.name) == 1)
	{
		printf("\tInvalid Name! Please use Alphabets only\n");	//Sorry Elon Musk and son
		goto name;
	}
	
	for(j=0;j<subjects;j++)
	{
		enterMarks:		//if total exceeds the limit	
		printf("\tEnter marks of %s: ",subject[j]);
		scanf("%f",&std.marks[j]);
		fflush(stdin);
		//verifying if marks is not greater or less than it should be
		if(std.marks[j] < 0 || std.marks[j] > 100)
		{
			printf("\n\tMarks cannot be Greater than 100 and Lesser than 0!!\n");
			printf("\tEnter the Marks Again CAREFULLY.\n\n");
			goto enterMarks;
		}
		std.total += std.marks[j];
	}
	std.percent = std.total/subjects;		//calculating percentage of student
	std.gpa = std.percent/25;				//calculating gpa from percent
	strcpy(std.grade,grade_calc(std.percent));	//finding the equivalent gpa using gpa_calc
	
	//writing records into the file
	fwrite(&std,sizeof(std),1,fp);
	printf("\n\tRecord Successfully Stored!\n");
	printf("\t_____________________________________\n");
	fclose(fp);
	
	//if user wants to add more	
	printf("\n\tDo you want to ADD more Record(y/n)? => ");
	yesno = getche();
	if(yesno == 'y' || yesno == 'Y')
		add_record();
	else
		menu();
}

//display function
void display()
{
	line();
	printf("\tDISPLAY MENU\n");
	line();
	
	FILE *fp = fopen("student_record.txt","r");
	if(fp == NULL)
	{
		printf("\n\tNo Record Found to Display!!\n\t");
		system("pause");
		system("cls");
		menu();
	}
	student std;
	int j;
	char ch = fgetc(fp);
	
	if(ch != EOF)
	{
		rewind(fp);
		printf("\n\tROLL\tNAME\t\t\tTOTAL\t\tPERCENT %%\tGPA\tGRADE\n");
		line();
		while(fread(&std,sizeof(student),1,fp))
		{
			printf("\t %d\t%-20s\t%.2f\t\t%.2f%%\t\t%.2f\t %s\n",std.rollno,std.name,std.total,std.percent,std.gpa,std.grade);
		}	
	}
	else	
		printf("\n\tNo Data Found! ADD Data To Display.\n");
	
		
	printf("\n\tPress any key to continue...");
	getchar();
	system("cls");
	
	fclose(fp);
}

//edit function
void edit()
{
	FILE *fp = fopen("student_record.txt","r");
	if(fp == NULL)
	{
		printf("\n\tFile does not exist! Please record data first.\n\t");
		system("pause");
		system("cls");
		menu();
	}
	FILE *fp1 = fopen("temp.txt","w");
	student std;
	int j,rno,found=0;
	
	line();
	printf("\tEDIT MENU\n");
	line();
	
	//display message if file is empty
	if(counter(record_count) == 0)
	{
		printf("\n\tNo record found to EDIT!\n\t");
		system("pause");
		system("cls");
		menu();
	}
	
	//display the list of student so that user can see and choose the rollno to delete it
	printf("\n\tROLL\tNAME\t\t\tTOTAL\t\tPERCENT %%\tGPA\tGRADE\n");
	line();
	while(fread(&std,sizeof(student),1,fp))
	{
		printf("\t %d\t%-20s\t%.2f\t\t%.2f%%\t\t%.2f\t %s\n",std.rollno,std.name,std.total,std.percent,std.gpa,std.grade);
	}
	rewind(fp);	//moving cursor the the first line of the file to read 
	
	printf("\n\n\tEnter the Rollno of student you want to EDIT: ");
	scanf("%d",&rno);
	fflush(stdin);
	
	while(fread(&std,sizeof(student),1,fp))
	{
		if(std.rollno == rno)
		{
			found = 1;
			printf("\n\tROLL\tNAME\t\t\tTOTAL\t\tPERCENT %%\tGPA\tGRADE\n");
			printf("\t %d\t%-20s\t%.2f\t\t%.2f%%\t\t%.2f\t%s\n",std.rollno,std.name,std.total,std.percent,std.gpa,std.grade);
			
			std.total = 0;
			std.percent =0;
		
			printf("\n\tEnter New Rollno : ");
			scanf("%d",&std.rollno);
			fflush(stdin);
			printf("\tEnter New Name : ");
			scanf("%[^\n]s",std.name);
			
			for(j=0;j<subjects;j++)
			{
				fflush(stdin);
				printf("\tEnter marks of %s : ",subject[j]);
				scanf("%f",&std.marks[j]);
				std.total += std.marks[j];
			}
			std.percent = std.total/subjects;
			std.gpa = std.percent/25;
			strcpy(std.grade,grade_calc(std.percent));
			printf("\n\tRecord successfully Updated!\n\t");	
		}	
		
		fwrite(&std,sizeof(student),1,fp1);	
	}
	fclose(fp);
	fclose(fp1);
	
	if(found)
	{
		fp1 = fopen("temp.txt","r");
		fp = fopen("student_record.txt","w");
		
		while(fread(&std,sizeof(student),1,fp1))
		{
			fwrite(&std,sizeof(student),1,fp);
		}
		
		fclose(fp);
		fclose(fp1);
	}
	else
		printf("\n\tRecord Not Found.\n");
		
	printf("\n\t");
	system("pause");
	system("cls");	
}

//delete function
void delete_record()
{
	FILE *fp = fopen("student_record.txt","r");
	FILE *fp1 = fopen("temp.txt","w");
	student std;
	int j,rno,found=0;
	
	line();
	printf("\tDELETE RECORD\n");
	line();
	
	//display the list of student so that user can see and choose the rollno to delete it
	printf("\n\tROLL\tNAME\t\t\tTOTAL\t\tPERCENT %%\tGPA\tGRADE\n");
	line();
	while(fread(&std,sizeof(student),1,fp))
	{
		printf("\t %d\t%-20s\t%.2f\t\t%.2f%%\t\t%.2f\t%s\n",std.rollno,std.name,std.total,std.percent,std.gpa,std.grade);
	}
	rewind(fp);	//moving cursor the the first line of the file to read 
	
	//asking for rollno
	printf("\n\n\tEnter the Rollno of student you want to DELETE: ");
	scanf("%d",&rno);
	fflush(stdin);
	
	while(fread(&std,sizeof(student),1,fp))
	{
		if(std.rollno == rno)	
		{
			found = 1;	
			printf("\n\tRecord Successfully Deleted!\n");	
		}
		else
			fwrite(&std,sizeof(student),1,fp1);	
	}
	fclose(fp);
	fclose(fp1);
	
	if(found)
	{
		fp1 = fopen("temp.txt","r");
		fp = fopen("student_record.txt","w");
		
		while(fread(&std,sizeof(student),1,fp1))
		{
			fwrite(&std,sizeof(student),1,fp);
		}
		
		fclose(fp);
		fclose(fp1);
	}
	else
		printf("\n\tRecord Not Found.\n");
		
	printf("\n\t");
	system("pause");
	system("cls");	
}
//	data counter
int counter()
{
	FILE *fp = fopen("student_record.txt","r");
	fseek(fp,0,SEEK_END);
	n  = ftell(fp)/sizeof(student);
	fclose(fp);
	return n;	
}

// no of record function
int no_of_record()
{
	line();
    printf("\tNUMBER OF RECORDS\n");
    line(); 
    
	FILE *fp = fopen("student_record.txt","r");
	if(fp == NULL)
	{
		printf("\n\tFile does not exist!\n\t");
		system("pause");
		system("cls");
		menu();
	}
	
	fseek(fp,0,SEEK_END);
	n  = ftell(fp)/sizeof(student);
	printf("\n\tNo of Records = %d\n\n\t",n);
		
	system("pause");
    system("cls");
	fclose(fp);
}

//search record function
void search()
{
	FILE *fp = fopen("student_record.txt","r");
	student std;
	int j,rno,found=0;
	
	line();
	printf("\tSEARCH MENU\n");
	line();
	
	printf("\tEnter the Rollno of student you want to search: ");
	scanf("%d",&rno);
	getchar();
	
	while(fread(&std,sizeof(student),1,fp))
	{
		if(std.rollno == rno)
		{
			found = 1;
			printf("\n\tROLL\tNAME\t\t\tTOTAL\t\tPERCENT %%\tGPA\tGRADE\n");
			line();
			printf("\t %d\t%-20s\t%.2f\t\t%.2f%%\t\t%.2f\t %s\n",std.rollno,std.name,std.total,std.percent,std.gpa,std.grade);
			printf("\n\n\n\t%-30s%-10s\n","SUBJECT","MARKS");
			printf("\t-----------------------------------\n");
			for(j=0;j<subjects;j++)
			{
				printf("\t%-30s%-10.2f\n",subject[j],std.marks[j]);
			}
		}	
	}
	if(!found)
	{
		printf("\n\tRecord not Found!!\n");
	}
	printf("\n\t");
	system("pause");
	system("cls");
	
	fclose(fp);
}

//grade calculator 
char *grade_calc(float percent)
{
	char grade[3];
	//calculating grade
		if(percent >= 0 && percent < 20)
			return strcpy(grade,"E");
		else if(percent >= 20 && percent < 30)
        	return strcpy(grade,"D");
		else if(percent >= 30 && percent < 40)
        	return strcpy(grade,"D+");
		else if(percent >= 40 && percent < 50)
        	return strcpy(grade,"C");
   		else if(percent >= 50 && percent < 60)
       		return strcpy(grade,"C+");
    	else if(percent >= 60 && percent < 70)
        	return strcpy(grade,"B");
   		else if(percent >= 70 && percent < 80)
        	return strcpy(grade,"B+");
    	else if(percent >= 80 && percent < 90)
        	return strcpy(grade,"A");
    	else
       		return strcpy(grade,"A+");
}

//sort function
void sort_menu()
{
	int j,ch;
	student std;
	
	FILE *fp = fopen("student_record.txt","r");
	if(fp == NULL)
	{
		printf("\n\tNO RECORD FOUND TO SORT!!\n\t");
		system("pause");
		system("cls");
		menu();
	}
	
	do{
		system("cls");
		line();
		printf("\tSORT RECORDS MENU\n");
		line();
		
		//display message if file is empty
		if(counter(record_count) == 0)
		{
			printf("\n\tNo record found to SORT! Please ADD Records.\n\t");
			system("pause");
			system("cls");
			menu();
		}
		printf("\n\t1.Sort By Name");
		printf("\n\t2.Sort By RollNo.");
		printf("\n\t3.Sort By GPA");
		printf("\n\t0.Back to Menu");
		
		printf("\n\n\tEnter your choice: ");
		scanf("%d",&ch);
		getchar();
		
		switch(ch)
		{
			case 1:
				alphabetical_sort();
				break;
			case 2:
				sort_by_rollno();
				break;
			case 3:
				sort_by_gpa();
				break;
		}
		
	}while(ch != 0);
		
	printf("\n\t");
	system("cls");
	
	fclose(fp);
}

//sorting array of structure Alphabetical
void alphabetical_sort()
{
	system("cls");
	line();
	printf("\tARRANGING STUDENTS RECORD IN ALPHABETICAL ORDER\n");
	line();
	
	student *s, std;
	int i,j,n;
	FILE *fp = fopen("student_record.txt","r");
	if(fp == NULL)
	{
		printf("\n\tFiles cannot be Opened!\n");
		system("pause");
		system("cls");
		menu();
	}
	
	//calculating number of records in the file
	fseek(fp,0,SEEK_END);
	n = ftell(fp)/sizeof(student);
	s = (student*)calloc(n,sizeof(student));
	
	rewind(fp);
	
	//reading data into s[i]
	for(i=0;i<n;i++)
	{
		fread(&s[i],sizeof(student),1,fp);
	}
	
	//comparing and arranging the records in alphabetical order
	for(i=0;i<n;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(strcmp(s[i].name,s[j].name)>0)
			{
				std = s[i] ;
				s[i] = s[j];
				s[j] = std;
			}
		}
	}
	
	//printing sorted data
	printf("\n\tROLL\tNAME\t\t\tTOTAL\t\tPERCENT %%\tGPA\tGRADE\n");
	line();
	for(i=0;i<n;i++)
	{
		printf("\t %d\t%-20s\t%.2f\t\t%.2f%%\t\t%.2f\t%s\n",s[i].rollno,s[i].name,s[i].total,s[i].percent,s[i].gpa,s[i].grade);
	}
	
	printf("\n\n\t");
	system("pause");
}

void sort_by_rollno()
{
	system("cls");
	line();
	printf("\tARRANGING STUDENTS RECORD BY ROLL NUMBER\n");
	line();
	
	student *s, std;
	int i,j,n;
	FILE *fp = fopen("student_record.txt","r");
	if(fp == NULL)
	{
		printf("\n\tFiles cannot be Opened!\n");
		system("pause");
		system("cls");
		menu();
	}
	
	//calculating number of records in the file
	fseek(fp,0,SEEK_END);
	n = ftell(fp)/sizeof(student);
	s = (student*)calloc(n,sizeof(student));
	
	rewind(fp);
	
	//reading data into s[i]
	for(i=0;i<n;i++)
	{
		fread(&s[i],sizeof(student),1,fp);
	}
	
	//comparing and arranging the records by rollno
	for(i=0;i<n;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(s[i].rollno > s[j].rollno)
			{
				std = s[i];
				s[i] = s[j];
				s[j] = std;
			}
		}
	}
	
	//printing sorted data
	printf("\n\tROLL\tNAME\t\t\tTOTAL\t\tPERCENT %%\tGPA\tGRADE\n");
	line();
	for(i=0;i<n;i++)
	{
		printf("\t %d\t%-20s\t%.2f\t\t%.2f%%\t\t%.2f\t%s\n",s[i].rollno,s[i].name,s[i].total,s[i].percent,s[i].gpa,s[i].grade);
	}
	
	printf("\n\n\t");
	system("pause");
}

void sort_by_gpa()
{
	system("cls");
	line();
	printf("\tARRANGING STUDENTS RECORD BY GPA\n");
	line();
	
	student *s, std;
	int i,j,n;
	FILE *fp = fopen("student_record.txt","r");
	if(fp == NULL)
	{
		printf("\n\tFiles cannot be Opened!\n");
		system("pause");
		system("cls");
		menu();
	}
	
	//calculating number of records in the file
	fseek(fp,0,SEEK_END);
	n = ftell(fp)/sizeof(student);
	s = (student*)calloc(n,sizeof(student));
	
	rewind(fp);
	
	//reading data into s[i]
	for(i=0;i<n;i++)
	{
		fread(&s[i],sizeof(student),1,fp);
	}
	
	//comparing and arranging the records by gpa
	for(i=0;i<n;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(s[i].gpa < s[j].gpa)
			{
				std = s[i];
				s[i] = s[j];
				s[j] = std;
			}
		}
	}
	
	//printing sorted data
	printf("\n\tROLL\tNAME\t\t\tTOTAL\t\tPERCENT %%\tGPA\tGRADE\n");
	line();
	for(i=0;i<n;i++)
	{
		printf("\t %d\t%-20s\t%.2f\t\t%.2f%%\t\t%.2f\t%s\n",s[i].rollno,s[i].name,s[i].total,s[i].percent,s[i].gpa,s[i].grade);
	}
	
	printf("\n\n\t");
	system("pause");
}

//nameChecker , checks if the user input character only or not(Alphabets only)
int nameChecker(char name[20])
{
	int i=0;
	
	while(name[i]!= '\0')
	{
		if((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z'))
			return 0;
		else
			return 1;
	}
}
//about myself
void about()
{
	FILE *text = fopen("features.txt","r");
	char ch;
	system("cls");
	line();
	printf("\tABOUT\n");
	line();
	
	printf("\n\tThis Application is developed by Ankush Gautam.\n\n");
	printf("\tI made this as a project work for C programming subject. This program is purely\n\tmade in DEV C++ ");
	printf("so running in other IDE may result in bugs.\n\n\tThis Application can store name,marks of students");
	printf(" and calculate GPA. This app can\n\talso sort records according to the Name, Rollno and GPA.\n\n");
	
	ch = fgetc(text);
	while(ch != EOF)
	{
		printf("%c",ch);
		ch = fgetc(text);
	}
	fclose(text);
	printf("\n\n\tTHANK YOU!!\n\n\t");
	system("pause");
	system("cls");
}