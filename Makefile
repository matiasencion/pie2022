# --------------------------------------------------------------------
#  Makefile de ejemplo del obligatorio 2.
#  Progrmacion Para Ingenieria Electrica
# --------------------------------------------------------------------

# Define una constante que tiene las banderas de compilacion
COMOPT=-Wall -std=c99 -ggdb -Werror
### -Wall    : muestra todos los warnings
### -std=c99 : utiliza el estandar c99
### -ggbd    : permite utilizar los ejecutables con el debugger
### -Werror  : todos los warnings son errores


# Genera el ejecutable
obligatorio2: obligatorio2.o libbits.a libimagen.a
		gcc $(COMOPT) -o obligatorio2 obligatorio2.o -L./  -limagen -lbits -lm

# Compila bits.c a codigo objeto
bits.o: bits.c
		gcc $(COMOPT) -o bits.o -c bits.c

# Compila imagen.c a codigo objeto
imagen.o: imagen.c
		gcc $(COMOPT) -o imagen.o -c imagen.c

# Compila obligatorio2. c a codigo objeto
obligatorio2.o: obligatorio2.c
		gcc $(COMOPT) -o obligatorio2.o -c obligatorio2.c

# Crea la biblioteca libbits
libbits.a: bits.o
		ar rcs libbits.a bits.o

# Crea la biblioteca libimagen
libimagen.a: imagen.o
		ar rcs libimagen.a imagen.o

# Borra los archivos (si existen) que son producto de la compilacion 
clean:
		rm -f *.a *.o obligatorio2
