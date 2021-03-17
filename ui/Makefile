sources=mm.c
objects=${sources:.c=.o}
headers=ui.h

mainObj=ui.o

${mainObj}: ${objects}
	${LD} -r ${objects} -o ${mainObj}

.c.o:
	${CC} -c ${CFLAGS} $<

${objects}: ${headers}
