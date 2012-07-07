// This is a simple date generator that I needed to create a list of possible passwords for a cracker.
// To compile: gcc -o random_dategenerator.c -o dategen
// To run: ./dategen <first year> <last year>
// If no args specified the default is 1970 - 2012 

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int inF=0, inL=0;
    if(argc!=3) {
	inF=1970;
	inL=2012;
    }
    else {
	inF=atoi(argv[1]);
	inL=atoi(argv[2]);
    }
    
    // Not standarized date for example: 141987 for First of April 1987 
    int d,m,y;
    for(y=inF; y<=inL; y++) {
	for(m=0; m<=12; m++) {
	    for(d=0; d<=31; d++) {
		printf("%d%d%d\n",d,m,y);
	    }
	}
    }
    
    // More standarized format for example: 01041987 for First of April 1987
    for(y=inF; y<=inL; y++) {
	for(m=0; m<=12; m++) {
	    if(m<10) {
		for(d=0; d<=31; d++) {
		    if(d<10) printf("0%d0%d%d\n",d,m,y);
		    else printf("%d0%d%d\n",d,m,y);
		}
	    }
	    else {
		for(d=0; d<=31; d++) {
		    if(d<10) printf("0%d%d%d\n",d,m,y);
		    else printf("%d%d%d\n",d,m,y);
		}
	    }
	}
    }

    return 0;
}
