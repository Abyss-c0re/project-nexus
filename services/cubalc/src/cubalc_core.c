#define _POSIX_C_SOURCE 200809L
#include "cubalc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void cubalc_matrix_clear(cubalc_matrix *m) {
  if (!m) return;
  memset(m, 0, sizeof(*m));
}

void cubalc_matrix_from_ascii(cubalc_matrix *m, const char *bits01, int n) {
  cubalc_matrix_clear(m);
  if (!bits01 || n <= 0) return;
  if (n > CUBALC_ATOM_BITS) n = CUBALC_ATOM_BITS;
  m->n = (uint16_t)n;
  for (int i = 0; i < n; i++) {
    if (bits01[i] == '1') {
      m->bits[i >> 3] |= (uint8_t)(1u << (i & 7));
      m->set++;
    }
  }
}

int cubalc_matrix_get(const cubalc_matrix *m, int i) {
  if (!m || i < 0 || i >= m->n) return 0;
  return (m->bits[i >> 3] >> (i & 7)) & 1;
}

void cubalc_matrix_set(cubalc_matrix *m, int i, int on) {
  if (!m || i < 0 || i >= CUBALC_ATOM_BITS) return;
  if (i >= m->n) m->n = (uint16_t)(i + 1);
  int was = cubalc_matrix_get(m, i);
  if (on) m->bits[i >> 3] |= (uint8_t)(1u << (i & 7));
  else    m->bits[i >> 3] &= (uint8_t)~(1u << (i & 7));
  if (on && !was) m->set++;
  else if (!on && was && m->set) m->set--;
}

int cubalc_matrix_popcount(const cubalc_matrix *m) {
  return m ? m->set : 0;
}

int cubalc_matrix_hamming(const cubalc_matrix *a, const cubalc_matrix *b) {
  if (!a || !b) return CUBALC_ATOM_BITS;
  int n = a->n > b->n ? a->n : b->n;
  if (n > CUBALC_ATOM_BITS) n = CUBALC_ATOM_BITS;
  int h = 0;
  for (int i = 0; i < n; i++)
    if (cubalc_matrix_get(a, i) != cubalc_matrix_get(b, i)) h++;
  return h;
}

float cubalc_matrix_compat(const cubalc_matrix *a, const cubalc_matrix *b) {
  if (!a || !b || a->n == 0 || b->n == 0) return 0.f;
  int n = a->n < b->n ? a->n : b->n;
  if (n <= 0) return 0.f;
  int same = 0, both = 0;
  for (int i = 0; i < n; i++) {
    int x = cubalc_matrix_get(a, i), y = cubalc_matrix_get(b, i);
    if (x == y) same++;
    if (x || y) both++;
  }
  /* compatibility: shared pattern density */
  float base = (float)same / (float)n;
  float bond = both ? (float)(n - cubalc_matrix_hamming(a, b)) / (float)n : base;
  return base * 0.5f + bond * 0.5f;
}

int cubalc_algocube_digit(const cubalc_matrix *m) {
  if (!m || m->n == 0) return 4;
  uint32_t rng = 0xC0BEA160u, set = m->set;
  for (int i = 0; i < m->n; i++)
    if (cubalc_matrix_get(m, i)) rng ^= (uint32_t)(i + 1) * 0x9E3779B9u;
  rng ^= set * 16777619u;
  rng ^= rng << 13; rng ^= rng >> 17; rng ^= rng << 5;
  return (int)(rng % 10u);
}

void cubalc_atom_init(cubalc_atom *a, const char *id, uint8_t proton) {
  if (!a) return;
  memset(a, 0, sizeof(*a));
  if (id) {
    strncpy(a->id, id, CUBALC_ID_LEN - 1);
  }
  a->proton = proton ? 1 : 0;
  a->alive = proton ? 1 : 0;
  a->unity = 1.f;
  a->matrix.n = CUBALC_ATOM_BITS;
  a->digit = cubalc_algocube_digit(&a->matrix);
}

int cubalc_atom_impulse(cubalc_atom *a, uint8_t proton) {
  if (!a) return -1;
  a->proton = proton ? 1 : 0;
  if (proton) {
    a->alive = 1;
    /* creation: set a pulse bit pattern from digit */
    for (int i = 0; i < 8; i++)
      cubalc_matrix_set(&a->matrix, (a->digit * 3 + i) % CUBALC_ATOM_BITS, 1);
  } else {
    /* destroy: clear half the matrix (deconstruct edge) */
    for (int i = 0; i < a->matrix.n; i += 2)
      cubalc_matrix_set(&a->matrix, i, 0);
    if (a->matrix.set == 0) a->alive = 0;
  }
  a->digit = cubalc_algocube_digit(&a->matrix);
  return 0;
}

