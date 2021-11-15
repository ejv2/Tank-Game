SRC = hud.c
OBJ = ${SRC:.c=.o}
HDR = hud.h

MOBJ = mhud.o

${MOBJ}: ${OBJ}
	${LD} -r ${OBJ} -o ${MOBJ}

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: ${HDR}
