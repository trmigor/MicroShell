a.out: main.o libmyshell.so
	g++ -o a.out main.o -L. -lmyshell -Wl,-rpath,.
	./a.out

main.o: main.cpp
	g++ -c main.cpp

libmyshell.so: realizations/command.o realizations/built-in.o realizations/central.o
	g++ -shared -o libmyshell.so realizations/command.o realizations/built-in.o realizations/central.o

realizations/command.o: realizations/command.cpp
	g++ -c -fPIC realizations/command.cpp -o realizations/command.o

realizations/built-in.o: realizations/built-in.cpp
	g++ -c -fPIC realizations/built-in.cpp -o realizations/built-in.o

realizations/central.o: realizations/central.cpp
	g++ -c -fPIC realizations/central.cpp -o realizations/central.o

fullclean:
	rm -f *.o *.out *.so
	rm -f realizations/*.o

clean: 
	rm -f *.o *.out