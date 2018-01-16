#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct contact{
	unsigned long phone_number;
	long first_name_posn;
	long last_name_posn;
	long company_name_posn;
	long email_posn;
	long next;
};
struct data{

	char first_name[100];
	char last_name[100];
	char company_name[100];
	char email1[100];
};

int alpha_cmp(const void *a, const void *b)
{
    return strcmp(a, b);
}

FILE* myOpenforReading ( char fileName[] );
int contCount (FILE *fptr);
int *nextVals (FILE *fptr, int *arrayNext, int numConts);
char **orderCont (FILE *fptr, int numConts, char **arrayCont);
void createList (FILE *fptr, int numConts, char **arrayCont, char **contList);
int interface (FILE *fptr, int *arrayNext, char **arrayCont, int numConts);
int addCont ( FILE *fptr, struct contact *info, int location);
int retrieveCont ( FILE *fptr);
char *numCheck ( char phoneNum[]);
void removeTrailingHardReturn ( char line[] );

FILE* myOpenforReading (char fileName[]) {
	FILE* fileptr;
	fileptr = fopen(fileName, "a+");
	if (fileptr == NULL) {
      	fileptr = fopen(fileName, "w+");
   }
   return fileptr;
}

int main ( int argc, char *argv[] ) {

	FILE *fp = NULL;
	int check1 = 1;
	int check2 = 0;
	int flag1 = 0;
	int i = 0;
	int numConts = 0;
	int location = 0;
	int *arrayNext = NULL;
	char **arrayCont = NULL;
	char **contList = NULL;
	struct contact *data;
	data = malloc(sizeof(struct contact));
	fp = myOpenforReading( argv[1] );

		numConts = contCount(fp);
		arrayNext = malloc (sizeof(int)*numConts);
		arrayCont = malloc(sizeof(char * ) * numConts);
		contList = malloc(sizeof(char * ) * numConts);
		for (i = 0; i < numConts; ++i) {
			arrayCont[i] = malloc(sizeof(char)*100);
			contList[i] = malloc(sizeof(char)*100);
		}

		if(numConts != 0)
			orderCont (fp, numConts, arrayCont);

		nextVals (fp, arrayNext, numConts);
		createList (fp, numConts, arrayCont, contList);
		interface (fp, arrayNext, arrayCont, numConts);


		while (flag1 == 0) {
			fseek(fp,0,SEEK_END);
			location = ftell(fp);
			check1 = addCont (fp , data, location);
			if (check1 == 1) {
				check2 = retrieveCont (fp);
			}
			if (check2 == 1)
				break;
		}

	return 0;
}

int contCount (FILE *fptr) {

	long location = 0;
	int k = 0;
	int i = 0;
	long nextVal = 0;

		fseek(fptr,0,SEEK_END);
		location = ftell(fptr);

		while ( k < location && location != 0 ) {
			fseek(fptr,0,SEEK_SET);
			fseek(fptr,k,SEEK_SET);
			fseek(fptr,sizeof(unsigned long),SEEK_CUR);
			fseek(fptr, sizeof(long) * 4,SEEK_CUR);
			fread(&nextVal, sizeof(long),1,fptr);
			k = nextVal;
			++i;
		}

	return i;
}

int *nextVals (FILE *fptr, int *arrayNext, int numConts) {

	int k = 0;
	int i = 1;
	long nextVal = 0;

		arrayNext[0] = 0;

		while ( i < numConts + 1 ) {
			fseek(fptr,0,SEEK_SET);
			fseek(fptr,k,SEEK_SET);
			fseek(fptr,sizeof(unsigned long),SEEK_CUR);
			fseek(fptr, sizeof(long) * 4,SEEK_CUR);
			fread(&nextVal, sizeof(long),1,fptr);
			k = nextVal;
			arrayNext[i] = k;
			++i;
		}

	return arrayNext;

}

char **orderCont (FILE *fptr, int numConts, char **arrayCont) {

	int i = 0;
	int k = 0;
	int j = 0;
	long posn = 0;
	char tempCont[numConts][100];

	while (1) {
		fseek(fptr,k,SEEK_SET);
		fseek(fptr, sizeof(unsigned long),SEEK_CUR);
		fseek(fptr, sizeof(long),SEEK_CUR);
		fread(&posn,sizeof(long),1,fptr);
		if (posn == 0) {
			printf("TAKEN\n");
			fread(&posn,sizeof(long),1,fptr);
			fseek(fptr, sizeof(long),SEEK_CUR);
			fread(&k,sizeof(long),1,fptr);
			fseek(fptr,posn,SEEK_SET);
			fread(arrayCont[j],sizeof(char),100,fptr);
			strcpy(tempCont[j],arrayCont[i]);
			++i;
		} else {
			fseek(fptr, sizeof(long)*2,SEEK_CUR);
			fread(&k,sizeof(long),1,fptr);
			fseek(fptr,posn,SEEK_SET);
			fread(arrayCont[j],sizeof(char),100,fptr);
			strcpy(tempCont[j], arrayCont[i]);
			++i;
		}
		++j;
		if (j == numConts)
			break;
	}

	for (j = 0; j < numConts; ++j) {
		strcpy(tempCont[j],arrayCont[j]);
	}
	qsort(tempCont, numConts, sizeof tempCont[0], alpha_cmp);

	for (j = 0; j < numConts; ++j) {
		strcpy(arrayCont[j],tempCont[j]);
	}

	return arrayCont;
}

