default:
	gcc src/shellred.c -o bin/shell -lreadline; gcc src/tee.c -o bin/tee; gcc src/wc.c -o bin/wc; gcc src/yes.c -o bin/yes; ./bin/shell
build:
	gcc src/shellred.c -o bin/shell -lreadline; gcc src/tee.c -o bin/tee; gcc src/wc.c -o bin/wc; gcc src/yes.c -o bin/yes
run:
	./bin/shell
