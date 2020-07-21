#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "utility.h"
#include "core.h"

//Global constants
#define HOUR_LIMIT 86399.999444
#define SNUM 151
#define TCNUM 30 


//Initializes the Time vars to 0
//Input: Time structure
//Output: none
void initT(struct Time * time)
{
	time->hour = 0;
	time->minute = 0;
	time->second = 0;
	time->msecond = 0;

	return;
}



//Displays the given time
//Input: Time structure
//Output: none
void printT(const struct Time time)
{
	//Prints hour
	if(time.hour < 10)
	{
		printf("0");
	}
	printf("%d", time.hour);

	printf(":");

	//Prints minute
	if(time.minute < 10)
	{
		printf("0");
	}
	printf("%d", time.minute);

	printf(":");

	//Prints second 
	if(time.second < 10)
	{
		printf("0");
	}
	printf("%d", time.second);

	printf(",");

	//Prints millisecond 
	if(time.msecond < 100)
	{
		printf("0");
	}

	if(time.msecond < 10)
	{
		printf("0");
	}
	printf("%d\n", time.msecond);

	return;
}



//Writes the given time to file
//Input: Time structure
//Output: none
void fprintT(const struct Time time, FILE * wfile)
{
	//Writes hour
	if(time.hour < 10)
	{
		fprintf(wfile, "0");
	}
	fprintf(wfile, "%d", time.hour);

	fprintf(wfile, ":");

	//Writes minute
	if(time.minute < 10)
	{
		fprintf(wfile, "0");
	}
	fprintf(wfile, "%d", time.minute);

	fprintf(wfile, ":");

	//Writes second 
	if(time.second < 10)
	{
		fprintf(wfile, "0");
	}
	fprintf(wfile, "%d", time.second);

	fprintf(wfile, ",");

	//Writes millisecond 
	if(time.msecond < 100)
	{
		fprintf(wfile, "0");
	}

	if(time.msecond < 10)
	{
		fprintf(wfile, "0");
	}
	fprintf(wfile, "%d", time.msecond);

	return;
}



//Converts seconds to hh:mm:ss,mls format
//and stores the results in "time"
//Input: Time structure
//Output: success or failure (1/0)
int convertT(struct Time * time, double seconds)
{
	//If the number of seconds exceeds the
	//program's limit, then return error
	if(seconds > HOUR_LIMIT)
	{
		return 0;
	}
	
	//Stores the number of hours
	double hours = seconds / 3600;
	time->hour = (int)(hours);

	//Stores the number of minutes
	double minutes = seconds / 60;
	if(minutes > 60)
	{
		minutes = minutes - (double) (time->hour * 60);
	}
	time->minute = (int)(minutes);
		
	//Stores the number of seconds
	int int_hours_minutes = time->hour * 3600 + time->minute * 60;
	double double_hours_minutes = (double) int_hours_minutes;
	double lseconds = seconds - double_hours_minutes;
	time->second = (int)(lseconds);

	//Stores the number of milliseconds
	double double_mseconds = (int) seconds;
	double_mseconds = ((seconds - double_mseconds) * 1000);
	time->msecond = (int)(double_mseconds);

	return 1;
}



