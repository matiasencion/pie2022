struct Clave_t {
	unsigned int valor;
	int largo;
};

int bit (unsigned int buffer, int pos_bit);

void ver_binario (unsigned int buffer, int pos_ls, int pos_ms);

unsigned int set_bit (unsigned int buffer, int pos, int valor);

unsigned int crear_mascara (int pos_ls, int pos_ms);

unsigned int concatena (unsigned int buffer_ms, unsigned int buffer_ls, int num_bits_ls);

unsigned int espejar (unsigned int buffer, int num_bits);

int paridad (unsigned int buffer);

void ver_clave (struct Clave_t clave);

struct Clave_t rotar_clave (struct Clave_t clave, unsigned int nrot);

unsigned int encriptar (unsigned int buffer, struct Clave_t clave);