void createList (FILE *fptr, int numConts, char **arrayCont, char **contList) {

	int i = 0;

	for (i = 0; i < numConts; ++i){
		strcat(arrayCont[i], " ");
		printf("%s.\n", arrayCont[i]);
	}



}













int interface (FILE *fptr, int *arrayNext, char **arrayCont, int numConts) {


	printf("Number of Contacts = %d\n", numConts);


	return 0;
}



int addCont (FILE *fptr, struct contact *info, int location) {

	int flag2 = 0;
	char yesNo[10];
	char fName[100];
	char lName[100];
	char cName[100];
	char pNum[100];
	char email[100];
	unsigned long pNumber;

		info->next = location;

		while (1) {

		printf("Do you wish to enter a new contact (Yes or No)?: ");
		fgets( yesNo, 11, stdin );
		removeTrailingHardReturn(yesNo);

		if ( strcmp ( yesNo, "Yes" ) == 0 ) {


			printf("First Name: ");
			fgets (fName, 101, stdin);
			removeTrailingHardReturn(fName);
			info->first_name_posn = sizeof(struct contact) + info->next;

			printf("Last Name: ");
			fgets (lName, 101, stdin);
			removeTrailingHardReturn(lName);
			info->last_name_posn = info->first_name_posn + strlen(fName) + 1;
			if (strcmp(fName,"\0") == 0) {
				info->last_name_posn = info->first_name_posn;
				info->first_name_posn = 0;
			}

			printf("Company Name: ");
			fgets (cName, 101, stdin);
			removeTrailingHardReturn(cName);
			info->company_name_posn = info->last_name_posn + strlen(lName) + 1;
			if (strcmp(lName,"\0") == 0) {
				info->company_name_posn = info->last_name_posn;
				info->last_name_posn = 0;
			}

			printf("Phone Number (enter only numbers): ");
			fgets (pNum, 101, stdin);
			removeTrailingHardReturn(pNum);
			numCheck(pNum);
			pNumber = atoi (pNum);
			info->phone_number = pNumber;

			printf("Email: ");
			fgets (email, 101, stdin);
			removeTrailingHardReturn(email);
			info->email_posn = info->company_name_posn + strlen(cName) + 1;
			if (strcmp(email,"\0") == 0) {
				while (strcmp(email,"\0") == 0) {
					printf("Email: ");
					fgets(email, 100, stdin);
					removeTrailingHardReturn(email);
				}
			}
			if (strcmp(cName,"\0") == 0) {
				info->email_posn = info->company_name_posn;
				info->company_name_posn = 0;
			}

			info->next = info->email_posn + strlen(email) + 1;


			fwrite(info,sizeof(struct contact),1,fptr);
			if (info->first_name_posn != 0) {
				fwrite(fName, sizeof(char),strlen(fName)+1,fptr);
			}
			if (info->last_name_posn != 0) {
				fwrite(lName, sizeof(char),strlen(lName)+1,fptr);
			}
			if (info->company_name_posn != 0) {
				fwrite(cName, sizeof(char),strlen(cName)+1,fptr);
			}
			fwrite(email, sizeof(char),strlen(email)+1,fptr);
		}
		else if (strcmp ( yesNo, "No" ) == 0) {
			flag2 = 1;
			break;
		} else {
			flag2 = 2;
		}

	}


	return flag2;
}