//Converts Label file to SRT file
//Input: name of the file
//Output: success (1) or failure (0)
int convert_srt(char file_name[])
{
	FILE * rfile;
	FILE * wfile;

	int fnsize = strlen(file_name);
	char rfile_name[fnsize + 1];	
	strcpy(rfile_name, file_name);


	//Tries to open a file for reading only
	//if it exists and display status
	printf("Open [%s]: ", rfile_name);
	if((rfile = fopen(rfile_name, "r")) == NULL)
	{
		printf("failed\n");
		return 0;
	}
	else
	{
		printf("success\n");
	}


	char * wfile_name = NULL;
	wfile_name = change_ext(file_name, "srt");

	//Tries to open a file for writing 
	//and display status
	printf("Open [%s]: ", wfile_name);
	if((wfile = fopen(wfile_name, "w")) == NULL)
	{
		if(wfile_name)
		{
			free(wfile_name);
		}
		fclose(rfile);
		printf("failed\n");
		return 0;
	}
	else
	{
		printf("success\n");
	}


	double temp_time = 0;
	struct Time time;
	
	//Initializes time to 0 equivalent values
	initT(&time);


	int sub_num= 1;	
	char text[SNUM + 1];
	int removed = 0;

	//Gets subtitle start time from external file 
	fscanf(rfile, "%lf", &temp_time);
	
	while(feof(rfile) == 0)
	{
		//Writes a number to external file
		fprintf(wfile, "%d\n", sub_num);

		//Converts the start time from external file
		//into hh:mm:ss,mls format. Also, it  writes
		//to an external file.
		convertT(&time, temp_time);
		fprintT(time, wfile);

		//Skipps the tab and writes arrow to external file
		fgetc(rfile);
		fprintf(wfile, "%s", " --> ");

		//Gets subtitle end time from external file,
	        //and converts it into hh:mm:ss,mls format.
		//Also, it writes to external file.
		fscanf(rfile, "%lf", &temp_time);
		convertT(&time, temp_time);
		fprintT(time, wfile);
		fprintf(wfile, "\n");

		//Gets the subtitle line ignoring the tab character.
		//It also removes the new line character if exists in
		//text
		fgetc(rfile);
		fgets(text, SNUM, rfile);
		removed = rep_nlc(text);
		
		//Writes the contents of text to wfile with a new
		//line character appended. If new line character has 
		//not been removed from the text, then ignores 
		//possible extra characters in rfile	
		fprintf(wfile, "%s\n", text);
		if(removed == 0)
		{
			while(fgetc(rfile) != '\n'){}
		}
		
		++sub_num;

		//Gets subtitle start time from external file 
		fscanf(rfile, "%lf", &temp_time);
		
		//Insert space only if the end of file
		//has not been reached yet
		if(feof(rfile) == 0)
		{
			fprintf(wfile, "\n");
		}
	}

	fclose(rfile);
	fclose(wfile);

	//Frees the momory used to store the name of the
	//file to write to	
	free(wfile_name);


	//Removes the file, the numbers were read from	
	removed = remove(rfile_name);
	
	printf("Remove [%s]: ", rfile_name);
	if(removed == 0)
	{
		printf("success\n\n");	
	}
	else
	{
		printf("failed\n\n");	
	}

	return 1;
}



