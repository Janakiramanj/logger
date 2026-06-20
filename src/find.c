#include"find.h"

int seek;

char* find(char* path, char* search_string)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
//        printf("%s doesn't Exist !!\n", path);
        return NULL;
    }

    int i = 0;
    char ch;

    char *buff = malloc(128);
    if (!buff) {
        close(fd);
        return NULL;
    }

 //   int match = 0;

    while (read(fd, &ch, 1) == 1)
    {
        if (ch != ' ' && ch != '\n' && ch != '\t')
        {
            if (i < 127)
                buff[i++] = ch;
        }
        else
        {
            buff[i] = '\0';

            if (i > 0 && strcmp(buff, search_string) == 0)
            {
                // found keyword → now read value

                i = 0;

                while (read(fd, &ch, 1) == 1)
                {
                    if (ch == '\n')
                        break;

                    if (ch != ' ' && ch != '\t')
                        buff[i++] = ch;
                }

                buff[i] = '\0';

                close(fd);
                return buff;
            }

            i = 0;
        }
    }

    free(buff);
    close(fd);
    return NULL;
}

char* seeknfind(int fd,char* search_string){

    int i=0;
    char ch;
    char* buff = malloc(30);

while(read(fd,&ch,1)){

	if((ch!=' ')&&(ch!='\n')&&(ch!='\t')){
	  buff[i] = ch;
	  i++;}
	  
	else
	{
	           while(i){
			buff[i]='\0';
			i--;}
	           buff[i]='\0';
	}

	if(i==strlen(search_string))
	{
		
		buff[i+1]='\0';

		if(strcmp(search_string,buff))
		{
			while(i){
				buff[i]='\0';
				i--;}
			buff[i]='\0';
		}
		else
		{

			while(i){
				buff[i]='\0';
				i--;}
			buff[i]='\0';

			while((ch!='\n'))
			{
				if(read(fd,&ch,1)==0)
					break;
				if((ch!=' ')&&(ch!='\n')&&(ch!='\t')){
				buff[i]=ch;
				i++;}
			}
			buff[i+1]='\0';

			return buff;
		}

	}
}

return NULL;
}
