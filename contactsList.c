#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "struct.h"
#define CHOICE_LEN 10
#define MAX_LEN 100

void removeTrailing(char* buffer);
char* checkPhone_num(char* phoneBuf);
char* checkEmail(char* emailBuf);

int main ()
{
   FILE *infile = NULL;
   struct contact data;
   struct contact readData;
   char userChoice1[CHOICE_LEN];
   char userChoice2[CHOICE_LEN];
   unsigned long compNum;
   char bufRetNum[MAX_LEN];
   char bufFirst[MAX_LEN];
   char bufLast[MAX_LEN];
   char bufCompany[MAX_LEN];
   char bufNum[MAX_LEN];
   char bufEmail[MAX_LEN];
   bool state = true;
   char *endptr;
   int end = 0;
   int curr = 0;
   int count = 0;

/*This opens the file to read and write , if file doesn't exist then create a new one called myContactsList.db   */
   infile = fopen("myContactsList.db", "a+");
   if (infile == NULL)
   {
      infile = fopen("myContactsList.db", "w+");
   }

   while (state == true) {
   printf("Do you wish to enter a new contact(Yes or No)?: ");
   scanf("%s", userChoice1);
   getchar();

   if (strcmp(userChoice1, "Yes") == 0)
   {
      /*This part asks all the info required to the user */
      printf("First Name: ");
      fgets(bufFirst, MAX_LEN, stdin);
      removeTrailing(bufFirst);

      printf("Last Name: ");
      fgets(bufLast, MAX_LEN, stdin);
      removeTrailing(bufLast);
      printf("%s premier fois", bufLast);

      printf("Company Name: ");
      fgets(bufCompany, MAX_LEN, stdin);
      if (bufLast[0] == '\0')
      {
         while (bufCompany[0] == '\n')
         {
            printf("Company Name: ");
            fgets(bufCompany, MAX_LEN, stdin);
         }
      }
      removeTrailing(bufCompany);

      printf("Phone Number(enter only numbers): ");
      fgets(bufNum, MAX_LEN, stdin);
         /*if the first char is enter, it will ask the user to input their number again*/
        /* while (bufNum[0] == '\n')
         {
            printf("Phone Number(enter only numbers): ");
            fgets(bufNum, MAX_LEN, stdin);
         }
         if (bufNum[0] != '\n')
         {*/
            checkPhone_num(bufNum);
        /*}*/
         removeTrailing(bufNum);

      printf("Email: ");
      fgets(bufEmail, MAX_LEN, stdin);
         while (bufEmail[0] == '\n')
         {
            printf("Email: ");
            fgets(bufEmail, MAX_LEN, stdin);
         }
         checkEmail(bufEmail);
         removeTrailing(bufEmail);

   state = true;
   /*Stroul converts the string to an unsigned long and store it into the struct */
   data.phone_number = strtoul(bufNum, &endptr, 10);

   /*This part puts all the location of the info into the struct */
   if (bufFirst[0] != '\0')
   {
      data.first_name_posn = count + sizeof(struct contact);
      if (bufLast[0] != '\0')
      {
         data.last_name_posn = data.first_name_posn + strlen(bufFirst) + 1;
         if (bufCompany[0] != '\0')
         {
            data.company_name_posn = data.last_name_posn + strlen(bufLast) + 1;
            data.email_posn = data.company_name_posn + strlen(bufCompany) + 1;
            data.next = data.email_posn + strlen(bufEmail) + 1;
         }
	 else if (bufCompany[0] == '\0')
	 {
	   data.company_name_posn = 0;
	   data.email_posn = data.last_name_posn + strlen(bufLast) + 1;
	   data.next = data.email_posn + strlen(bufEmail) + 1;
	 }
      }
      else if (bufLast[0] == '\0')
      {
	data.last_name_posn = 0;
	if (bufCompany[0] != '\0')
         {
            data.company_name_posn = data.first_name_posn + strlen(bufFirst) + 1;
            data.email_posn = data.company_name_posn + strlen(bufCompany) + 1;
            data.next = data.email_posn + strlen(bufEmail) + 1;
         }
	 else if (bufCompany[0] == '\0')
	 {
	   data.company_name_posn = 0;
	   data.email_posn = data.first_name_posn + strlen(bufFirst) + 1;
	   data.next = data.email_posn + strlen(bufEmail) + 1;
	 }
      }
   }
   else if (bufFirst[0] == '\0')
   {
     data.first_name_posn = 0;
     if (bufLast[0] != '\0')
      {
	data.last_name_posn = count + sizeof(struct contact);
         if (bufCompany[0] != '\0')
         {
            data.company_name_posn = data.last_name_posn + strlen(bufLast) + 1;
            data.email_posn = data.company_name_posn + strlen(bufCompany) + 1;
            data.next = data.email_posn + strlen(bufEmail) + 1;
         }
	 else if (bufCompany[0] == '\0')
	 {
	   data.company_name_posn = 0;
	   data.email_posn = data.last_name_posn + strlen(bufLast) + 1;
	   data.next = data.email_posn + strlen(bufEmail) + 1;
	 }
      }
      else if (bufLast[0] == '\0')
      {
	data.last_name_posn = 0;
	if (bufCompany[0] != '\0')
         {
	    data.company_name_posn = count + sizeof(struct contact);
            data.email_posn = data.company_name_posn + strlen(bufCompany) + 1;
            data.next = data.email_posn + strlen(bufEmail) + 1;
         }
	 else if (bufCompany[0] == '\0')
	 {
	   data.company_name_posn = 0;
	   data.email_posn = count + sizeof(struct contact);
	   data.next = data.email_posn + strlen(bufEmail) + 1;
	 }
      }
   }
   count = data.next;

   /*This part write the info in the file and store them*/
   fwrite(&data, sizeof(struct contact), 1, infile);

   if (bufFirst[0] != '\0') {
    fwrite(bufFirst, 1, strlen(bufFirst)+1, infile);
   }

   if (bufLast[0] != '\0') {
    fwrite(bufLast, 1, strlen(bufLast)+1, infile);
   }

   if (bufCompany[0] != '\0') {
    fwrite(bufCompany, 1, strlen(bufCompany)+1, infile);
   }

    fwrite(bufEmail, 1, strlen(bufEmail)+1, infile);

    fseek(infile, 0, SEEK_END);

   }
   /*This is the part where the user doesnt want to add a contact but to retrieve data*/
   else if (strcmp(userChoice1, "No") == 0)
   {
      printf("Do you wish to retrieve a contact(Yes or No)?: ");
      scanf("%s", userChoice2);
      getchar();

      if (strcmp(userChoice2, "Yes") == 0)
      {
         printf("Phone Number: ");
         fgets(bufRetNum, MAX_LEN, stdin);
         removeTrailing(bufRetNum);
         compNum = strtoul(bufRetNum, &endptr, 10);

         fseek(infile, 0, SEEK_END);
         end = ftell(infile);
         fseek(infile, 0, SEEK_SET);

         while(curr != end)
         {
            fread(&readData, sizeof(struct contact), 1, infile);

            if (readData.phone_number != compNum)
            {
               fseek(infile, readData.next, SEEK_SET);
               curr = readData.next;
               if (curr == end)
               {
                  printf("No match found.\n");
               }
            }

            else if (readData.phone_number == compNum)
            {
               fseek(infile, 0, SEEK_CUR);
               curr = ftell(infile);

               if (readData.first_name_posn != 0)
               {
                  curr = readData.last_name_posn - curr;
                  fread(bufFirst, sizeof(char), curr, infile);
                  printf("First Name: %s\n", bufFirst);
                  if (readData.company_name_posn != 0)
                  {
                      curr = ftell(infile);
                      curr = readData.company_name_posn - curr;
                      fread(bufLast, sizeof(char), curr, infile);
                      printf("Last Name: %s\n", bufLast);
                      curr = ftell(infile);
                      curr = readData.email_posn - curr;
                      fread(bufCompany, sizeof(char), curr, infile);
                      printf("Company Name: %s\n", bufCompany);
                      curr = ftell(infile);
                      curr = readData.next - curr;
                      fread(bufEmail, sizeof(char), curr, infile);
                      printf("Phone Number(enter only numbers): %lu\n", readData.phone_number);
                      printf("Email: %s\n", bufEmail);
                      curr = end;
                  }
                  else if (readData.company_name_posn == 0)
                  {
                      curr = ftell(infile);
                      curr = readData.email_posn - curr;
                      fread(bufLast, sizeof(char), curr, infile);
                      printf("Last Name: %s\n", bufLast);
                      printf("Company Name: \n");
                      curr = ftell(infile);
                      curr = readData.next - curr;
                      fread(bufEmail, sizeof(char), curr, infile);
                      printf("Phone Number(enter only numbers): %lu\n", readData.phone_number);
                      printf("Email: %s\n", bufEmail);
                      curr = end;
                  }
               }
               else if (readData.last_name_posn == 0)
               {
                  if (readData.company_name_posn != 0)
                  {
                    curr = readData.company_name_posn - curr;
                    fread(bufFirst, sizeof(char), curr, infile);
                    printf("First Name: %s\n", bufFirst);
                    printf("Last Name: \n");
                    curr = ftell(infile);
                    curr = readData.email_posn - curr;
                    fread(bufCompany, sizeof(char), curr, infile);
                    printf("Company Name: %s\n", bufCompany);
                    curr = ftell(infile);
                    curr = readData.next - curr;
                    fread(bufEmail, sizeof(char), curr, infile);
                    printf("Phone Number(enter only numbers): %lu\n", readData.phone_number);
                    printf("Email: %s\n", bufEmail);
                    curr = end;
                  }
                  else if (readData.company_name_posn == 0)
                  {
                    curr = readData.email_posn - curr;
                    fread(bufFirst, sizeof(char), curr, infile);
                    printf("First Name: %s\n", bufFirst);
                    printf("Last Name: \n");
                    printf("Company Name: \n");
                    curr= ftell(infile);
                    curr = readData.next - curr;
                    fread(bufEmail, sizeof(char), curr, infile);
                    printf("Phone Number(enter only numbers): %lu\n", readData.phone_number);
                    printf("Email: %s\n", bufEmail);
                    curr = end;
                  }


               }
            }
            else if (readData.first_name_posn == 0)
            {
              printf("First Name: \n");
              if (readData.last_name_posn != 0)
              {
                if (readData.company_name_posn != 0)
                {
                  curr = readData.company_name_posn - curr;
                  fread(bufLast, sizeof(char), curr, infile);
                  printf("Last Name: %s\n", bufLast);
                  curr = ftell(infile);
                  curr = readData.email_posn - curr;
                  fread(bufCompany, sizeof(char), curr, infile);
                  printf("Company Name: %s\n", bufCompany);
                  curr = ftell(infile);
                  curr = readData.next - curr;
                  fread(bufEmail, sizeof(char), curr, infile);
                  printf("Phone Number(enter only numbers): %lu\n", readData.phone_number);
                  printf("Email: %s\n", bufEmail);
                  curr = end;
                }
                else if (readData.company_name_posn == 0)
                {
                   curr = ftell(infile);
                   curr = readData.email_posn - curr;
                   fread(bufLast, sizeof(char), curr, infile);
                   printf("Last Name: %s\n", bufLast);
                   printf("Company Name: \n");
                   curr = ftell(infile);
                   curr = readData.next - curr;
                   fread(bufEmail, sizeof(char), curr, infile);
                   printf("Phone Number(enter only numbers): %lu\n", readData.phone_number);
                   printf("Email: %s\n", bufEmail);
                   curr = end;
                }
              }
              else if (readData.last_name_posn == 0)
              {
                if (readData.company_name_posn != 0)
                {
                   printf("Last Name: \n");
                   curr = ftell(infile);
                   curr = readData.email_posn - curr;
                   fread(bufCompany, sizeof(char), curr, infile);
                   printf("Company Name: %s\n", bufCompany);
                   curr = ftell(infile);
                   curr = readData.next - curr;
                   fread(bufEmail, sizeof(char), curr, infile);
                   printf("Phone Number(enter only numbers): %lu\n", readData.phone_number);
                   printf("Email: %s\n", bufEmail);
                   curr = end;
                }
                else if (readData.company_name_posn == 0)
                {
                   printf("Last Name: \n");
                   printf("Company Name: \n");
                   curr= ftell(infile);
                   curr = readData.next - curr;
                   fread(bufEmail, sizeof(char), curr, infile);
                   printf("Phone Number(enter only numbers): %lu\n", readData.phone_number);
                   printf("Email: %s\n", bufEmail);
                   curr = end;
                }
              }
         }
      }
     }
      else if (strcmp(userChoice2, "No") == 0)
      {
         exit(0);
      }

      else
      {
         printf("Please give a correct answer!\n");
      }
   }
   else
   {
       printf("Please give a correct answer!\n");
   }
 }
 fclose(infile);
 return 0;
}

