#include <stdio.h>

struct Clave_t {
	unsigned int valor;
	int largo;
};

int bit (unsigned int buffer, int pos_bit) {
	buffer = buffer >> pos_bit;
	return (buffer & 1);
};

void ver_binario (unsigned int buffer, int pos_ls, int pos_ms) {
	int i;
	unsigned int aux;
	for (i = pos_ms;i >= pos_ls;i--) {
		aux = buffer >> i;
		printf ("%d", aux & 1);
	};
	printf ("\n");
};

unsigned int set_bit (unsigned int buffer, int pos, int valor) {
	unsigned int aux = 1;
	if (bit (buffer, pos) != valor) {
		aux = aux << pos;
		return (buffer ^ aux);
	} else
		return buffer;
};

unsigned int crear_mascara (int pos_ls, int pos_ms) {
	int i;
	unsigned int aux = 0;
	for (i = pos_ls; i <= pos_ms; i++)
		aux = set_bit (aux, i, 1);
	return aux;
};

unsigned int concatena (unsigned int buffer_ms, unsigned int buffer_ls, int num_bits_ls) {
	buffer_ms = buffer_ms << num_bits_ls;
	buffer_ls = crear_mascara (0, num_bits_ls-1) & buffer_ls;
	return (buffer_ms | buffer_ls);
};

unsigned int espejar (unsigned int buffer, int num_bits) {
	int i;
	unsigned int aux = 0;
	for (i = 0;i < num_bits;i++)
		aux = set_bit (aux, i, bit (buffer, num_bits-i-1));
	return (aux);
};

int paridad (unsigned int buffer) {
	int i;
	int aux = 0;
	for (i = 0;i <= 31;i++)
		if (bit (buffer, i) == 1)
			aux = aux + 1;
	return ((aux % 2)*-1+1);
};
			
void ver_clave (struct Clave_t clave) {
	ver_binario (clave.valor, 0, clave.largo-1);
};

struct Clave_t rotar_clave (struct Clave_t clave, unsigned int nrot) {
	if (nrot % clave.largo == 0 || nrot == 0)
			return clave;
	else {
		int i;
		struct Clave_t aux = clave;
		aux.valor = aux.valor << (nrot % clave.largo);
		for (i = 0;i < (nrot % clave.largo);i++) 
			aux.valor = set_bit (aux.valor, i, bit (aux.valor, clave.largo + i));
		aux.valor = aux.valor & crear_mascara (0, clave.largo-1);
		return aux;
	};
};

unsigned int encriptar (unsigned int buffer, struct Clave_t clave) {
	int cociente = 32 / clave.largo;
	int i;
	unsigned int aux = 0;
	for (i = cociente;i > 0;i--) 
		aux = aux | (clave.valor << (clave.largo * (i - 1) + 32 % clave.largo));
	aux = aux | clave.valor >> (clave.largo - 32 % clave.largo);
	return (buffer ^ aux);
};
