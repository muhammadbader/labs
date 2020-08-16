#include <stdlib.h>
#include <stdio.h>
#include <string.h>

 struct fun_desc {
  char *name;
  char (*fun)(char);
};

char encrypt(char c); /* Gets a char c and returns its encrypted form by adding 3 to its value.
          If c is not between 0x20 and 0x7E it is returned unchanged */
char decrypt(char c); /* Gets a char c and returns its decrypted form by reducing 3 to its value. 
            If c is not between 0x20 and 0x7E it is returned unchanged */
char dprt(char c); /* dprt prints the value of c in a decimal representation followed by a 
           new line, and returns c unchanged. */
char cprt(char c); /* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed 
                    by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns 
                    the value of c unchanged. */
char my_get(char c); /* Ignores c, reads and returns a character from stdin using fgetc. */

char quit(char c); /* Gets a char c,  and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */

void menus(struct fun_desc *menu,char* carray);

char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}
 
char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  /* TODO: Complete during task 2.a */
  for(int i=0;i<array_length;++i){
      mapped_array[i]=f(array[i]);
  }
  free(array);
  return mapped_array;
}
 


int main(int argc, char **argv){
  /* TODO: Test your code */
      char *carray=(char*)malloc(5*sizeof (char));
      struct fun_desc menu[]={{"Censor",censor},{"Encrypt",encrypt},{"Decrypt",decrypt},{"Print dec",dprt},{"Print string",cprt},{"Get string",my_get},{"Quit",quit},{NULL,NULL}};
      menus(menu,carray);
      free(carray);
  return 0;
}
int counter = 0;
char *buffer="";
void menus(struct fun_desc *menu,char* carray){
    while(1){
        int i=0;
        while(menu[i].name!=NULL){
          printf("%d) %s\n",i,menu[i].name);
          i++;
        }
        printf("Option:");
        int number;
        char c;
        while(c=getchar()!='\n'){
            counter++;
            char *tmp = buffer;
            buffer=(*char)malloc(sizeof(char)*counter);
            strcpy(buffer,tmp,c);
        }
        number = atoi(buffer);
        if (number>=0&&number<=6){
            printf("Within bounds\n");
        }else{
            printf("Not within bounds");
            exit(3);
        }
        carray=map(carray,5,menu[number].fun);
        printf("Done.\n\n");
    }

}

// /* Gets a char c and returns its encrypted form by adding 3 to its value. If c is not between 0x20 and 0x7E it is returned unchanged */
char encrypt(char c){
    return (c>=0x20&&c<=0x7e)?c+3:c;
} 

/* Gets a char c and returns its decrypted form by reducing 3 to its value. 
            If c is not between 0x20 and 0x7E it is returned unchanged */
char decrypt(char c){
    return (c>=0x20&&c<=0x7e)?c-3:c;
}

/* dprt prints the value of c in a decimal representation followed by a 
           new line, and returns c unchanged. */
char dprt(char c){
    printf("%d\n",c);
    return c;
}


/* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed 
                    by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns 
                    the value of c unchanged. */
char cprt(char c){
    (c>=0x20&&c<=0x7e)?printf("%c\n",c):printf(".\n");
    return c;
}

/* Ignores c, reads and returns a character from stdin using fgetc. */
char my_get(char c){
    return fgetc(stdin);
}

/* Gets a char c,  and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */
char quit(char c){
    if(c=='q'){
        exit(3);
    }
    return c;
} 