void removeTrailing(char* buffer)
{
   if (buffer[strlen(buffer) - 1] == '\n');
   {
      buffer[strlen(buffer) - 1] = '\0';
   }
}

char* checkPhone_num(char* phoneBuf)
{
   int i = 0;
   bool condition = true;

   while (phoneBuf[0] == '\n')
    {
       printf("Phone Number(enter only numbers): ");
       fgets(phoneBuf, MAX_LEN, stdin);
    }

   for (i = 0; i < strlen(phoneBuf); i++)
   {
      if (isalpha(phoneBuf[i]))
      {
        condition = true;
        break;
      }
      else
      {
         condition = false;
      }
   }
   if (condition == true)
   {
      printf("Phone Number(enter only numbers): ");
      fgets(phoneBuf, MAX_LEN, stdin);
      checkPhone_num(phoneBuf);
   }
   else
   {
      if (strlen(phoneBuf) == 8 || strlen(phoneBuf) == 11)
      {
         return phoneBuf;
      }
      else
      {
        printf("Phone Number(enter only numbers): ");
        fgets(phoneBuf, MAX_LEN, stdin);
        checkPhone_num(phoneBuf);
      }
   }
   return phoneBuf;
}

char* checkEmail(char* emailBuf)
{
   char* token;
   int i = 0;
   int len = 0;
   char tempBuf[100];

   strcpy(tempBuf,emailBuf);
   if (strstr(tempBuf, "@") == NULL || strstr(tempBuf, ".") == NULL)
   {
     printf("Email: ");
     fgets(emailBuf, MAX_LEN, stdin);
     checkEmail(emailBuf);
   }
   else
   {
     token = strtok(tempBuf, "@");

     while( token != NULL)
     {
        token = strtok(NULL, ".");
        i++;
        len = strlen(token);
        printf("%s last token and its length is %d\n", token, len);

        if (i == 2)
        {
          if (strlen(token) != 4)
          {
             printf("Email: ");
             fgets(emailBuf, MAX_LEN, stdin);
             checkEmail(emailBuf);
          }
          else
          {
             printf("%s email KIN ARRIVE LI", emailBuf);
             return emailBuf;
          }
        }
     }
   }
   return emailBuf;
}
