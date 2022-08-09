#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "bits.h"

typedef unsigned int pixel_t;

struct Imagen_t {
	int filas;
	int columnas;
	pixel_t** pixeles;
};

enum formato_ppm {PLANO, NO_PLANO};

typedef enum formato_ppm FormatoPPM_t;

enum codigo_error {OK, PPM_ARCHIVO_INEXISTENTE, PPM_ERROR_LECTURA, PPM_ENCABEZADO_INVALIDO, PPM_DATOS_INVALIDOS, PPM_ERROR_ESCRITURA, PPM_CRIPTO_NO_VALIDA, ERROR};

typedef enum codigo_error CodigoError_t;

unsigned int min (unsigned int x, unsigned int y) {
	if (x == y)
		return x;
	if (x > y)
		return y;
	else 
		return x;
};

CodigoError_t inicializar_imagen (struct Imagen_t* pin, int filas, int columnas) {
	int i;
	(*pin).filas = filas;
	(*pin).columnas = columnas;
	(*pin).pixeles = (unsigned int**) malloc (filas * sizeof (unsigned int**));
	for (i = 0;i < filas; i++) 
		*((*pin).pixeles + i) = (unsigned int*) malloc (columnas * sizeof (unsigned int*));
	return 0;
};

CodigoError_t destruir_imagen (struct Imagen_t* pin) {
	int i;
	for (i = ((*pin).filas - 1);i >= 0; i--)
		free (*((*pin).pixeles + i));
	free ((*pin).pixeles);
	(*pin).filas = 0;
	(*pin).columnas = 0;
	return 0;
}; 

CodigoError_t duplicar_imagen (const struct Imagen_t* pin, struct Imagen_t* pout) {
	inicializar_imagen (pout, (*pin).filas, (*pin).columnas);
	return 0;
};

CodigoError_t leer_imagen (const char* ruta_imagen, struct Imagen_t* pin) {
	FILE *fp;
	if ((fp = fopen (ruta_imagen, "r")) == NULL) 
		return 1; 
	char tipo[2];
	int ancho;
	int alto;
	int max;
	int error;
	error = fscanf (fp, "%s %d %d %d", tipo, &ancho, &alto, &max);
	if (error == 0)
		return 2;
	else { 
		if (strcmp (tipo, "P3") == 0) {
			inicializar_imagen (pin, alto, ancho);
			int i, j;
			unsigned int r, g, b;
			for (i = 0;i < alto;i++) {
				for (j = 0;j < ancho;j++) {
					error = fscanf (fp, "%d %d %d", &r, &g, &b);
					if (error == 0)
						return 2;
					else {
						if (error < 3)
							return 4;
						else
							*(*((*pin).pixeles + i) + j) = concatena (concatena (r, g, 8), b, 8);
					};
				};
			};
		}
		else {
			if (strcmp (tipo, "P6") == 0) {
				inicializar_imagen (pin, alto, ancho);
				int i, j;
				unsigned int r, g, b;
				fgetc(fp);
				for (i = 0;i < alto;i++) {
					for (j = 0;j < ancho;j++) {
						if ((r = fgetc(fp)) == EOF)
							return 4;
						if ((g = fgetc(fp)) == EOF)
							return 4;
						if ((b = fgetc(fp)) == EOF)
							return 4;
						*(*((*pin).pixeles + i) + j) = concatena (concatena (r, g, 8), b, 8);
					};
				};
			}
			else 
				return 3;
		};
	};
	fclose (fp);
	return 0;
};
	
CodigoError_t escribir_imagen (const struct Imagen_t* pin, const char* ruta_imagen, FormatoPPM_t formato) {
	FILE *fp;
	if ((fp = fopen (ruta_imagen, "w")) == NULL) 
		return 5;
	int i, j;
	if (formato == 0) {
		if (fprintf (fp,"%s %d %d %d ", "P3", (*pin).columnas, (*pin).filas, 255) < 0)
			return 5;
		for (i = 0;i < (*pin).filas;i++) {
			for (j = 0;j < (*pin).columnas;j++) {
				if (fprintf (fp,"% d% d% d", *(*((*pin).pixeles + i) + j) >> 16, (*(*((*pin).pixeles + i) + j) >> 8) & crear_mascara (0, 7), *(*((*pin).pixeles + i) + j) & crear_mascara (0,7)) < 0)
					return 5;
			};
		};
	}
	else {
		if (fprintf (fp,"%s %d %d %d ", "P6", (*pin).columnas, (*pin).filas, 255) < 0)
			return 5;
		for (i = 0;i < (*pin).filas;i++) {
			for (j = 0;j < (*pin).columnas;j++) {
				if (fprintf (fp,"%c%c%c", *(*((*pin).pixeles + i) + j) >> 16, (*(*((*pin).pixeles + i) + j) >> 8) & crear_mascara (0, 7), *(*((*pin).pixeles + i) + j) & crear_mascara (0,7)) < 0)
					return 5;
			};
		};
	};
	fclose (fp);
	return 0;
};

