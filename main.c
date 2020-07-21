#include <stdio.h>
#include "core.h"
#include "utility.h"


//Global constants
#define PROMPT 30


int main(void)
{
	printf("\n");
	
	char star = '*';
	int star_num = 30;

	printhlc(star, star_num);
	printf("Wellcome to SubOps\n");
	printhlc(star, star_num);

	printf("\n");

	print_menu();
	printf("\n");
	printhlc(star, star_num);

	int responce = 0;
	char prompt[PROMPT] = "Job number [1]: ";

	do
	{
		responce = get_num(prompt);
		printhlc(star, star_num);
		exe_job(responce);
		printf("\n");
		printhlc(star, star_num);
	}
	while(responce != 0);
	
	printf("\n");
	return 0;
}
