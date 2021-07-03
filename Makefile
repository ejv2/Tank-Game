SRC = main.c level.c player.c util.c inputs.c menu.c
OBJ = ${SRC:.c=.o}
SOBJ = ui/ui.o
HDR = tank.h

EXE = tank-game
SDLFLAGS = `sdl2-config --cflags --libs`

export LDFLAGS += -lSDL2_image -lSDL2_ttf -lm
export CFLAGS += -std=c99 -Wall -Wpedantic

VERSION=0.0.1

ifeq ($(DEBUG),1)
	CFLAGS += -g
else
	CFLAGS += -O2
endif


${EXE}: ${OBJ} ${SOBJ}
	${CC} -o $@ ${OBJ} ${SOBJ} ${SDLFLAGS} ${LDFLAGS}

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: ${HDR}
${SOBJ}: FORCE
	cd ui && ${MAKE}

clean:
	rm *.o
	rm ui/*.o
	rm ${EXE}

distclean:
	rm *.gz

dist: ${EXE}
	tar -cf "tank-game-${VERSION}.tar" tank-game COPYING README.md res/ levels/
	gzip tank-game-${VERSION}.tar

FORCE:

.PHONY = clean distclean dist FORCE
