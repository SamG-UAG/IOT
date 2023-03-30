#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

#define TOTAL 100

int main(){
	int fdr, fdw;
	char buffer[TOTAL];
	
	do{
        fdr = open ("comm1", O_RDONLY);
        fdw = open ("comm2", O_WRONLY);

        printf ("Digita el texto: ");
        gets(buffer);
        write(fdw, buffer, sizeof(buffer));

        read(fdr, buffer, sizeof(buffer));
        printf("Mensaje: %s\n", buffer);
    } while (strcmp(buffer, "Adios") != 0);

    close(fdw);
    close(fdr);

    return 0;
}