static void role_color(const char *role, uint8_t *r, uint8_t *g, uint8_t *b) {
  *r = 242; *g = 38; *b = 71; /* default crimson */
  if (!role) return;
  if (strstr(role, "kernel") || strstr(role, "sot")) { *r=255; *g=165; *b=46; }
  else if (strstr(role, "llama")) { *r=64; *g=242; *b=115; }
  else if (strstr(role, "quest") || strstr(role, "lizard")) { *r=90; *g=200; *b=255; }
  else if (strstr(role, "wivrn")) { *r=180; *g=120; *b=255; }
  else if (strstr(role, "kinect")) { *r=255; *g=200; *b=80; }
  else if (strstr(role, "host") || strstr(role, "station")) { *r=242; *g=38; *b=71; }
  else if (strstr(role, "coord")) { *r=255; *g=100; *b=160; }
  else if (strstr(role, "destroy") || strstr(role, "decon")) { *r=30; *g=30; *b=36; }
  else if (strstr(role, "create") || strstr(role, "construct")) { *r=255; *g=60; *b=80; }
}

int cubalc_cube_spawn(cubalc_chain *ch, const char *id, const char *role,
                      uint8_t proton, float x, float y, float z) {
  if (!ch || ch->n_cubes >= CUBALC_MAX_CUBES || ch->n_cubes >= CUBALC_BUDGET) return -1;
  cubalc_cube *c = &ch->cubes[ch->n_cubes];
  memset(c, 0, sizeof(*c));
  strncpy(c->id, id ? id : "cube", CUBALC_ID_LEN - 1);
  strncpy(c->label, id ? id : "cube", CUBALC_ID_LEN - 1);
  strncpy(c->role, role ? role : "aspect", sizeof(c->role) - 1);
  c->x = x; c->y = y; c->z = z; c->s = 0.12f;
  role_color(c->role, &c->r, &c->g, &c->b);
  c->a = 220;
  cubalc_atom_init(&c->atom, c->id, proton);
  /* seed matrix from role hash + genesis overlap */
  for (int i = 0; i < 16; i++) {
    int bit = (c->id[i % (int)strlen(c->id)] * 7 + i * 3) % CUBALC_ATOM_BITS;
    cubalc_matrix_set(&c->atom.matrix, bit, 1);
  }
  /* inherit some genesis bits for chain cohesion */
  for (int i = 0; i < ch->initial.n && i < CUBALC_ATOM_BITS; i++)
    if (cubalc_matrix_get(&ch->initial, i) && (i % 3 == 0))
      cubalc_matrix_set(&c->atom.matrix, i, 1);
  c->atom.digit = cubalc_algocube_digit(&c->atom.matrix);
  /* 6 LEGO faces open */
  c->n_ports = 6;
  for (int f = 0; f < 6; f++) {
    c->ports[f].open = 1;
    c->ports[f].peer = -1;
    c->ports[f].face = (uint8_t)f;
    c->ports[f].gate = c->atom.matrix; /* gate = own matrix by default */
  }
  return ch->n_cubes++;
}

int cubalc_cube_plug(cubalc_chain *ch, int a, int b) {
  if (!ch || a < 0 || b < 0 || a >= ch->n_cubes || b >= ch->n_cubes || a == b) return -1;
  float cmp = cubalc_matrix_compat(&ch->cubes[a].atom.matrix, &ch->cubes[b].atom.matrix);
  /* LEGO snap only if matrices compatible enough */
  if (cmp < 0.35f) return -2; /* incompatible */
  /* find free ports */
  int pa = -1, pb = -1;
  for (int i = 0; i < ch->cubes[a].n_ports; i++)
    if (ch->cubes[a].ports[i].open && ch->cubes[a].ports[i].peer < 0) { pa = i; break; }
  for (int i = 0; i < ch->cubes[b].n_ports; i++)
    if (ch->cubes[b].ports[i].open && ch->cubes[b].ports[i].peer < 0) { pb = i; break; }
  if (pa < 0 || pb < 0) return -3;
  ch->cubes[a].ports[pa].peer = (int16_t)b;
  ch->cubes[b].ports[pb].peer = (int16_t)a;
  ch->cubes[a].plugged++;
  ch->cubes[b].plugged++;
  /* binary talk on plug */
  cubalc_cube_talk(ch, a, b);
  return 0;
}

