#ifndef CUBALC_H
#define CUBALC_H
#include "cubalc_law.h"
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

/* Packed bit matrix — SoT for one Cube */
typedef struct cubalc_matrix {
  uint16_t n;     /* bit count (≤ CUBALC_ATOM_BITS) */
  uint16_t set;
  uint8_t  bits[(CUBALC_ATOM_BITS + 7) / 8];
} cubalc_matrix;

/* Nanobot atom core inside every Cube */
typedef struct cubalc_atom {
  char     id[CUBALC_ID_LEN];
  uint8_t  proton;      /* 1 create · 0 destroy */
  uint8_t  alive;
  uint8_t  digit;       /* algocube 0–9 */
  float    unity;
  cubalc_matrix matrix; /* inner state — compatibility key */
} cubalc_atom;

/* Plug port — LEGO stud: other cubes snap if matrices compatible */
typedef struct cubalc_port {
  uint8_t  open;
  int16_t  peer;        /* index in chain or -1 */
  uint8_t  face;        /* 0+X 1-X 2+Y 3-Y 4+Z 5-Z */
  cubalc_matrix gate;   /* required mask for plug */
} cubalc_port;

/* Visible Cube object (OOP instance in VR) */
typedef struct cubalc_cube {
  char     id[CUBALC_ID_LEN];
  char     label[CUBALC_ID_LEN];
  char     role[24];    /* os aspect: kernel|llama|quest|wivrn|kinect|coord|host|… */
  float    x, y, z, s;
  uint8_t  r, g, b, a;
  cubalc_atom atom;
  cubalc_port ports[CUBALC_MAX_PORTS];
  int      n_ports;
  uint8_t  plugged;     /* count of live plugs */
} cubalc_cube;

/* Cube Chain — LEGO set from Initial State Matrix */
typedef struct cubalc_chain {
  cubalc_matrix initial; /* genesis matrix — beginning of chain */
  cubalc_cube cubes[CUBALC_MAX_CUBES];
  int n_cubes;
  uint32_t seq;
  float unity;
  uint8_t hold_flash;
  char creed[80];
  char status[160];
} cubalc_chain;

/* Binary transfer frame (cube talk) */
typedef struct cubalc_bin_hdr {
  uint32_t magic;
  uint16_t proto;
  uint16_t n_bits;
  uint16_t set;
  uint8_t  proton;
  uint8_t  digit;
  uint32_t seq;
  char     from_id[CUBALC_ID_LEN];
  char     to_id[CUBALC_ID_LEN];
} cubalc_bin_hdr;

/* matrix ops */
void cubalc_matrix_clear(cubalc_matrix *m);
void cubalc_matrix_from_ascii(cubalc_matrix *m, const char *bits01, int n);
int  cubalc_matrix_get(const cubalc_matrix *m, int i);
void cubalc_matrix_set(cubalc_matrix *m, int i, int on);
int  cubalc_matrix_popcount(const cubalc_matrix *m);
int  cubalc_matrix_hamming(const cubalc_matrix *a, const cubalc_matrix *b);
float cubalc_matrix_compat(const cubalc_matrix *a, const cubalc_matrix *b); /* 0..1 */

/* atom / proton */
void cubalc_atom_init(cubalc_atom *a, const char *id, uint8_t proton);
int  cubalc_atom_impulse(cubalc_atom *a, uint8_t proton); /* create=1 destroy=0 */
int  cubalc_algocube_digit(const cubalc_matrix *m);

/* cube OOP */
int  cubalc_cube_spawn(cubalc_chain *ch, const char *id, const char *role,
                       uint8_t proton, float x, float y, float z);
int  cubalc_cube_plug(cubalc_chain *ch, int a, int b); /* plug if matrices compatible */
int  cubalc_cube_unplug(cubalc_chain *ch, int a, int b);
int  cubalc_cube_talk(cubalc_chain *ch, int from, int to); /* binary matrix transfer */

/* binary pack/unpack */
int  cubalc_bin_pack(const cubalc_atom *atom, const char *from, const char *to,
                     uint32_t seq, uint8_t *out, size_t cap, size_t *n_out);
int  cubalc_bin_unpack(const uint8_t *in, size_t n, cubalc_atom *atom_out,
                       char *from, char *to, uint32_t *seq);

/* chain / genesis */
void cubalc_chain_init(cubalc_chain *ch);
int  cubalc_chain_from_initial(cubalc_chain *ch, const cubalc_matrix *genesis, uint32_t seq);
int  cubalc_chain_os_aspects(cubalc_chain *ch); /* spawn OS viz cubes from live probes */
int  cubalc_chain_tick(cubalc_chain *ch);
int  cubalc_chain_write_viz(const cubalc_chain *ch, const char *path);
int  cubalc_chain_write_json(const cubalc_chain *ch, const char *path);
int  cubalc_chain_impulse(cubalc_chain *ch, const char *cube_id, uint8_t proton);

/* NEXUS_COORD fold into genesis bit pattern */
int  cubalc_coord_to_matrix(const char *plate_line, cubalc_matrix *out);

#ifdef __cplusplus
}
#endif
#endif
