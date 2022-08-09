#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "imagen.h"

int main (int argc, char* argv[]) {
	if (strcmp (argv[1], "convertir_formato") == 0) {
		FormatoPPM_t formato;
		struct Imagen_t mpin;
		struct Imagen_t* pin = &mpin;
		if (strcmp (argv[4], "plano") == 0)
			formato = 0;
		else 
			formato = 1;
		leer_imagen (argv[2], pin);
		escribir_imagen (pin, argv[3], formato);
		destruir_imagen (pin);
		return 0;
	};
	if (strcmp (argv[1], "filtrar_sepia") == 0) {
		struct Imagen_t mpin;
		struct Imagen_t* pin = &mpin;
		struct Imagen_t mpout;
		struct Imagen_t* pout = &mpout;
		FormatoPPM_t formato;
		if (strcmp (argv[4], "plano") == 0)
			formato = 0;
		else 
			formato = 1;
		leer_imagen (argv[2], pin);
		filtrar_sepia (pin, pout);
		escribir_imagen (pout, argv [3], formato);
		destruir_imagen (pin);
		destruir_imagen (pout);
		return 0;
	};
	if (strcmp (argv[1], "generar_cripto_imagen") == 0) {
		struct Imagen_t mpcriptoim;
		struct Imagen_t* pcriptoim = &mpcriptoim;
		FormatoPPM_t formato;
		if (strcmp (argv[6], "plano") == 0)
			formato = 0;
		else 
			formato = 1;
		generar_cripto_imagen (pcriptoim, atoi (argv[2]), atoi (argv[3]), atoi (argv[4]));
		escribir_imagen (pcriptoim, argv[5], formato);
		destruir_imagen (pcriptoim);
		return 0;
	};
	if (strcmp (argv[1], "validar_cripto_imagen") == 0) {
		struct Imagen_t mpcriptoim;
		struct Imagen_t* pcriptoim = &mpcriptoim;
		leer_imagen (argv[2], pcriptoim);
		if (validar_cripto_imagen (pcriptoim) == 0) {
			printf ("Es una cripto imagen\n");
			destruir_imagen (pcriptoim);
			return 0;
		}
		else {
			fprintf (stderr, "Cripto imagen no valida\n");
			destruir_imagen (pcriptoim);
			return 1;
		};
	};
	
	if (strcmp (argv[1], "encriptar_imagen") == 0) {
		struct Imagen_t mpcriptoim;
		struct Imagen_t* pcriptoim = &mpcriptoim;
		leer_imagen (argv[3], pcriptoim);
		if (validar_cripto_imagen (pcriptoim) == 0) {
			FormatoPPM_t formato;
			struct Imagen_t mpin;
			struct Imagen_t* pin = &mpin;
			struct Imagen_t mpout;
			struct Imagen_t* pout = &mpout;
			if (strcmp (argv[5], "plano") == 0)
				formato = 0;
			else 
				formato = 1;
			leer_imagen (argv[2], pin);
			encriptar_imagen (pin, pcriptoim, pout);
			escribir_imagen (pout, argv[4], formato);
			destruir_imagen (pin);
			destruir_imagen (pout);
			destruir_imagen (pcriptoim);
			return 0;
		}
		else {
			destruir_imagen (pcriptoim);
			fprintf(stderr, "Cripto imagen no valida\n");
			return 1;
		};
	};
}
