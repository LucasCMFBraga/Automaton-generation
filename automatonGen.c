// Finite automaton generator

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//function prototypes
int** state_matrix_creator();
void generates_file_c();
void capture_values();
void create_if_program();
void create_goto_program();
void print_matriz();

//variaveis globais
int n_symbols,n_state;
int initial_state,final_state_numbers;
int program_type;
char program_name[255];
int *final_state;
char *symbols;
int **matrix;


//--------------------------------------------------------------------------------------------------
void main(){

	// captures values ​​for the state matrix
	capture_values();

	// create state matrix with the inputs coditions
	matrix=state_matrix_creator();

	// create the file with the finite automaton code with the options chosen in the input
	generates_file_c();
}


//-----------------------------------------------------------------------------------------------------
void capture_values(){
	printf("------------------------------------------------------------------\n");

	printf("What is the file name ?: ");
	gets(program_name);
	printf("\n------------------------------------------------------------------\n");

	printf("Number of symbols?: ");
	scanf("%d",&n_symbols);

	
	symbols=((char*)malloc(n_symbols*sizeof(char)));
	for(int c=0;c<n_symbols;c++){
		printf("What is the symbol %d ? : ",c);
		scanf(" %c",&symbols[c]);
		}

	printf("\n------------------------------------------------------------------\n");
	printf("Number of states ? : ");
	scanf("%d",&n_state);

	printf("\n------------------------------------------------------------------\n");
	printf("What is the initial state ? : ");
	scanf("%d",&initial_state);

	printf("\n------------------------------------------------------------------\n");
	printf("Number of final states ? : ");
	scanf("%d",&final_state_numbers);

	final_state=((int*)malloc(final_state_numbers*sizeof(int)));
	for(int c=0;c<final_state_numbers;c++){
		printf("What is the final state %d ? : ",c);
		scanf("%d",&final_state[c]);
	}
	
	printf("\n------------------------------------------------------------------\n");
	printf("Hit, 1-for (function)   or   2-for (goto):");
	scanf("%d",&program_type);
	
	printf("\n------------------------------------------------------------------\n");
	printf("\nInsert -1 if the transition does not exist !\n\n");

		
	
}



//-----------------------------------------------------------------------------------

int** state_matrix_creator(){
	int **state_matrix,*symbols_coluna;

	// creating state matriz
	state_matrix=((int**)malloc(n_state*sizeof(int*)));
	for(int c=0;c<=n_state;c++){
		state_matrix[c]=((int*)malloc(n_symbols*sizeof(int)));
	}

	// Inseting values
	for (int i=0;i<n_state;i++){
		for(int j=0;j<n_symbols;j++)
		{
			printf("For state (%d) and o symbol (%c). What is the next state ? : ",i,symbols[j]);
			scanf("%d",&state_matrix[i][j]);
		}
	}

	return state_matrix;
}


//--------------------------------------------------------------------------------------------
void generates_file_c (){
	
	FILE *file_c;

	if((file_c=fopen(program_name,"wt"))==NULL){
		printf("failed to open");
		exit(0);
	}


	// Writing the header
	fprintf(file_c,"#include <stdio.h>\n#include <conio.h>\n#include <stdlib.h>\n\n");

	if(program_type==1){ // create "fuction" program 
		create_if_program(file_c);
	}
	else if(program_type==2) // create "goto" program
		create_goto_program(file_c);

	print_matriz();

	printf(" \n\n*************************program created*************************\n");

	fclose(file_c);
}


//----------------------------------------------------------------------------------------------

