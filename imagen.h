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

int min (int x, int y);

CodigoError_t inicializar_imagen (struct Imagen_t* pin, int filas, int columnas);

CodigoError_t destruir_imagen (struct Imagen_t* pin);

CodigoError_t duplicar_imagen (const struct Imagen_t* pin, struct Imagen_t* pout);

CodigoError_t leer_imagen (const char* ruta_imagen, struct Imagen_t* pin);

CodigoError_t escribir_imagen (const struct Imagen_t* pin, const char* ruta_imagen, FormatoPPM_t formato);

CodigoError_t filtrar_sepia (const struct Imagen_t* pin, struct Imagen_t* pout);

CodigoError_t generar_cripto_imagen (struct Imagen_t* pcriptoim, int filas, int columnas, int min_largo_clave);

CodigoError_t validar_cripto_imagen (const struct Imagen_t* pcriptoim);

CodigoError_t encriptar_imagen (const struct Imagen_t* pin, const struct Imagen_t* pcriptoim, struct Imagen_t* pout);



