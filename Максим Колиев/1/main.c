#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

	if(argc < 2) {
		printf("Incorrect Input\nPlease, ./a.out \"Your String\"\n");
		exit(1);
	}

	int len = strlen(argv[1]);
	char cur;
	int flag = 0;
	
			
	while(EOF != scanf("%c", &cur)) {
		
		if (flag == 0 && cur == argv[1][flag])
			flag++;
		else
			if (cur == argv[1][flag])
				flag++;
	
		printf("%c", cur);	
	
		if (flag == len) {
		       printf("*");
		       flag = 0;
	     	}	       
	} 

	return 0;
}
