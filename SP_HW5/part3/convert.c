/*
 * convert.c : take a file in the form 
 *  word1
 *  multiline definition of word1
 *  c
 * followed by a blank line
 * word2....etc
 * convert into a file of fixed-length records
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "dict.h"
#define BIGLINE 512

int main(int argc, char **argv) {
	FILE *in;
	FILE *out;        /* defaults */
	char line[BIGLINE];
	static Dictrec dr, blank;
	
	/* If args are supplied, argv[1] is for input, argv[2] for output */
	if (argc==3) {
		if ((in =fopen(argv[1],"r")) == NULL){DIE(argv[1]);}
		if ((out =fopen(argv[2],"w")) == NULL){DIE(argv[2]);}	
	}
	else{
		printf("Usage: convert [input file] [output file].\n");
		return -1;
	}

	/* Main reading loop : read word first, then definition into dr */

	/* Loop through the whole file. */
	while (!feof(in)) {
		
		/* Create and fill in a new blank record.
		 * First get a word and put it in the word field, then get the definition
		 * and put it in the text field at the right offset.  Pad the unused chars
		 * in both fields with nulls.
		 */

		/* Read word and put in record.  Truncate at the end of the "word" field.
		 *
		 * Fill in code. */
		 fgets(line,BIGLINE,in);
		 if(!feof(in)){
		 	if(strcmp(line,"\n")!=0){
		 		strcpy(dr.word,line);
		 	}
		 	int j=0;
		 	for(int i =strlen(dr.word)-1;i<WORD;i++){
		 		dr.word[i]='\0';
		 		//j++;
		 	}
			
		/* Read definition, line by line, and put in record.
		 *
		 * Fill in code. */
			fgets(line,BIGLINE,in);
			for(int i =0;i<TEXT;i++){
		 		dr.text[i]='\0';
		 	}
			while(strcmp(line,"\n")!=0){
				for(int i =strlen(line)-1;i<BIGLINE;i++){
		 			line[i]='\0';
		 			//j++;
		 		}
				strcat(dr.text,line);
				fgets(line,BIGLINE,in);	
			}
		
			 for(int i =strlen(dr.text);i<480;i++){
		 		dr.text[i]='\0';
		 	}

		/* Write record out to file.
		 *
		 * Fill in code. */
		 	fwrite(&dr,sizeof(Dictrec),1,out);
		 	
		}
		
		
	}
	
	fclose(in);
	fclose(out);
	return 0;
}