int cubalc_cube_unplug(cubalc_chain *ch, int a, int b) {
  if (!ch || a < 0 || b < 0) return -1;
  for (int i = 0; i < ch->cubes[a].n_ports; i++)
    if (ch->cubes[a].ports[i].peer == b) {
      ch->cubes[a].ports[i].peer = -1;
      if (ch->cubes[a].plugged) ch->cubes[a].plugged--;
    }
  for (int i = 0; i < ch->cubes[b].n_ports; i++)
    if (ch->cubes[b].ports[i].peer == a) {
      ch->cubes[b].ports[i].peer = -1;
      if (ch->cubes[b].plugged) ch->cubes[b].plugged--;
    }
  return 0;
}

int cubalc_bin_pack(const cubalc_atom *atom, const char *from, const char *to,
                    uint32_t seq, uint8_t *out, size_t cap, size_t *n_out) {
  if (!atom || !out) return -1;
  size_t need = sizeof(cubalc_bin_hdr) + (atom->matrix.n + 7) / 8;
  if (cap < need) return -1;
  cubalc_bin_hdr *h = (cubalc_bin_hdr *)out;
  memset(h, 0, sizeof(*h));
  h->magic = CUBALC_MAGIC_BIN;
  h->proto = CUBALC_PROTO_V1;
  h->n_bits = atom->matrix.n;
  h->set = atom->matrix.set;
  h->proton = atom->proton;
  h->digit = atom->digit;
  h->seq = seq;
  if (from) strncpy(h->from_id, from, CUBALC_ID_LEN - 1);
  if (to) strncpy(h->to_id, to, CUBALC_ID_LEN - 1);
  memcpy(out + sizeof(*h), atom->matrix.bits, (atom->matrix.n + 7) / 8);
  if (n_out) *n_out = need;
  return 0;
}

int cubalc_bin_unpack(const uint8_t *in, size_t n, cubalc_atom *atom_out,
                      char *from, char *to, uint32_t *seq) {
  if (!in || n < sizeof(cubalc_bin_hdr) || !atom_out) return -1;
  const cubalc_bin_hdr *h = (const cubalc_bin_hdr *)in;
  if (h->magic != CUBALC_MAGIC_BIN || h->proto != CUBALC_PROTO_V1) return -2;
  size_t need = sizeof(*h) + (h->n_bits + 7) / 8;
  if (n < need) return -1;
  memset(atom_out, 0, sizeof(*atom_out));
  atom_out->proton = h->proton;
  atom_out->digit = h->digit;
  atom_out->alive = h->proton ? 1 : 0;
  atom_out->matrix.n = h->n_bits > CUBALC_ATOM_BITS ? CUBALC_ATOM_BITS : h->n_bits;
  atom_out->matrix.set = h->set;
  memcpy(atom_out->matrix.bits, in + sizeof(*h), (atom_out->matrix.n + 7) / 8);
  if (from) { strncpy(from, h->from_id, CUBALC_ID_LEN - 1); from[CUBALC_ID_LEN-1]=0; }
  if (to) { strncpy(to, h->to_id, CUBALC_ID_LEN - 1); to[CUBALC_ID_LEN-1]=0; }
  if (seq) *seq = h->seq;
  return 0;
}

int cubalc_cube_talk(cubalc_chain *ch, int from, int to) {
  if (!ch || from < 0 || to < 0 || from >= ch->n_cubes || to >= ch->n_cubes) return -1;
  uint8_t buf[256];
  size_t n = 0;
  cubalc_cube *A = &ch->cubes[from], *B = &ch->cubes[to];
  if (cubalc_bin_pack(&A->atom, A->id, B->id, ch->seq, buf, sizeof buf, &n) != 0) return -1;
  cubalc_atom received;
  char f[CUBALC_ID_LEN], t[CUBALC_ID_LEN];
  uint32_t seq = 0;
  if (cubalc_bin_unpack(buf, n, &received, f, t, &seq) != 0) return -1;
  /* merge received bits into peer (OR for create, AND-clear for destroy frame) */
  if (received.proton) {
    for (int i = 0; i < received.matrix.n && i < CUBALC_ATOM_BITS; i++)
      if (cubalc_matrix_get(&received.matrix, i))
        cubalc_matrix_set(&B->atom.matrix, i, 1);
    B->atom.alive = 1;
  } else {
    for (int i = 0; i < received.matrix.n && i < CUBALC_ATOM_BITS; i++)
      if (cubalc_matrix_get(&received.matrix, i))
        cubalc_matrix_set(&B->atom.matrix, i, 0);
  }
  B->atom.digit = cubalc_algocube_digit(&B->atom.matrix);
  B->atom.unity = cubalc_matrix_compat(&A->atom.matrix, &B->atom.matrix);
  return 0;
}

