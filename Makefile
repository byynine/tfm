bld/linux/main: src/linux/main.c | bld/linux
	gcc -o $@ $<

bld/linux:
	mkdir -p bld/linux

run: bld/linux/main
	bld/linux/main

clean:
	rm -rf bld/
