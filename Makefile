exec = a.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g -lm -ldl -fPIC -rdynamic


%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

libobjload.a: $(objects)
	ar rcs $@ $^

install:
	make
	make libobjload.a
	mkdir -p /usr/local/include/objload
	cp -r ./src/include/* /usr/local/include/objload/.
	cp ./libobjload.a /usr/local/lib/.

clean:
	-rm *.out
	-rm *.o
	-rm *.a
	-rm src/*.o