CodigoError_t filtrar_sepia (const struct Imagen_t* pin, struct Imagen_t* pout) {
	unsigned int i, j, rin, rout, gin, gout, bin, bout;
	inicializar_imagen (pout, (*pin).filas, (*pin).columnas);
	for (i = 0;i < (*pin).filas;i++) {
		for (j = 0;j < (*pin).columnas;j++) {
			rin = *(*((*pin).pixeles + i) + j) >> 16;
			gin = (*(*((*pin).pixeles + i) + j) >> 8) & crear_mascara (0, 7);
			bin = *(*((*pin).pixeles + i) + j) & crear_mascara (0, 7);
			rout = min (round (0.393 * rin + 0.769 * gin + 0.189 * bin), 255);
			gout = min (round (0.349 * rin + 0.686 * gin + 0.168 * bin), 255);
			bout = min (round (0.272 * rin + 0.534 * gin + 0.131 * bin), 255);
			*(*((*pout).pixeles + i) + j) = concatena (concatena (rout, gout, 8), bout, 8);
		};
	};
	return 0;
};	

CodigoError_t generar_cripto_imagen (struct Imagen_t* pcriptoim, int filas, int columnas, int min_largo_clave) {
	unsigned int aux, largo;
	int i, j;
	unsigned int menor = set_bit (0, min_largo_clave - 1, 1);
	inicializar_imagen (pcriptoim, filas, columnas);
	for (i = 0;i < filas;i++) {
		for (j = 0;j < columnas;j++) {
			aux = (rand() % (crear_mascara (0, 18) - menor + 1) + menor);
			for (largo = 32;bit (aux, largo - 1) != 1;largo--);
			*(*((*pcriptoim).pixeles + i) + j) = concatena (largo, aux, 19);
		};
	};
	return 0;
};
		
CodigoError_t validar_cripto_imagen (const struct Imagen_t* pcriptoim) {
	int i, j;
	for (i = 0;i < (*pcriptoim).filas;i++) {
		for (j = 0;j < (*pcriptoim).columnas;j++) {
			if (((*(*((*pcriptoim).pixeles + i) + j) >> 19) > 19) || ((*(*((*pcriptoim).pixeles + i) + j) >> 19) < 1))
				return  6;
		};
	};
	return 0;
};

CodigoError_t encriptar_imagen (const struct Imagen_t* pin, const struct Imagen_t* pcriptoim, struct Imagen_t* pout) {
	if (validar_cripto_imagen (pcriptoim) == 6)
		return 6;
	int i, j;
	struct Clave_t clave;
	inicializar_imagen (pout, (*pin).filas, (*pin).columnas);
	if (((*pcriptoim).columnas >= (*pin).columnas) && ((*pcriptoim).filas >= (*pin).filas)) {
		for (i = 0;i < (*pin).filas;i++) {
			for (j = 0;j < (*pin).columnas;j++) {
				clave.largo = (*(*((*pcriptoim).pixeles + i) + j) >> 19) & crear_mascara (0, 4);
				clave.valor = *(*((*pcriptoim).pixeles + i) + j) & crear_mascara (0, clave.largo - 1);
				*(*((*pout).pixeles + i) + j) = encriptar (*(*((*pin).pixeles + i) + j), clave);
			};
		};
	}
	else {
		for (i = 0;i < (*pin).filas;i++) {
			for(j = 0;j < (*pin).columnas;j++) {
				clave.largo = (*(*((*pcriptoim).pixeles + (i % (*pcriptoim).filas)) + (j % (*pcriptoim).columnas)) >> 19) & crear_mascara (0, 4);
				clave.valor = *(*((*pcriptoim).pixeles + (i % (*pcriptoim).filas)) + (j % (*pcriptoim).columnas)) & crear_mascara (0, clave.largo - 1);
				*(*((*pout).pixeles + i) + j) = encriptar (*(*((*pin).pixeles + i) + j), clave);
			};
		};
	};
	return 0;
};