void create_if_program(FILE *file_c){
	int One_more,Is_final=0;

	// Writing prototype functions
	fprintf(file_c,"//prototype functions\n");
	for(int c=0;c<n_state;c++){
		fprintf(file_c,"void s%d();\n",c);
	}
	fprintf(file_c,"void accepts();\n");
	fprintf(file_c,"void rejects();\n");


	//Global variables statements
	fprintf(file_c,"\n\n//Global variables\nchar f[250];\nint p;\n\n");


	//Logic to make code
	for(int i=0;i<n_state;i++){ //  runs the state matrix, like the rows
		One_more=1;
		Is_final=0;
		fprintf(file_c,"void s%d() {\n",i);
		for(int j=0;j<n_symbols;j++){ // runs the state matrix, like the columns 
			
			if(matrix[i][j]>=0 && One_more){ // write on file if the value more than 0
				fprintf(file_c,"\tif (f[p]=='%c'){\n",symbols[j]);
				fprintf(file_c,"\t\tp++;\n");
				fprintf(file_c,"\t\ts%d();\n",matrix[i][j]);
				fprintf(file_c,"\t}\n");
				One_more=0;
				}

			else if(matrix[i][j]>=0 && !One_more){
				fprintf(file_c,"\telse if (f[p]=='%c'){\n",symbols[j]);
				fprintf(file_c,"\t\tp++;\n");
				fprintf(file_c,"\t\ts%d();\n",matrix[i][j]);
				fprintf(file_c,"\t}\n");
			}

		}
		
		for(int c=0;c<final_state_numbers;c++)//Checks whether it is the final state
			if(final_state[c]==i)
				Is_final=1;
		
		if(Is_final){
			fprintf(file_c,"\telse if ( f[p] == 0 ){\n\t\taccepts( );\n\t}\n\telse\n\t\trejects( );\n}\n\n");
		}
		else if(One_more){
			fprintf(file_c,"\trejects();\n}\n\n");
		}

		else{
			fprintf(file_c,"\telse\n");
			fprintf(file_c,"\t\trejects();\n}\n\n");
		}	
		
	}

	//Function statement rejects and accepts.
	fprintf(file_c,"void accepts( ){\n\tprintf(\"accepted\\n\");\n\tgetch();\n\texit(0);\n}\n");

	fprintf(file_c,"void rejects( ){\n\tprintf(\"rejected\\n\");\n\tgetch();\n\texit(0);\n}\n");
	
	// Main function statement 
	fprintf(file_c,"int main( ){\n\tp = 0;\n\tprintf(\"Insert the word: \");\n\tgets(f);\n\ts%d( );\n\treturn(0);\n}",initial_state);

}


//----------------------------------------------------------------------------------------------------------------
void create_goto_program(FILE *file_c){


int One_more,Is_final=0;

	// Main function statement
	fprintf(file_c,"int main( ){\n\tchar f[250];\n\tint p=0;\n\tprintf(\"Insert the word: \");\n\tgets(f);\n\tgoto S%d;\n\n",initial_state);

	
	//Logic to make code
	for(int i=0;i<n_state;i++){ //  runs the state matrix, like the rows
		One_more=1;
		Is_final=0;
		fprintf(file_c,"S%d: \n",i);
		for(int j=0;j<n_symbols;j++){// runs the state matrix, like the columns
			
			if(matrix[i][j]>=0 && One_more){ // write on file if the value more than 0
				fprintf(file_c,"\tif (f[p]=='%c'){\n",symbols[j]);
				fprintf(file_c,"\t\tp++;\n");
				fprintf(file_c,"\t\tgoto S%d;\n",matrix[i][j]);
				fprintf(file_c,"\t}\n");
				One_more=0;
				}

			else if(matrix[i][j]>=0 && !One_more){
				fprintf(file_c,"\telse if (f[p]=='%c'){\n",symbols[j]);
				fprintf(file_c,"\t\tp++;\n");
				fprintf(file_c,"\t\tgoto S%d;\n",matrix[i][j]);
				fprintf(file_c,"\t}\n");
			}

		}
		for(int c=0;c<final_state_numbers;c++)//Checks whether it is the final state
			if(final_state[c]==i)
				Is_final=1;
		
		if(Is_final){
			fprintf(file_c,"\telse if ( f[p] == 0 ){\n\t\t goto ACCEPTS;\n\t}\n\telse\n\t\tgoto REJECTS;\n\n");
		}
		else if(One_more){
		fprintf(file_c,"\tgoto REJECTS;\n}\n\n");
		}	
		else{
			fprintf(file_c,"\telse\n");
			fprintf(file_c,"\t\tgoto REJECTS;\n\n");
		}	
		
	}

	//Function statement rejects and accepts
	fprintf(file_c,"ACCEPTS:\n\tprintf(\"accepted\\n\");\n\tgetch();\n\treturn(0);\n\n");
	fprintf(file_c,"REJECTS:\n\tprintf(\"rejected\\n\");\n\tgetch();\n\treturn(0);\n\n}\n");


}

void print_matriz(){
	char n=0;
	printf("\n---------------------------States Matrix---------------------------\n");

	for(int i=0;i<n_symbols;i++) //Symbol rows
		printf("%5c",symbols[i]);
	printf("\n   ");

	for(int i=0;i<n_symbols;i++) 
		printf("-----");	
	printf("\n");

	for(int i=0;i<n_state;i++){ //rows
		for(int j=0;j<n_symbols;j++){ //columns
			if(j==0){
				if(matrix[i][j]<=-1)
					printf("S%d|%2c__",i,n);
				else
					printf("S%d|%2cS%d",i,n,matrix[i][j]);
			}else{
				if(matrix[i][j]<=-1)
					printf("%4c__",n);
				else
				printf("%4cS%d",n,matrix[i][j]);
			}
		}
	printf("\n");
 	}
}