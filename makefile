doodle: doodle.o gamemechanics.o ./GfxLib/libisentlib.a
	gcc -Wall $^ -o $@ -lm -lglut -lGL -lX11

doodle.o: doodle.c ./GfxLib/GfxLib.h ./GfxLib/BmpLib.h ./GfxLib/ESLib.h
	gcc -Wall -c $< -O2

gamemechanics.o: gamemechanics.c 
	gcc -Wall -c $< -O2

gfx:
	cd ./GfxLib; make;

clean:
	(rm -f *~ *.o; cd ./GfxLib; make clean)

deepclean: 
	(rm -f doodle; cd ./GfxLib; make clean; rm -f libisentlib.a)