void cubalc_chain_init(cubalc_chain *ch) {
  if (!ch) return;
  memset(ch, 0, sizeof(*ch));
  ch->hold_flash = CUBALC_HOLD_FLASH;
  ch->unity = 1.f;
  strncpy(ch->creed, CUBALC_CREED, sizeof(ch->creed) - 1);
  snprintf(ch->status, sizeof ch->status, "empty chain");
}

int cubalc_coord_to_matrix(const char *plate_line, cubalc_matrix *out) {
  if (!out) return -1;
  cubalc_matrix_clear(out);
  out->n = CUBALC_ATOM_BITS;
  if (!plate_line) return -1;
  /* fold plate chars into bits — SoT seed */
  size_t L = strlen(plate_line);
  for (size_t i = 0; i < L; i++) {
    unsigned char c = (unsigned char)plate_line[i];
    int bit = (int)((c * 131u + i * 17u) % CUBALC_ATOM_BITS);
    if (c == '1' || (c & 1)) cubalc_matrix_set(out, bit, 1);
  }
  /* law flags sticky */
  cubalc_matrix_set(out, 11, 1); /* hold_flash slot */
  cubalc_matrix_set(out, 12, 1); /* no_brain_wire */
  cubalc_matrix_set(out, 13, 1); /* matrix_is_key */
  return 0;
}

int cubalc_chain_from_initial(cubalc_chain *ch, const cubalc_matrix *genesis, uint32_t seq) {
  if (!ch || !genesis) return -1;
  cubalc_chain_init(ch);
  ch->initial = *genesis;
  ch->seq = seq;
  ch->hold_flash = 1;
  snprintf(ch->status, sizeof ch->status, "genesis set=%u n=%u seq=%u",
           (unsigned)genesis->set, (unsigned)genesis->n, (unsigned)seq);
  return 0;
}

/* OS aspects as LEGO cubes — important planes of the Cube way */
int cubalc_chain_os_aspects(cubalc_chain *ch) {
  if (!ch) return -1;
  /* ring layout — octopus-friendly big studs */
  struct { const char *id; const char *role; uint8_t proton; float ang; float elev; } asp[] = {
    {"cube-sot",     "kernel_sot",   1, 0.f,           1.35f},
    {"cube-coord",   "coord",        1, 0.785f,        1.35f},
    {"cube-llama",   "llama",        1, 1.57f,         1.25f},
    {"cube-host",    "host_station", 1, 2.356f,        1.25f},
    {"cube-quest",   "quest_lizard", 1, 3.141f,        1.20f},
    {"cube-wivrn",   "wivrn_way",    1, 3.927f,        1.20f},
    {"cube-kinect",  "kinect",       1, 4.712f,        1.15f},
    {"cube-create",  "construct",    1, 5.498f,        1.40f},
    {"cube-destroy", "deconstruct",  0, 0.f,           0.95f},
    {"cube-hive",    "nanobot_hive", 1, 1.0f,          1.55f},
  };
  int n = (int)(sizeof(asp) / sizeof(asp[0]));
  float R = 0.85f;
  for (int i = 0; i < n && ch->n_cubes < CUBALC_BUDGET; i++) {
    float x = cosf(asp[i].ang) * R;
    float z = -2.f + sinf(asp[i].ang) * R;
    cubalc_cube_spawn(ch, asp[i].id, asp[i].role, asp[i].proton, x, asp[i].elev, z);
  }
  /* plug adjacent in ring if compatible */
  int base = ch->n_cubes - n;
  if (base < 0) base = 0;
  for (int i = 0; i < n - 1; i++)
    cubalc_cube_plug(ch, base + i, base + ((i + 1) % n));
  /* create plugs to destroy (construct↔deconstruct edge) */
  for (int i = 0; i < ch->n_cubes; i++) {
    if (strcmp(ch->cubes[i].id, "cube-create") == 0)
      for (int j = 0; j < ch->n_cubes; j++)
        if (strcmp(ch->cubes[j].id, "cube-destroy") == 0)
          cubalc_cube_plug(ch, i, j);
  }
  snprintf(ch->status, sizeof ch->status, "OS LEGO n=%d plugs live", ch->n_cubes);
  return ch->n_cubes;
}

