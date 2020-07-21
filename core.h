#include <wchar.h> //pubs.opengroup.org

//Structure to hold time
struct Time
{
	int hour;
	int minute;
	int second;
	int msecond;
};


void printT(const struct Time time);
void fprintT(const struct Time time, FILE * wfile);
void initT(struct Time * time);
int convertT(struct Time * time, double seconds);
int convert_srt(char file_name[]);
int renum_srt(char file_name[]);
int split_srt(char file_name[]);
char * change_ext(char * file_name, char * extension);
char * remove_ext(char * file_name);
int split_file(FILE file, FILE enfile, FILE rufile);
char * language(wchar_t * text);
