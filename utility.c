#include <stdio.h>
#include <string.h>
#include "core.h"
#include "utility.h"


//Global constants
#define FILE_NAME 100 
#define PROMPT 30
#define RNUM 30
#define SNUM 1000


//Prints a line of characters
//Input: the character type and how many to print
//Output: none 
void printhlc(char character, int count)
{
	for(int i = 0; i < count; ++i)
	{
		printf("%c", character);
	}
	printf("\n");

	return;
}



//Displays the menu for the program
//Input: none
//Output: none 
void print_menu(void)
{
	printf("1. Show menu\n");
	printf("2. Convert to SRT\n");
	printf("3. Renumber SRT\n");
	printf("4. Split SRT\n");
	printf("0. Exit\n");

	return;
}



//Gets a number
//Input: none
//Output: an integer
int get_num(char prompt[])
{
	int integer = 0;
	int temp = 0;

	while(temp == 0)
	{	
		printf("%s", prompt);
		temp = scanf("%d", &integer);
		while(getchar() != '\n'){}
	}
	return integer;
}



//Gets a string of characters from user and stores in text
//Input: text, size of text and prompt for text
//Output: none
void get_str(char * text, int size, char * prompt)
{
	//If text is NULL, return 
	if(text == NULL)
	{
		return;
	}

	//If size of text is less than 1, return
	if(size < 1)
	{
		return;
	}
	
	printf("%s", prompt);
	fgets(text, size, stdin);


	int end = strlen(text) - 1;

	//gets rid of new line character if
	//input is shorter than size - 1
	if(text[end] == '\n')
	{
		text[end] = '\0';
	}
	else
	{
		//Extracts extra characters typed	
		while(getchar() != '\n'){}
	}
	return;
}



//Replaces the new line character at the end of a string with a '\0' 
//Input: a string of characters and its length
//Output: replaced or not replaced (1/0)
int rep_nlc(char * string)
{
	if(!string)
	{
		return 0;
	}
	else
	{
		int end = strlen(string) - 1;
		
		if(string[end] == '\n')
		{
			string[end] = '\0';
		}
		else
		{
			return 0;
		}
	}
	return 1;
}



//Executes a job requested by user
//Input: a job number from menu
//Output: none
void exe_job(int job)
{
	//If the request is to show menu, then
	//shows menu
	if(job == 1)
	{
		printf("\n");
		print_menu();
		return;
	}

	//If the request is to convert to SRT, then
	//attempts to do that
	if(job == 2)
	{
		int status = 0;
		char file_name[FILE_NAME];
		char prompt[] = "Enter file name: ";

		printf("\nConverting to SRT.\n\n");
		
		//Gets the file name
		get_str(file_name, FILE_NAME, prompt);

		//Performs conversion of a file	
		status = convert_srt(file_name);

		printf("Job status: ");
		if(status == 1)
		{
			printf("success\n");
		}
		else
		{
			printf("failed\n");
		}
		return;
	}

	//If the request is to renumber SRT subtitles, 
	//then attempts to do that
	if(job == 3)
	{
		printf("\nRenumbering SRT.\n\n");
		
		int status = 0;
		char file_name[FILE_NAME];
		char prompt[] = "Enter file name: ";

		//Gets the file name
		get_str(file_name, FILE_NAME, prompt);
		
		//Does renumbering	
		status = renum_srt(file_name);

		printf("Job status: ");
		if(status == 1)
		{
			printf("success\n");
		}
		else
		{
			printf("failed\n");
		}
		return;

	}

	//If the request is to split SRT, then
	//tries to split the SRT file containing
	//subtitles in different language to separate
	//files with only one language in each
	if(job == 4)
	{
		printf("\nSplitting SRT.\n\n");
		
		int status = 0;
		char file_name[FILE_NAME];
		char prompt[] = "Enter file name: ";

		//Gets the file name
		get_str(file_name, FILE_NAME, prompt);
		
		//Does splitting
		status = split_srt(file_name);

		printf("Job status: ");
		if(status == 1)
		{
			printf("success\n");
		}
		else
		{
			printf("failed\n");
		}
		return;
	}

	//If the request is to exit the application,
	//than it does just that or says the job
	//doesn't exist
	if(job == 0)
	{
		printf("\nExiting ...\n");
		return;
	}
	else
	{
		printf("\nNo such job\n");
		return;
	}
}
