SRC = mm.c loading.c test.c
OBJ = ${SRC:.c=.o}
HDR = ui.h

MOBJ = ui.o

${MOBJ}: ${OBJ}
	${LD} -r ${OBJ} -o ${MOBJ}

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: ${HDR}