int retrieveCont ( FILE *fptr ) {

	char yesNo[10];
	int num1 = 0;
	int num2 = 0;
	char phoneSearch[100];
	unsigned long findNum;
	long k = 0;
	int found = 0;
	unsigned long phoneN;
	long nextVal;
	struct data names;
	long fName;
	long lName;
	long cName;
	long email;
	long next;
	int counter = 0;

	printf("Do you wish to retrieve a contact (Yes or No)?: ");
	fgets( yesNo, 11, stdin );
	removeTrailingHardReturn(yesNo);

	if ( strcmp ( yesNo, "Yes" ) == 0 ) {
		printf("Phone Number: ");
		fgets(phoneSearch,100,stdin);
		removeTrailingHardReturn(phoneSearch);
		findNum = atoi (phoneSearch);

		fseek(fptr,0,SEEK_END);
		num2 = ftell(fptr);

		while ( counter <= num2 && num2 != 0 ) {
			fseek(fptr,0,SEEK_SET);
			fseek(fptr,k,SEEK_SET);
			fread(&phoneN, sizeof(unsigned long),1,fptr);
			if (findNum == phoneN) {
				found = 1;
				break;
			}
			fseek(fptr, sizeof(long) * 4,SEEK_CUR);
			fread(&nextVal, sizeof(long),1,fptr);
			k = nextVal;
			counter = nextVal;
		}
	}

	else if (strcmp ( yesNo, "No" ) == 0) {
		num1 = 1;
	} else {
		num1 = 2;
	}

		if (found == 1) {

				fread(&fName, sizeof(long),1, fptr);
				fread(&lName, sizeof(long),1, fptr);
				fread(&cName, sizeof(long),1, fptr);
				fread(&email, sizeof(long), 1, fptr);
				fread(&next, sizeof(long), 1, fptr);
			/* Possible cases */
			if ( fName != 0 && lName != 0 && cName != 0 ) {

				fread(names.first_name,lName - fName,1,fptr);
				printf("First Name: %s\n", names.first_name);
				fread(names.last_name,cName - lName, 1, fptr);
				printf("Last Name: %s\n", names.last_name);
				fread(names.company_name, email - cName, 1, fptr);
				printf("Company Name: %s\n", names.company_name);
				printf("Phone Number (enter only numbers): %ld\n", findNum);
				fread(names.email1, next - email, 1, fptr);
				printf("Email: %s\n", names.email1);
			}
			else if (fName == 0 && lName != 0 && cName != 0) {

				printf("First Name: \n");
				fread(names.last_name,cName - lName, 1, fptr);
				printf("Last Name: %s\n", names.last_name);
				fread(names.company_name, email - cName, 1, fptr);
				printf("Company Name: %s\n", names.company_name);
				printf("Phone Number (enter only numbers): %ld\n", findNum);
				fread(names.email1, next - email, 1, fptr);
				printf("Email: %s\n", names.email1);

			}
			else if (fName == 0 && lName == 0 && cName != 0) {

				printf("First Name: \n");
				printf("Last Name: \n");
				fread(names.company_name, email - cName, 1, fptr);
				printf("Company Name: %s\n", names.company_name);
				printf("Phone Number (enter only numbers): %ld\n", findNum);
				fread(names.email1, next - email, 1, fptr);
				printf("Email: %s\n", names.email1);

			}
			else if (fName == 0 && lName == 0 && cName == 0) {

				printf("First Name: \n");
				printf("Last Name: \n");
				printf("Company Name: \n");
				printf("Phone Number (enter only numbers): %ld\n", findNum);
				fread(names.email1, next - email, 1, fptr);
				printf("Email: %s\n", names.email1);

			}
			else if (fName != 0 && lName != 0 && cName == 0) {

				fread(names.first_name,lName - fName,1,fptr);
				printf("First Name: %s\n", names.first_name);
				fread(names.last_name,email - fName, 1, fptr);
				printf("Last Name: %s\n", names.last_name);
				printf("Company Name: \n");
				printf("Phone Number (enter only numbers): %ld\n", findNum);
				fseek(fptr,email,SEEK_SET);
				fread(names.email1, next - email, 1, fptr);
				printf("Email: %s\n", names.email1);
			}
			else if (fName == 0 && lName != 0 && cName == 0) {

				printf("First Name: \n");
				fread(names.last_name,cName - lName, 1, fptr);
				printf("Last Name: %s\n", names.last_name);
				fread(names.company_name, email - cName, 1, fptr);
				printf("Company Name: \n");
				printf("Phone Number (enter only numbers): %ld\n", findNum);
				fseek(fptr,email,SEEK_SET);
				fread(names.email1, next - email, 1, fptr);
				printf("Email: %s\n", names.email1);

			}
			else if (fName != 0 && lName == 0 && cName == 0) {

				fread(names.first_name,lName - fName,1,fptr);
				printf("First Name: %s\n", names.first_name);
				printf("Last Name: \n");
				printf("Company Name: \n");
				printf("Phone Number (enter only numbers): %ld\n", findNum);
				fseek(fptr,email,SEEK_SET);
				fread(names.email1, next - email, 1, fptr);
				printf("Email: %s\n", names.email1);
			}

		}
		else if (found == 2) {
			printf("No match found.\n");
		}

	return num1;
}

char *numCheck ( char phoneNum[]) {

	int k;
	int temp;
	char tempChar;
	char newPhoneNum[100];

	for (k = 0; k < strlen (phoneNum); ++k) {
		tempChar = phoneNum[k];

		if (phoneNum[k] == '0') {
			temp = 1;
		} else {
			temp = atoi (&tempChar);
		}

		if (temp == 0) {
			printf ("Phone Number (enter only numbers): ");
			fgets (newPhoneNum, 100, stdin);
			removeTrailingHardReturn (newPhoneNum);
			numCheck (newPhoneNum);
			strcpy (phoneNum, newPhoneNum);
		}
	}
	if (strcmp(phoneNum,"\0") == 0) {
			printf ("Phone Number (enter only numbers): ");
			fgets (newPhoneNum, 100, stdin);
			removeTrailingHardReturn (newPhoneNum);
			numCheck (newPhoneNum);
			strcpy (phoneNum, newPhoneNum);
	}

	return phoneNum;
}


void removeTrailingHardReturn (char line[]) {

   if (line[strlen(line)-1] == '\n')
   {
      line[strlen(line)-1] = '\0';
   }

}
