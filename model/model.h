#ifndef MODEL_H
#define MODEL_H

// =========================
// SafeSpace TinyML Model
// =========================

#define TEMP_MIN_SEGURA 18
#define TEMP_MAX_SEGURA 30

#define UMIDADE_MIN_SEGURA 50

#define LUZ_MIN_SEGURA 3000

#define TEMP_CRITICA_ALTA 40
#define TEMP_CRITICA_BAIXA 5

#define UMIDADE_CRITICA 20

#define LUZ_CRITICA 1000

enum EstadoHabitat {
  SEGURO,
  ALERTA,
  CRITICO
};

#endif