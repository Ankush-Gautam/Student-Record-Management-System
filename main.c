#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#define subs 5 // no of subjects

//structure
struct student{
	int roll;
	char name[24];
	float marks[subs];
	float percent;
	float total;
	float gpa;
	char grade[3];
}std;

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
//helping funcs
void login();
void input_details();
void title_banner(char*);
char *grade_calc(float);
int nameChecker(char*);
int rollChecker(int);
//text color for designing
void red () {
  printf("\033[1;31m");	// changes text to red
}
void yellow() {
  printf("\033[1;33m");
}
void reset() {
  printf("\033[0m");
}
void green(){
	printf("\033[0;32m");
}
void blue(){
	printf("\033[0;34m");
}
void purple(){
	printf("\033[0;35m");
}
void cyan(){
	printf("\033[0;36m");
}

//global variables
FILE *fp;
int i,j;
char subject[subs][20] = {"Maths","English","C Programming","Digital Logic","Computer Concept"};

///main function
void main(){
	
	menu();
//	login();
}

//menu to choose either to record, display , delete , edit etc. and also runs all funcs, so calling it in the main() is enough
void menu(){
	int choice;
	
	do{
		//for design purpose only
		system("cls");
		cyan();
		for(i=0;i<120;i++)
			printf("%c",206);		//178,219,176,206 looks good
		red();
		printf("\n\n\t\t\t\t\t  STUDENT RECORD MANGER\n\t\t\t\t\tWELCOME TO THE MAIN MENU\n\n");
		cyan();
		for(i=0;i<120;i++)
			printf("%c",206);	//end of design
		reset();
		printf("\n\n\t1.ADD RECORD\n\t2.VIEW RECORD\n\t3.EDIT RECORD\n\t4.SEARCH RECORD\n\t5.DELETE RECORD\n\t6.SORT RECORDS");
		printf("\n\t7.NO OF RECORDS\n\t8.ABOUT\n\t9.MISCELLANEOUS\n\t0.EXIT\n");
		fflush(stdin);
		yellow();
		printf("\n\n\tEnter your choice:\n\t>> ");
		scanf("%d",&choice);
		reset();
		switch(choice){
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
				printf("\n\n\tNO OF RECORDS = %d\n\n\t",no_of_records());
				yellow();
				system("pause");
				break;
			case 8:
				about();
				break;
			case 9:
				misc();
				break;
			case 0:
				exit(0);
		}
	}while(choice!=0);
}	//end of menu()

//to add record
void add_record(){
	title_banner("ADD RECORD");	//menu title	
	
	FILE *fadd = fopen("record.txt","ab");	//opening file to add record
	int n = no_of_records();
	char yesno;
	
	//adding records using inputdetails() which asks info
	green();
	printf("\n\tRecord No. %d\n\t------------",n+1);
	reset();
	input_details();	
	fwrite(&std,sizeof(std),1,fadd);
	fclose(fadd);
	green();
	printf("\n\tRecord successfully stored!\n\t---------------------------\n");
	reset();
	//if user wants to add more record
	yellow();
	printf("\tDo you want to add more?(Press 'y' for Yes or 'n' for No)\n\t>> ");
	scanf(" %c",&yesno);
	reset();
	if(yesno == 'y' || yesno == 'Y')
		add_record();
	else
		menu();
}	//end of add record function

//to display the records
void view_record(){
	title_banner("VIEW RECORD");
	int record_count = no_of_records();
	fp = fopen("record.txt","rb");
	//checking if the file has atleast one record,otherwise to print no record found
	if(fp == NULL || record_count == 0){
		red();
		printf("\n\tNo Record Found!! Please ADD RECORD first.\n\n\t");
		yellow();
		system("pause");
		menu();
	}
	
	//reading the file and displaying the content
	green();
	printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\tGPA\t\tGRADE\n","NAME");
	printf("\t---------------------------------------------------------------------------------------------\n");reset();
	while(fread(&std,sizeof(std),1,fp)){
		printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%.2f\t\t %s\n",std.roll,std.name,std.total,std.percent,std.gpa,std.grade);
	}
	
	fclose(fp);
	printf("\n\t");
	yellow();
	system("pause");
}	//end of view records

