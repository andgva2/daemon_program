all: ./bin/daemon_program
.PHONY: clean
clean:
	rm -r -f ./bin/ ./obj/
./bin/daemon_program: ./obj/daemon_program.o ./obj/become_daemon.o
	mkdir -p ./bin/
	gcc -I./src/ -o ./bin/daemon_program ./obj/daemon_program.o ./obj/become_daemon.o

./obj/daemon_program.o: ./src/daemon_program.c
	mkdir -p ./obj/
	gcc -I./src/ -c ./src/daemon_program.c -o ./obj/daemon_program.o

./obj/become_daemon.o: ./src/become_daemon.c
	mkdir -p ./obj/
	gcc -I./src/ -c ./src/become_daemon.c -o ./obj/become_daemon.o