//Renumbers the subtitle numbers if they are not correct
//Input: file name
//Output: success or failure (1/0)
int renum_srt(char file_name[])
{
	FILE * rfile;
	FILE * wfile;

	char rfile_name[strlen(file_name) + 6];	
	strcpy(rfile_name, file_name);
	strcat(rfile_name, ".temp");


	int renamed = -1;
	
	//Renames file from user	
	renamed = rename(file_name, rfile_name);

	printf("Renaming: ");	
	if(renamed == 0)
	{
		printf("success\n");
	}
	else
	{
		printf("failed\n");
		return 0;
	}


	//Tries to open a file for reading only
	//if it exists and display status
	printf("Open [%s]: ", rfile_name);
	if((rfile = fopen(rfile_name, "r")) == NULL)
	{
		printf("failed\n");
		return 0;
	}
	else
	{
		printf("success\n");
	}


	char wfile_name[strlen(file_name) + 1];
	strcpy(wfile_name, file_name);
	
	//Tries to open a file for writing 
	//and display status
	printf("Open [%s]: ", wfile_name);
	if((wfile = fopen(wfile_name, "w")) == NULL)
	{
		fclose(rfile);
		printf("failed\n");
		return 0;
	}
	else
	{
		printf("success\n");
	}


	int count = 1;	
	int removed = 0;

	char tctext[TCNUM];
	char sltext[SNUM];
	
	//Ignores all characters on sub number line
	while(fgetc(rfile) != '\n' && feof(rfile) == 0){}

	while(feof(rfile) == 0)
	{
		//Writes the sub number to wfile
		fprintf(wfile, "%d\n", count);
	

		//Reads the time code line and might remove '\n' from
		//tctext
		fgets(tctext, TCNUM, rfile);
		removed = rep_nlc(tctext);
		
		if(feof(rfile) == 0)
		{	
			//Writes the contents of tctext to wfile with
		        //a new line character appended. If new line 
			//character has not been removed from the 
			//tctext, then ignores possible extra 
			//characters in rfile	
			fprintf(wfile, "%s\n", tctext);
			if(removed == 0)
			{
				while(fgetc(rfile) != '\n' && 
				      feof(rfile) == 0){}
			}
		}

		
		//Subtitle line(s)
		char tchar = '\0';
		tchar = fgetc(rfile);
		while(feof(rfile) == 0 && tchar != '\n')
		{
			ungetc(tchar, rfile);

			//Reads the line and might remove '\n' from
			//sltext
			fgets(sltext, SNUM, rfile);
			removed = rep_nlc(sltext);
		
			//Writes the contents of sltext to wfile 
			//with a new line character appended. If new 
			//line character has not been removed from 
			//the tctext, then ignores possible extra 
			//characters in rfile
			if(feof(rfile) == 0)
			{
				fprintf(wfile, "%s\n", sltext);
			}
			if(removed == 0)
			{
				while(fgetc(rfile) != '\n' &&
				      feof(rfile) == 0){}
			}
			tchar = fgetc(rfile);
		}
		ungetc(tchar, rfile);
		
		++count;

		//Ignores all characters on an empty line 
		while(fgetc(rfile) != '\n' && feof(rfile) == 0){}
		
		//Unless it's the end of rfile, write a new line
		//character to wfile
		if(feof(rfile) == 0)
		{
			fprintf(wfile, "\n");
		}

		//Ignores all characters on sub number line 
		while(fgetc(rfile) != '\n' && feof(rfile) == 0){}
	}

	fclose(rfile);
	fclose(wfile);


	//Removes the file, the information was read from	
	removed = remove(rfile_name);
	
	printf("Remove [%s]: ", rfile_name);
	if(removed == 0)
	{
		printf("success\n\n");	
	}
	else
	{
		printf("failed\n\n");	
	}
	return 1;
}



//Calls a routine to split file's content into a few files such that
//each file would hold subtitles in only one language
//Input: a file name to split subtitles from
//Output: success (1) or failure (0)
int split_srt(char file_name[])
{
	FILE * rfile;
	FILE * wenfile;
	FILE * wrufile;
	char * rfname;
	char * enfname;
	char * rufname;
	char * temp;
	int rfsize = 0;
	int enfnlen = 0;
	int rufnlen = 0;

	//Tries to open a file for reading only
	//if it exists and display status
	rfname = file_name;
	printf("Open [%s]: ", rfname);
	if((rfile = fopen(rfname, "r")) == NULL)
	{
		printf("failed\n");
		return 0;
	}
	else
	{
		printf("success\n");
	}
	
	//Creates a file name for english subtitles
	rfsize = strlen(rfname);
	temp = malloc(sizeof(char) * rfsize + 1);
	strcpy(temp, rfname);
	temp = remove_ext(temp);
	enfnlen = strlen(temp) + 1;
	enfname = malloc(sizeof(char) * enfnlen + 8);
	strcpy(enfname, temp);
	strcat(enfname, "[EN].srt");
	
	//Tries to open a file for english subtitles
	//in a write only mode
	printf("Open [%s]: ", enfname);
	if((wenfile = fopen(enfname, "w")) == NULL)
	{
		free(temp);
		free(enfname);
		printf("failed\n");
		return 0;
	}
	else
	{
		printf("success\n");
	}
	
	//Creates a file name for russian subtitles	
	rufnlen = enfnlen;
	rufname = malloc(sizeof(char) * rufnlen + 8);
	strcpy(rufname, temp);
	strcat(rufname, "[RU].srt");
	
	//Tries to open a file for russian subtitles
	//in a write only mode
	printf("Open [%s]: ", rufname);
	if((wrufile = fopen(rufname, "w")) == NULL)
	{
		free(temp);
		free(enfname);
		free(rufname);
		printf("failed\n");
		return 0;
	}
	else
	{
		printf("success\n");
	}
	
	//Sets the current locale
	setlocale(LC_ALL, "ru_RU.UTF-8");

	//Writes lines to appropriate file based on the detected language
	char lang[3];	
	wchar_t ws[100];
	wint_t * ret = fgetws(ws, 100, rfile);
	while(ret != NULL)
	{
		strcpy(lang, language(ws));
		if(strcmp(lang, "en") == 0)
			fputws(ws, wenfile);
		else if(strcmp(lang, "ru") == 0)
			fputws(ws, wrufile);
		else
		{
			fputws(ws, wenfile);
			fputws(ws, wrufile);
		}
		ret = fgetws(ws, 100, rfile);
	}
	
	//Free memory and close file streams
	free(temp);
	free(enfname);
	free(rufname);
	fclose(rfile);
	fclose(wenfile);
	fclose(wrufile);

	return 1;
}