int cubalc_chain_tick(cubalc_chain *ch) {
  if (!ch) return -1;
  ch->seq++;
  float u = 0.f; int alive = 0;
  for (int i = 0; i < ch->n_cubes; i++) {
    cubalc_cube *c = &ch->cubes[i];
    c->atom.digit = cubalc_algocube_digit(&c->atom.matrix);
    if (c->atom.alive) { alive++; u += c->atom.unity > 0 ? c->atom.unity : 1.f; }
    /* gentle LEGO pulse size by set density */
    float dens = c->atom.matrix.n ? (float)c->atom.matrix.set / (float)c->atom.matrix.n : 0;
    c->s = 0.10f + 0.08f * dens;
  }
  ch->unity = alive ? u / (float)alive : 1.f;
  snprintf(ch->status, sizeof ch->status,
           "tick seq=%u cubes=%d alive=%d unity=%.2f hold=%u creed=hail",
           (unsigned)ch->seq, ch->n_cubes, alive, ch->unity, (unsigned)ch->hold_flash);
  return 0;
}

int cubalc_chain_impulse(cubalc_chain *ch, const char *cube_id, uint8_t proton) {
  if (!ch || !cube_id) return -1;
  for (int i = 0; i < ch->n_cubes; i++) {
    if (strcmp(ch->cubes[i].id, cube_id) == 0) {
      cubalc_atom_impulse(&ch->cubes[i].atom, proton);
      /* propagate binary talk to all plugged peers */
      for (int p = 0; p < ch->cubes[i].n_ports; p++) {
        int peer = ch->cubes[i].ports[p].peer;
        if (peer >= 0) cubalc_cube_talk(ch, i, peer);
      }
      return 0;
    }
  }
  return -1;
}

int cubalc_chain_write_viz(const cubalc_chain *ch, const char *path) {
  if (!ch || !path) return -1;
  FILE *f = fopen(path, "w");
  if (!f) return -1;
  int n = ch->n_cubes;
  if (n > CUBALC_BUDGET) n = CUBALC_BUDGET;
  fprintf(f, "{\"schema\":\"cube.viz_frame.v1\",\"seq\":%u,\"unity\":%.4f,"
             "\"n_cubes\":%d,\"budget\":%d,\"cubalc\":true,\"hold_flash\":1,"
             "\"hud\":\"CubalC LEGO · %d studs · unity=%.2f · %s\","
             "\"cubes\":[",
          (unsigned)ch->seq, ch->unity, n, CUBALC_BUDGET, n, ch->unity, ch->creed);
  for (int i = 0; i < n; i++) {
    const cubalc_cube *c = &ch->cubes[i];
    if (i) fputc(',', f);
    fprintf(f, "{\"x\":%.3f,\"y\":%.3f,\"z\":%.3f,\"s\":%.3f,"
               "\"rgba\":[%u,%u,%u,%u],\"role\":%u,"
               "\"id\":\"%s\",\"label\":\"%s\",\"digit\":%u,\"proton\":%u,"
               "\"plugged\":%u,\"set\":%u}",
            c->x, c->y, c->z, c->s,
            c->r, c->g, c->b, c->a,
            c->atom.proton ? 1 : 6,
            c->id, c->label, (unsigned)c->atom.digit, (unsigned)c->atom.proton,
            (unsigned)c->plugged, (unsigned)c->atom.matrix.set);
  }
  fprintf(f, "]}\n");
  fclose(f);
  return 0;
}

int cubalc_chain_write_json(const cubalc_chain *ch, const char *path) {
  if (!ch || !path) return -1;
  FILE *f = fopen(path, "w");
  if (!f) return -1;
  fprintf(f, "{\"schema\":\"cubalc.chain.v1\",\"creed\":\"%s\","
             "\"seq\":%u,\"unity\":%.4f,\"hold_flash\":%u,\"n_cubes\":%d,"
             "\"initial_set\":%u,\"status\":\"%s\",\"budget\":%d,"
             "\"law\":\"matrix_compat_plug·binary_talk·nanobot_atom·proton_01\"}\n",
          ch->creed, (unsigned)ch->seq, ch->unity, (unsigned)ch->hold_flash,
          ch->n_cubes, (unsigned)ch->initial.set, ch->status, CUBALC_BUDGET);
  fclose(f);
  return 0;
}
