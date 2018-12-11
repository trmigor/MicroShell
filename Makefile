make:
	make MicroShell -B -s

MicroShell: main.o realizations/command.o realizations/built-in.o realizations/central.o
	g++ -fsanitize=address -o MicroShell main.o realizations/command.o realizations/built-in.o realizations/central.o

CXXFLAG=-fsanitize=address 

main.o: main.cpp
	g++ -c main.cpp

realizations/command.o: realizations/command.cpp
	g++ -c -fsanitize=address -fPIC realizations/command.cpp -o realizations/command.o

realizations/built-in.o: realizations/built-in.cpp
	g++ -c -fsanitize=address -fPIC realizations/built-in.cpp -o realizations/built-in.o

realizations/central.o: realizations/central.cpp
	g++ -c -fsanitize=address -fPIC realizations/central.cpp -o realizations/central.o

clean:
	rm -f *.o *.out MicroShell
	rm -f realizations/*.o