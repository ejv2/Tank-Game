sources=main.c level.c player.c util.c inputs.c menu.c
objects=${sources:.c=.o}
headers=tank.h

exec=tank-game
sdlflags=`sdl2-config --cflags --libs`

LDFLAGS += -lSDL2_image -lm
CFLAGS += -std=c99 -Wall -Wpedantic

VERSION=0.0.1

ifeq ($(DEBUG),1)
	CFLAGS += -g
endif


${exec}: ${objects}
	${CC} -o $@ ${objects} ${sdlflags} ${LDFLAGS}

.c.o:
	${CC} -c ${CFLAGS} $<

${objects}: ${headers}

clean:
	rm *.o
	rm ${exec}
	-rm *.tar

dist: ${exec}
	tar -cf "tank-game-${VERSION}.tar" tank-game COPYING README.md res/ levels/
