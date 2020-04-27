#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int main( ){
	char f[250];
	int p=0;
	printf("Insert the word: ");
	gets(f);
	goto S1;

S0: 
	if (f[p]=='a'){
		p++;
		goto S0;
	}
	else if (f[p]=='b'){
		p++;
		goto S1;
	}
	else
		goto REJECTS;

S1: 
	if (f[p]=='a'){
		p++;
		goto S2;
	}
	else if (f[p]=='b'){
		p++;
		goto S0;
	}
	else if ( f[p] == 0 ){
		 goto ACCEPTS;
	}
	else
		goto REJECTS;

S2: 
	if (f[p]=='a'){
		p++;
		goto S1;
	}
	else if (f[p]=='b'){
		p++;
		goto S2;
	}
	else
		goto REJECTS;

ACCEPTS:
	printf("accepted\n");
	getch();
	return(0);

REJECTS:
	printf("rejected\n");
	getch();
	return(0);

}
