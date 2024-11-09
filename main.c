#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define MAX 1000


typedef struct {
	int size;
	char array[MAX];
} Carr;


typedef struct {
	int size;
	char *array[MAX];
} uCarr;


uCarr extend_ucarr(uCarr ucarr, char arr[]) {
	uCarr extended_ucarr;
	extended_ucarr.size = 0;

	int i = 0;

	for (i;i < ucarr.size;i++) {
		extended_ucarr.size += 1;
		extended_ucarr.array[i] = ucarr.array[i];
	}

	extended_ucarr.array[i + 1] = arr;
	extended_ucarr.size += 1;

	return extended_ucarr;
}


Carr extend_carr(Carr carr, char c) {
	Carr extended_carr;
	extended_carr.size = 0;

	int i = 0;

	for (i;i < carr.size;i++) {
		extended_carr.size += 1;
		extended_carr.array[i] = carr.array[i];
	}

	extended_carr.array[i + 1] = c;
	extended_carr.size += 1;

	return extended_carr;
}


char* carr_to_uk(Carr carr) {
	char arr[carr.size];

	for (int i = 0; i < carr.size; i++) {
		arr[i] = carr.array[i];
	}

	char *ustr = arr;

	return ustr;	
}


int start_programm(char *argv[]) {
	pid_t pid;

	pid = fork();

	if (pid == 0) {
		execvp(argv[0], argv);
		perror("CHILD PROCESS ERROR");
	}

	return 0;
}


int main(int argc, char *argv[]) {
	FILE *file;
	int c, i;

	file = fopen(argv[1], "r");

	if (file == NULL) {
		printf("ERROR: Can not open configuration file\n");
		return 0;
	}
	
	i = 0;

	uCarr stprc;
	stprc.size = 0;
	Carr act;
	act.size = 0;
	
	while ((c = fgetc(file)) != EOF) {
		if ((i == 0 || i == 1) && c != ' ') {
			act = extend_carr(act, c);
			printf("%d\n", act.size);
		} else if ((i == 0 || i == 1) && c == ' ') {
			char *ustr = carr_to_uk(act);
			printf("%s\n", ustr);
			stprc = extend_ucarr(stprc, ustr);
			Carr new_carr;
			new_carr.size = 0;
			act = new_carr;
			i++;
		} else if (i == 2 && c != '\n') {
			act = extend_carr(act, c);
		} else if (i == 2 && c == '\n') {
			char *ustr = carr_to_uk(act);
			stprc = extend_ucarr(stprc, ustr);
			Carr new_carr;
			new_carr.size = 0;
			act = new_carr;
			i = 0;
			start_programm(stprc.array);
			uCarr new_ucarr;
			new_ucarr.size = 0;
			stprc = new_ucarr;
		} else {
			printf("ERROR: incorrect configuration");
		}
	}

	fclose(file);

	return 0;
}
