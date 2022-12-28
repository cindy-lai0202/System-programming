/*
 * lookup1 : straight linear search through a local file
 * 	         of fixed length records. The file name is passed
 *	         as resource.
 */
#include <string.h>
#include "dict.h"
#define BIGLINE 512
int lookup(Dictrec * sought, const char * resource) {
	Dictrec dr;
	static FILE * in;
	static int first_time = 1;
	size_t numRead;
	char line[BIGLINE];
	if (first_time) { 
		first_time = 0;
		/* open up the file
		 *
		 * Fill in code. */
		 if ((in =fopen(resource,"r")) == NULL){
		 	perror("open error");
		 }
	}

	/* read from top of file, looking for match
	 *
	 * Fill in code. */
	rewind(in);
	
	while(!feof(in)) {
		/* Fill in code. */
		
		/*fgets(dr.word,32,in);
		printf("word=%s\n",dr.word);
		fgets(dr.text,480,in);
	*/
		fread(&dr,sizeof(dr),1,in);
		if(strcmp(dr.word,sought->word)==0){
			strcpy(sought->text,dr.text);
			return FOUND;
			}
	/*printf("dr=%s",dr.word);
			fflush(stdout);*/

}
	return NOTFOUND;
}