//Changes the extension of a file
//Input: file name and extension to change to
//Output: pointer to the new file name and extension memory 
char * change_ext(char * file_name, char * extension)
{
	int fnsize = strlen(file_name);
	int esize = strlen(extension);

	//Quit if the file name size and extension size
	//are less than 1
	if(fnsize < 1 || esize < 1)
	{
		return 0;
	}
		
	int index = fnsize - 1;
	int found = 0;

	//Finds the location of the first dot
	//searching from the end of the file name
	while(index > 0 && found == 0)
	{
		if(file_name[index] == '.')
		{
			found = 1;
		}
		else
		{
			--index;
		}
	}
	
	char * nfile_name;
	int new_size = 0;
	
	//Allocates the right amount of memory 
	//for the new file name
	if(found == 0)
	{
		index = fnsize;
		new_size = fnsize + esize + 2;
		nfile_name = calloc(new_size, sizeof(char));
	}
	else
	{
		new_size = index + esize + 2;
		nfile_name = calloc(new_size, sizeof(char));
	}
	
	//Copies the file name until the dot
	for(int i = 0; i < index; ++i)
	{
		nfile_name[i] = file_name[i];
	}

	//Adds the dot and extension parts
	nfile_name[index] = '.';
	strcat(nfile_name, extension);
	
	return nfile_name;
}



//Removes file's extension if it exists
//File extension is assumed to be any characters after the last "."
//Input: character array to remove an extension from
//Output: a new character array without the extension
char * remove_ext(char * file_name)
{
		int found = 0;
		int index = 0;

		//Checks if file name exists and returns if it doesn't
		if(!file_name)
				return NULL;

		//Checks if the "." exists and if it is, gives
		//its location as an index
		int fnlen = strlen(file_name);
		int i = fnlen - 1;
		while(i >= 0 && found == 0)
		{
				if(file_name[i] == '.')
				{
						found = 1;
						index = i;
				}
				--i;
		}
		
		//If "." is found, remove extension.
		if(found == 1 && index > 0)
		{
			char * new_name = malloc(sizeof(char)*index+1);
			for(int j = 0; j < index; ++j)
			{
					new_name[j] = file_name[j];
			}
			new_name[index] = '\0';
			free(file_name);
			return new_name;
		}
		return file_name;
}



//Returns the language code for the characters in a wide string.
//Currently, works for English and Russian languages only
//Input: string of characters
//Output: the language name, "xx", or "-1" 
char * language(wchar_t * text)
{
	//If string is not provided, return error
	if(!text)
		return "-1";

	int tleng = wcslen(text);
	int isLetter = 0;
	int isruRange = 0;
	int isenRange = 0;
	int ruCount = 0;
	int enCount = 0;
	wint_t letter = 0;
	
	//Counts how many Russian and English letters the text has
	for(int i = 0; i < tleng; ++i)
	{
		letter = (wint_t) text[i];
		isLetter = iswalpha(letter);
		isruRange = letter >= 1040 && letter <= 1103;
		isenRange = isalpha((int) letter);

		if(isLetter && isruRange)
			++ruCount;
		else if(isLetter && isenRange)
			++enCount;
	}
	
	//Returns the detected language code
	if(enCount > 0)
		return "en";
	else if(ruCount > 0)
		return "ru";
	return "mx";
}
