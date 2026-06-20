#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<pthread.h>
#include<wait.h>

extern int seek;

char* find(char* ,char* );
char* seeknfind(int ,char* );
void replaceafterfind(char*,char*,char* );
int replaceafterfindnseek(int ,char* ,char* );


