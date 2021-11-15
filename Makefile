SRC = main.c level.c player.c util.c inputs.c menu.c
OBJ = ${SRC:.c=.o}
UOBJ = ui/ui.o
HOBJ = hud/mhud.o
SOBJ = ${UOBJ} ${HOBJ}
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

${EXE}: ${OBJ} ${UOBJ} ${HOBJ}
	${CC} -o $@ ${OBJ} ${SOBJ} ${SDLFLAGS} ${LDFLAGS}

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: ${HDR}
${UOBJ}: FORCE
	${MAKE} -C ui
${HOBJ}: FORCE
	${MAKE} -C hud

clean:
	rm *.o
	rm ui/*.o
	rm hud/*.o
	rm ${EXE}

distclean:
	rm *.gz

dist: ${EXE}
	tar -cf "tank-game-${VERSION}.tar" tank-game COPYING README.md res/ levels/
	gzip tank-game-${VERSION}.tar

FORCE:

.PHONY = clean distclean dist FORCE
