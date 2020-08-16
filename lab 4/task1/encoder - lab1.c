#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LOWER_TO_UPPER 32

int main(int argc, char **argv){
	FILE *output = stdout;
	FILE *input = stdin;
	FILE *

_out = NULL;
	int encoder = 0; // encoder index in argv
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], "-D") == 0){ // debug mode
			debug_out = stderr;
			fprintf(debug_out, "-D\n");
			
		}else if(argv[i][1] == 'e'){ // encode mode
			encoder = i;
		}else if(argv[i][1] == 'o'){ // output file
			output = fopen(argv[i] + 2, "w"); // without -o
		}else if(argv[i][1] == 'i'){ //input file
			input = fopen(argv[i]+2, "r"); // without -i
			if(input == NULL){
				fprintf(stderr, "can't open file %s\n", argv[i] + 2); 
				return 1;
			}
		}else{
			printf("Invalid parameter: %s\n", argv[i]);
			return 1;
		}
	}
	char c;
	int i_encode = 2; // after the -e
	int size = strlen(argv[encoder]) - 2;
	while((c = fgetc(input)) != EOF){
		if(debug_out != NULL && c != '\n'){
			fprintf(debug_out, "%d\t", c);
		}
		if(encoder == 0){ // default
			if(c >= 'a' && c <= 'z'){ // lowercase
				c -= LOWER_TO_UPPER; //convert to uppercase
			}
		}else{ // encoder
			if(c != '\n'){
				int sign = (argv[encoder][0] == '-') ? -1 : 1;
				c += sign * (argv[encoder][i_encode] - 48); //'1'
				i_encode++;
				if(i_encode == size + 2) 
					i_encode = 2;
			}else{ // end line
				i_encode = 2;
			}
		}
		if(debug_out != NULL && c != '\n'){
			fprintf(debug_out, "%d\n", c);
		}
		fprintf(output, "%c", c);
	}
	
 	if(output != stdout) fclose(output);
	if(input != stdin) fclose(input);
	return 0;
}