//to edit/update records, first copying the records in a temp file and then recopying the temp file data into og file
void edit_record(){
	title_banner("EDIT RECORD");
	int rollno;
	int found = 0,record_count = no_of_records();
	
	//creating a new temp file to store new updated data
	FILE *tempfile = fopen("tempfile.txt","wb");
	fp = fopen("record.txt","rb");
	if(fp == NULL || record_count == 0){
		red();
		printf("\n\tFile doesn't exists!! Please ADD RECORD first\n\n\t");
		yellow();
		system("pause");
		menu();
	}
	
	//displaying the records, so that user can select which record to edit
	green();
	printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\tGPA\t\tGRADE\n","NAME");
	printf("\t---------------------------------------------------------------------------------------------\n");reset();
	while(fread(&std,sizeof(std),1,fp)){
		printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%.2f\t\t %s\n",std.roll,std.name,std.total,std.percent,std.gpa,std.grade);
	}
	rewind(fp); // to move the cursor at the start to read the file again
	
	fflush(stdin);
	yellow();
	printf("\n\n\tEnter the ROLLNUMBER of the Student you want to EDIT: ");
	scanf("%d",&rollno);
	reset();
	
	while(fread(&std,sizeof(std),1,fp)){
		if(rollno == std.roll){
			title_banner("EDIT RECORD");
			
			//displaying the match record first to make user clear
			green();
			printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\tGPA\t\tGRADE\n","NAME");
			printf("\t---------------------------------------------------------------------------------------------\n");reset();
			printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%.2f\t\t %s\n",std.roll,std.name,std.total,std.percent,std.gpa,std.grade);
		
			//display the marks too for more detail view
			green();
			printf("\n\n\t%-25sMARKS\n","SUBJECT");
			printf("\t------------------------------\n");reset();
			for(j=0;j<subs;j++)
				printf("\t%-25s%.2f\n",subject[j],std.marks[j]);
			
			green();
			printf("\n\tEnter New Data\n\t--------------");
			reset();

			//input details	, i didn't use the existing inputdetails() coz it gives trouble during name & rollno Verifying time.
			std.total = 0;
			std.percent = 0;
		
			rollno_again:
				fflush(stdin);
				printf("\n\tEnter RollNo: ");
				scanf("%d",&std.roll);
				//checking if rollno is a natural number
				if(std.roll <= 0){
					red();
					printf("\t(Invalid Rollnumber!!)");
					reset();
					goto rollno_again;
				}
				//checking if rollno already exists in the file
				if((rollChecker(std.roll) == 1) && (std.roll!=rollno)){	//2nd condition coz user shud be able to enter the previous rollno of that student
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
				if(nameChecker(std.name) == 1){
					red();
					printf("\t(Invalid Name!! Please use Alphabets Only.)\n");
					reset();
					goto name_again;
				}
			
			for(j=0;j<subs;j++)
			{
				again_marks:
					fflush(stdin);
					printf("\tEnter marks of %s: ",subject[j]);
					scanf("%f",&std.marks[j]);
					//checking for invalid input
					if(std.marks[j] < 0 || std.marks[j] > 100){
						red();
						printf("\t(Marks cannot be lesser than 0 or greater than 100. Please enter again CAREFULLY!)\n");
						reset();
						goto again_marks;
					}
					std.total += std.marks[j];
			}
			std.percent= std.total/subs;
			std.gpa = std.percent/25;
			strcpy(std.grade,grade_calc(std.percent));
			
			fwrite(&std,sizeof(std),1,tempfile);
			found = 1;
		}	//end of if(rollno == std.roll)
		else
			fwrite(&std,sizeof(std),1,tempfile);
	}
	fclose(fp);
	fclose(tempfile);
	
//	remove("record.txt");
//	rename("tempfile.txt","record.txt");
	//copying all the data from the tempfile to the main file coz rename&remove not working
	if(found){
		fp = fopen("record.txt","wb");
		tempfile = fopen("tempfile.txt","rb");
		
		while(fread(&std,sizeof(std),1,tempfile)){
			fwrite(&std,sizeof(std),1,fp);
		}
		fclose(fp);
		fclose(tempfile);
		green();
		printf("\n\tRecord Updated Successfully!\n\t-----------------------------");
		reset();
	}	
	else{
		red();
		printf("\n\tNo Match Found!!\n\t----------------");
		reset();
	}
		
	printf("\n\t");
	yellow();
	system("pause");
}	//end of edit record

//to search and display the individual search record
void search_record(){
	title_banner("SEARCH RECORD");
	char name[24];
	int found=0;
	int n = no_of_records();

	//opening the file to read data and search
	fp = fopen("record.txt","rb");
	if(fp == NULL || n == 0){
		red();
		printf("\n\tFile doesn't exists!! Please ADD RECORD first.\n\n\t");
		yellow();
		system("pause");
		menu();
	}
	yellow();
	fflush(stdin);
	printf("\n\n\tEnter the NAME of the Student: ");
	gets(name);
	reset();
	//reading the file and display the result with is equal to the entered array
	while(fread(&std,sizeof(std),1,fp)){
		if(strcmp(name,std.name)==0){
			found = 1;
			green();
			printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\tGPA\t\tGRADE\n","NAME");
			printf("\t---------------------------------------------------------------------------------------------\n");reset();
			printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%.2f\t\t %s\n",std.roll,std.name,std.total,std.percent,std.gpa,std.grade);
			
			//display the marks too in the search for more detail view
			green();
			printf("\n\n\t%-25sMARKS\n","SUBJECT");
			printf("\t-------------------------------\n");reset();
			for(j=0;j<subs;j++){
				printf("\t%-25s%6.2f\n",subject[j],std.marks[j]);
			}
		}
	}
	if(!found){
		red();
		printf("\n\tNo Match Found!!\n\t----------------");
		reset();
	}
	printf("\n\t");
	yellow();
	system("pause");
}	//end of search record()

//to delete the individual record
void delete_record(){
	title_banner("DELETE RECORD");
	int rollno;
	char yesno;
	int found = 0, record_count= no_of_records();
	//creating a new temp file to store new updated data
	FILE *tempfile = fopen("tempfile.txt","wb");
	fp = fopen("record.txt","rb");
	if(fp == NULL || record_count == 0){
		red();
		printf("\n\tFile doesn't exists!! Please ADD RECORD first.\n\n\t");
		yellow();
		system("pause");
		menu();
	}
	
	//displaying the records, so that user can select which record to delete
	green();
	printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\tGPA\t\tGRADE\n","NAME");
	printf("\t---------------------------------------------------------------------------------------------\n");reset();
	while(fread(&std,sizeof(std),1,fp)){
		printf("\t%02d\t%-30s%06.2f\t\t%6.2f %%\t%.2f\t\t %s\n",std.roll,std.name,std.total,std.percent,std.gpa,std.grade);
	}
	rewind(fp); // to move the cursor at the start to read the file again
	
	fflush(stdin);
	yellow();
	printf("\n\n\tEnter the ROLLNUMBER of the Student you want to DELETE: ");
	scanf("%d",&rollno);
	reset();
	
	while(fread(&std,sizeof(std),1,fp)){
		if(rollno == std.roll)
			found = 1;
		else
			fwrite(&std,sizeof(std),1,tempfile);
	}
	fclose(fp);
	fclose(tempfile);
	
//	remove("record.txt");
//	rename("tempfile.txt","record.txt");
	//copying all the data from the tempfile to the main file coz rename&remove not working
	if(found){
		fp = fopen("record.txt","wb");
		tempfile = fopen("tempfile.txt","rb");
		
		while(fread(&std,sizeof(std),1,tempfile)){
		fwrite(&std,sizeof(std),1,fp);
		}
		fclose(fp);
		fclose(tempfile);
		green();
		printf("\n\tRecord DELETED Successfully!");
		printf("\n\t-----------------------------\n\n");
		reset();
	}
	else{
		red();
		printf("\n\tNo Match Found!!\n\t----------------\n");
		reset();
	}
	//if user wants to delete more record
	yellow();
	printf("\tDo you want to DELETE Again?(Press 'y' for Yes or 'n' for No)\n\t>> ");
	scanf(" %c",&yesno);
	reset();
	if(yesno == 'y' || yesno == 'Y')
		delete_record();
	else
		menu();			
}	//end of delete record()

//to sort records according to name or rollno or grade
void sort_record(){
	title_banner("SORT RECORDS");
	
	int choice;	
	struct student *s,temp;
	int n = no_of_records();	//getting no of records
	
	//opening file to read the data
	fp = fopen("record.txt","rb");
	if(fp == NULL || n == 0){
		red();
		printf("\n\tFile doesn't exists!! Please ADD RECORD first.\n\n\t");
	yellow();
		system("pause");
		menu();
	}
	
	//allocating memory for struct s and storing file records in s[] as array
	s = (struct student*) calloc(n,sizeof(struct student));
	for(i=0;i<n;i++){
		fread(&s[i],sizeof(struct student),1,fp);
	}
	
	//sort menu to choose either to arrange acc. to rollno, name or gpa
	printf("\n\t1.Sort by Name\n\t2.Sort by Rollno\n\t3.Sort by GPA\n\t4.Sort By SerialNo.\n\t0.Back to Menu\n");
	yellow();
	printf("\n\tEnter a choice:\n\t>> ");
	scanf("%d",&choice);
	reset();
	
	if(choice == 1){
		title_banner("SORTING BY NAME");
		//sorting according to alphabetical order
		for(i=0;i<n;i++)
			for(j=i+1;j<n;j++){
				if(strcmp(s[i].name,s[j].name)>0){
					temp = s[i];
					s[i] = s[j];
					s[j] = temp;
				}	
			}	//j forloop
	}//close
	else if(choice == 2){
		title_banner("SORTING BY ROLLNO");
		//sorting according to rollno of students
		for(i=0;i<n;i++)
			for(j=i+1;j<n;j++){
				if(s[i].roll > s[j].roll){
					temp = s[i];
					s[i] = s[j];
					s[j] = temp;
				}	
			}	//j forloop
	}//close
	else if(choice == 3){
		title_banner("SORTING BY GPA");
		//sorting according to gpa
		for(i=0;i<n;i++)
			for(j=i+1;j<n;j++){
				if(s[i].gpa < s[j].gpa){
					temp = s[i];
					s[i] = s[j];
					s[j] = temp;
				}	
			}	//j forloop
	}
	else if(choice == 4){
		title_banner("SORTING BY SERIALNO.");
		//sorting according to alphabetical order and giving serial number
		for(i=0;i<n;i++)
			for(j=i+1;j<n;j++){
				if(strcmp(s[i].name,s[j].name)>0){
					temp = s[i];
					s[i] = s[j];
					s[j] = temp;
				}	
			}	//j forloop
			green();
			printf("\n\n\tSN.\t%-30sTOTAL\t\tPERCENT\t\tGPA\tGRADE\n","NAME");
			printf("\t-------------------------------------------------------------------------------------\n");reset();
			for(i=0;i<n;i++){
				printf("\t%02d\t%-30s%06.2f\t\t%5.2f %%\t\t%.2f\t  %s\n",i+1,s[i].name,s[i].total,s[i].percent,s[i].gpa,s[i].grade);
			}
		yellow();
		printf("\n\t");
		system("pause");
		menu();
	}
	else
		menu();
		
	//displaying the sorted record
	green();
	printf("\n\n\tROLL\t%-30sTOTAL\t\tPERCENT\t\tGPA\tGRADE\n","NAME");
	printf("\t-------------------------------------------------------------------------------------\n");reset();
	for(i=0;i<n;i++){
		printf("\t%02d\t%-30s%06.2f\t\t%5.2f %%\t\t%.2f\t  %s\n",s[i].roll,s[i].name,s[i].total,s[i].percent,s[i].gpa,s[i].grade);
	}
		
	printf("\n\n\t");
	yellow();
	system("pause");
}	//end of sort_records()

//to make user input details, just declaring a seperate func , maybe used multiple times so
void input_details(){
	std.total = 0;
	std.percent = 0;

	rollno_again:
		fflush(stdin);
		printf("\n\tEnter RollNo: ");
		scanf("%d",&std.roll);
		//checking if rollno is a natural number
		if(std.roll <= 0){
			red();
			printf("\t(Invalid Rollnumber!!)");
			reset();
			goto rollno_again;
		}
		//checking if rollno already exists in the file
		if((rollChecker(std.roll)) == 1){
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
		if(nameChecker(std.name) == 1){
			red();
			printf("\t(Invalid Name!! Please use Alphabets Only.)\n");
			reset();
			goto name_again;
		}
		else if(nameChecker(std.name) == 2){
			red();
			printf("\t(This Name already exists!!)\n");
			reset();
			goto name_again;
		}
	
	for(j=0;j<subs;j++)
	{
		again_marks:
			fflush(stdin);
			printf("\tEnter marks of %s: ",subject[j]);
			scanf("%f",&std.marks[j]);
			//checking for invalid input
			if(std.marks[j] < 0 || std.marks[j] > 100){
				red();
				printf("\t(Marks cannot be lesser than 0 or greater than 100. Please enter again CAREFULLY!)\n");
				reset();
				goto again_marks;
			}
			std.total += std.marks[j];
	}
	std.percent= std.total/subs;
	std.gpa = std.percent/25;
	strcpy(std.grade,grade_calc(std.percent));
}	//end of input_details()

//about me and the program
void about(){
	title_banner("ABOUT");
	
	char ch;
	//Opening the about file cause everything is written in that file
	FILE *aboutfp = fopen("about.txt","rb");
	if(aboutfp == NULL){
		red();
		printf("\n\tSorry! The file doesn't exists!!\n");
		reset();
	}

	//the texts are written directly in the file
	green();
	while((ch=fgetc(aboutfp)) != EOF)
	{
		printf("%c",ch);
	}
	fclose(aboutfp);
	printf("\n\n\t");
	yellow();
	system("pause");
}
//acts as a title bar and to reduce redundancy ,'cause i gave headings to all the defined-funcs
void title_banner(char *title){
	//for design purpose only
		system("cls");
		cyan();
		for(i=0;i<120;i++)
			printf("%c",247);		//178,219,176,206 looks good
		red();
		printf("\n\t\t\t\t\t\t%s\n",title);
		cyan();
		for(i=0;i<120;i++)
			printf("%c",247);	
		reset();
		printf("\n");//end of design
}	//end of titlebanner()

//grade calculator to find the equivalent grade (like 90% above = A+)
char *grade_calc(float percent){
	if(percent >= 90)
		return "A+";
	else if(percent >= 80 && percent < 90)
		return "A";
	else if(percent >= 70 && percent < 80)
		return "B+";
	else if(percent >= 60 && percent < 70)
		return "B";
	else if(percent >= 50 && percent < 60)
		return "C+";
	else if(percent >= 40 && percent < 50)
		return "C";
	else if(percent >= 20 && percent < 40)
		return "D";
	else
		return "E";
}	//end of grade calc()

// to know the number of records in the file and also returning it sothat it can be used in other funcs to display "NO record found" instead of leaving it blank
int no_of_records(){
	int n;
	fp = fopen("record.txt","rb");
	if(fp == NULL){
		red();
		printf("\n\tFile doesn't exists!! Please ADD RECORD first.\n\n\t");
		yellow();
		system("pause");
		menu();
	}
	
	fseek(fp,0,SEEK_END); //moving cursor to last position for total length/size 
	n = ftell(fp)/sizeof(std);	// since ftell gives the size of file and std gives size of each record. so the division gives no of records
	
	fclose(fp);
	return n;	//returning no of records
}	// end of noofrecords()

//just to verify if the name contains Alphabets only, coz C reads all the characters as CHARACTERS
int nameChecker(char *name){
	int flag=0;
	FILE *namefp = fopen("record.txt","r");
	struct student temp;
	
	//also checking if the name exists in the file
	while(fread(&temp,sizeof(struct student),1,namefp)){
		if(strcmp(temp.name,name)==0){
			fclose(namefp);
			return 2;
		}
	}
	for(i=0;name[i]!='\0';i++){
		if((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z'))	//sorry elon musk and sons
			flag = 0;
		else
			flag = 1;
	}

	return flag;
}	//end of nameChecker()

//to check if the rollno already exists in the main file
int rollChecker(int roll){
	FILE *rollfp = fopen("record.txt","r");
	struct student temp;
	
	while(fread(&temp,sizeof(struct student),1,rollfp)){
		if(roll == temp.roll){
			fclose(rollfp);
			return 1;
		}
	}
}	//end of rollChecker()

//login page
void login(){	
	title_banner("LOGIN PAGE");
	
	char username[] = "admin" , password[] = "password";
	char userinput[10], pass[10];
		
	fflush(stdin);
	printf("\n\n\tEnter Username: ");
	gets(userinput);
	fflush(stdin);
	printf("\tEnter Password: ");
	gets(pass);
	
	if(strcmp(username,userinput)==0 && strcmp(password,pass)==0){
		menu();
	}
	else{
		red();
		printf("\n\tInvalid Username or password!! (Hint: admin & password)\n\t");
		printf("-----------------------------\n\t");
		yellow();
		system("pause");
		login();
	}	
}

void sort_file(){
	title_banner("SORTING RECORDS INSIDE THE FILE");
	int n = no_of_records();	//getting no of records
	int choice;
	fp = fopen("record.txt","rb");
	if(fp == NULL || n == 0){
		red();
		printf("\n\tFile doesn't exists!! Please ADD RECORD first.\n\n\t");
		yellow();
		system("pause");
		menu();
	}
	struct student *s,temp;
	
	//allocating memory for struct s and storing file records in s[] as array
	s = (struct student*) calloc(n,sizeof(struct student));
	for(i=0;i<n;i++){
		fread(&s[i],sizeof(struct student),1,fp);
	}
	fclose(fp);
	//sort menu to choose either to arrange acc. to rollno, name or gpa
	printf("\n\t1.Sort by Name\n\t2.Sort by Rollno\n\t3.Sort by GPA\n\t0.Back to Menu\n");
	yellow();
	printf("\n\tEnter a choice:\n\t>> ");
	scanf("%d",&choice);
	reset();
	
	if(choice == 1){
		//sorting according to alphabetical order
		for(i=0;i<n;i++)
			for(j=i+1;j<n;j++){
				if(strcmp(s[i].name,s[j].name)>0){
					temp = s[i];
					s[i] = s[j];
					s[j] = temp;
				}	
			}	//j forloop
	}//close
	else if(choice == 2){
		//sorting according to rollno of students
		for(i=0;i<n;i++)
			for(j=i+1;j<n;j++){
				if(s[i].roll > s[j].roll){
					temp = s[i];
					s[i] = s[j];
					s[j] = temp;
				}	
			}	//j forloop
	}//close
	else if(choice == 3){
		//sorting according to gpa
		for(i=0;i<n;i++)
			for(j=i+1;j<n;j++){
				if(s[i].gpa < s[j].gpa){
					temp = s[i];
					s[i] = s[j];
					s[j] = temp;
				}	
			}	//j forloop
	}
	else
		menu();
		
	fp = fopen("record.txt","wb");
	for(i=0;i<n;i++){
		fwrite(&s[i],sizeof(struct student),1,fp);
	}
	fclose(fp);
	green();
	printf("\n\tFILE SUCCESSFULLY REARRANGED!\n\n\t");
	yellow();
	system("pause");
}

void misc(){
	int choice;
	char yesno;
	do{
		title_banner("MISCELLANEOUS");
		printf("\n\t1.DELETE ALL RECORDS AT ONCE\n\t2.ARRANGE RECORDS (IN FILE)\n\t0.Back to Menu\n");
		
		yellow();
		printf("\n\tEnter a choice:\n\t>> ");
		scanf("%d",&choice);
		reset();
		
		switch(choice){
			case 1:
				red();
				printf("\n\tDo you really want to DELETE ALL THE RECORDS?(y/n)\n\t>> ");
				scanf(" %c",&yesno);
				
				if(yesno == 'y' || yesno == 'Y'){
					remove("record.txt");
					green();
					printf("\n\tRecord Deleted Successfully!\n\n\t");
				}
				else{
					green();
					printf("\n\tNot Doing Anything to your precious DATA.\n");
				}
				printf("\n\t");	
				yellow();
				system("pause");
				break;
				
			case 2:
				sort_file();
				break;
		}//switch
	}while(choice!=0);
}
