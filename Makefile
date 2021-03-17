sources=main.c level.c player.c util.c inputs.c menu.c
objects=${sources:.c=.o} 
subObjects=ui/ui.o
headers=tank.h

exec=tank-game
sdlflags=`sdl2-config --cflags --libs`

export LDFLAGS += -lSDL2_image -lSDL2_ttf -lm
export CFLAGS += -std=c99 -Wall -Wpedantic

VERSION=0.0.1

ifeq ($(DEBUG),1)
	CFLAGS += -g
endif


${exec}: ${objects} ${subObjects}
	${CC} -o $@ ${objects} ${subObjects} ${sdlflags} ${LDFLAGS}

.c.o:
	${CC} -c ${CFLAGS} $<

${objects}: ${headers}
${subObjects}: FORCE
	cd ui && ${MAKE}

clean:
	rm *.o
	cd ui && rm *.o
	rm ${exec}
	-rm *.tar

dist: ${exec}
	tar -cf "tank-game-${VERSION}.tar" tank-game COPYING README.md res/ levels/

FORCE:
