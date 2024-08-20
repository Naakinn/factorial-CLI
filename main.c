#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include "fact.h"

#define DESCC 3
#define FLAGC 2
#define VERSC 2

#define EXIT_HELP 2

#define FGRED "\x1B[31m"
#define FGNORMAL "\x1B[0m"
#define FTUNDERLINE "\x1B[4m"

#define OPENMODE "w+" 


static char* flags[FLAGC][DESCC] = {
	{ "-h", "--help", "Show help page", },
	{ "-t", "--test", "Test flag, does nothing", },
};


int main(int argc, char** argv) {
	int parse_status = parse(argc, argv);
	
	switch (parse_status) {
		case EXIT_HELP: 
			help();
			goto exit_success;
			break; 
		case EXIT_FAILURE: 
			goto exit_failure; 
			break;
	}
	
	int has_opt = 0; 
	if (optind < argc) { 
		while (optind < argc) {
			if (strnumeric(argv[optind]) == 0) {
				unsigned long long res = factorial(atoll(argv[optind]));
				++has_opt;
				printf("%llu\n", res);
			}
			++optind;
		}
	} 	
	if (!has_opt) {
		throw_error();
	}
	exit_success:
	exit(EXIT_SUCCESS);

	exit_failure:
	exit(EXIT_FAILURE);
}

int parse(const int argc, char* argv[]) {
	int STATUS = 0;
	int c, i;
	
	static char* short_options = "ht";
	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ 0, 	  0, 		   0, 0 }
	};

	
	while(1) {
		int option_index = 0; 

		c = getopt_long(argc, argv, short_options, long_options, &option_index);

		if (c == -1) {
			break; 
		}

		switch (c) {
			case 'h': 
				STATUS = EXIT_HELP;
				break;
			case 't':
				printf("test option");
				break;
			case '?':
				STATUS = EXIT_FAILURE; 
				break;
			default: 
				printf("?? getopt returned character code 0x%x ??\n", c);
				STATUS = EXIT_FAILURE;
				break; 
		}
	}
	return STATUS;
}

unsigned long long factorial(unsigned long long number) {
	unsigned long long res = 1; 
	for (; number > 1; number--) {
		res *= number;
	}
	return res;
}

void help(void) {
	printf("fact - Get factorial of an unsigned(positive) integer\n");
	putchar('\n');
	printf("Usage: fact [OPTION...] [INTEGER]\n");
	putchar('\n');

	for (int i = 0; i < FLAGC; ++i) {
		printf("\t%s %s \t%s\n", flags[i][0], flags[i][1], flags[i][2]);
	}
}

int strnumeric(const char *str) {
	for (int i = 0; str[i] != '\0'; ++i) {
		if (!isdigit(str[i])) {
			return -1; 
		}
	}
	return 0;
}

void throw_error(void) {
	printf("%serror: %s", FGRED, FGNORMAL);
	printf("expected unsigned integer (see: %sfact --help%s)\n", FTUNDERLINE, FGNORMAL);
}
