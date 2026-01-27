bld/main: src/main.c | bld
	gcc -o $@ $<

bld:
	mkdir -p bld

run:
	@bld/main

clean:
	rm -rf bld
