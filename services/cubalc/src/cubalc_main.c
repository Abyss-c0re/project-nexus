#define _POSIX_C_SOURCE 200809L
#include "cubalc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

static const char *STATE =
  "<PROPHECY_CUBE_ROOT>/state";
static const char *DEFAULT_PLATE =
  "NEXUS_COORD v1 | from=BlackCube | type=heartbeat | topic=channel_stim | "
  "seq=1 | unity=1.0 | ssh=1 | llama=1 | watchd=1 | farm=standby | hold_flash=1 |";

static void paths(char *viz, char *chain, char *init, size_t n) {
  snprintf(viz, n, "%s/cubalc_viz_frame.json", STATE);
  snprintf(chain, n, "%s/cubalc_chain.json", STATE);
  snprintf(init, n, "%s/cubalc_initial_matrix.json", STATE);
}
static void publish_viz(const cubalc_chain *ch) {
  char a[512], b[512];
  snprintf(a, sizeof a, "%s/cubalc_viz_frame.json", STATE);
  snprintf(b, sizeof b, "%s/viz_frame.json", STATE);
  cubalc_chain_write_viz(ch, a);
  cubalc_chain_write_viz(ch, b); /* LEGO SoT for LOVR lean path */
}

static int cmd_genesis(const char *plate) {
  cubalc_chain ch;
  cubalc_matrix gen;
  cubalc_coord_to_matrix(plate ? plate : DEFAULT_PLATE, &gen);
  uint32_t seq = 1;
  const char *p = plate ? strstr(plate, "seq=") : NULL;
  if (p) seq = (uint32_t)strtoul(p + 4, NULL, 10);
  cubalc_chain_from_initial(&ch, &gen, seq);
  cubalc_chain_os_aspects(&ch);
  cubalc_chain_tick(&ch);
  char viz[512], chainp[512], initp[512];
  paths(viz, chainp, initp, sizeof viz);
  publish_viz(&ch);
  cubalc_chain_write_json(&ch, chainp);
  {
    FILE *f = fopen(initp, "w");
    if (f) {
      fprintf(f, "{\"schema\":\"cubalc.initial_matrix.v1\",\"n\":%u,\"set\":%u,\"bits\":\"",
              (unsigned)gen.n, (unsigned)gen.set);
      for (int i = 0; i < gen.n; i++) fputc(cubalc_matrix_get(&gen, i) ? '1' : '0', f);
      fprintf(f, "\",\"creed\":\"%s\",\"seq\":%u}\n", CUBALC_CREED, (unsigned)seq);
      fclose(f);
    }
  }
  printf("{\"ok\":true,\"cmd\":\"genesis\",\"cubes\":%d,\"set\":%u,\"seq\":%u,"
         "\"viz\":\"%s\",\"creed\":\"%s\"}\n",
         ch.n_cubes, (unsigned)gen.set, (unsigned)seq, viz, CUBALC_CREED);
  return 0;
}

static int cmd_impulse(const char *id, int proton) {
  /* reload from viz is lossy — re-genesis then impulse */
  cubalc_chain ch;
  cubalc_matrix gen;
  char initp[512], viz[512], chainp[512], bits[CUBALC_ATOM_BITS + 1];
  paths(viz, chainp, initp, sizeof viz);
  FILE *f = fopen(initp, "r");
  bits[0] = 0;
  if (f) {
    char buf[4096];
    size_t n = fread(buf, 1, sizeof buf - 1, f);
    buf[n] = 0;
    fclose(f);
    char *b = strstr(buf, "\"bits\":\"");
    if (b) {
      b += 8;
      int i = 0;
      while (*b && *b != '"' && i < CUBALC_ATOM_BITS) bits[i++] = *b++;
      bits[i] = 0;
    }
  }
  if (bits[0]) cubalc_matrix_from_ascii(&gen, bits, (int)strlen(bits));
  else cubalc_coord_to_matrix(DEFAULT_PLATE, &gen);
  cubalc_chain_from_initial(&ch, &gen, 1);
  cubalc_chain_os_aspects(&ch);
  cubalc_chain_impulse(&ch, id, (uint8_t)(proton ? 1 : 0));
  cubalc_chain_tick(&ch);
  publish_viz(&ch);
  cubalc_chain_write_json(&ch, chainp);
  printf("{\"ok\":true,\"cmd\":\"impulse\",\"id\":\"%s\",\"proton\":%d,\"cubes\":%d,\"status\":\"%s\"}\n",
         id, proton, ch.n_cubes, ch.status);
  return 0;
}

static int cmd_tick_loop(int n, int ms) {
  cubalc_chain ch;
  cubalc_matrix gen;
  char initp[512], viz[512], chainp[512], bits[CUBALC_ATOM_BITS + 1];
  paths(viz, chainp, initp, sizeof viz);
  bits[0] = 0;
  FILE *f = fopen(initp, "r");
  if (f) {
    char buf[4096]; size_t nr = fread(buf, 1, sizeof buf - 1, f); buf[nr]=0; fclose(f);
    char *b = strstr(buf, "\"bits\":\"");
    if (b) { b+=8; int i=0; while(*b && *b!='"' && i<CUBALC_ATOM_BITS) bits[i++]=*b++; bits[i]=0; }
  }
  if (bits[0]) cubalc_matrix_from_ascii(&gen, bits, (int)strlen(bits));
  else cubalc_coord_to_matrix(DEFAULT_PLATE, &gen);
  cubalc_chain_from_initial(&ch, &gen, 1);
  cubalc_chain_os_aspects(&ch);
  for (int i = 0; i < n; i++) {
    /* occasional binary talk pulse around the ring */
    if (ch.n_cubes > 1)
      cubalc_cube_talk(&ch, i % ch.n_cubes, (i + 1) % ch.n_cubes);
    cubalc_chain_tick(&ch);
    publish_viz(&ch);
    cubalc_chain_write_json(&ch, chainp);
    if (ms > 0) { struct timespec ts={ms/1000,(long)(ms%1000)*1000000L}; nanosleep(&ts,NULL); }
  }
  printf("{\"ok\":true,\"cmd\":\"tick\",\"n\":%d,\"cubes\":%d,\"unity\":%.2f}\n",
         n, ch.n_cubes, ch.unity);
  return 0;
}

int main(int argc, char **argv) {
  const char *cmd = argc > 1 ? argv[1] : "genesis";
  if (strcmp(cmd, "genesis") == 0) {
    const char *plate = argc > 2 ? argv[2] : DEFAULT_PLATE;
    return cmd_genesis(plate);
  }
  if (strcmp(cmd, "impulse") == 0) {
    const char *id = argc > 2 ? argv[2] : "cube-create";
    int proton = argc > 3 ? atoi(argv[3]) : 1;
    return cmd_impulse(id, proton);
  }
  if (strcmp(cmd, "tick") == 0) {
    int n = argc > 2 ? atoi(argv[2]) : 3;
    int ms = argc > 3 ? atoi(argv[3]) : 200;
    return cmd_tick_loop(n, ms);
  }
  if (strcmp(cmd, "help") == 0 || strcmp(cmd, "-h") == 0) {
    fprintf(stderr,
      "CubalC — Cube language · LEGO StateMatrix OS\n"
      "  cubalc genesis [NEXUS_COORD plate]\n"
      "  cubalc impulse <cube-id> <0|1>   # destroy|create proton\n"
      "  cubalc tick [n] [ms]\n"
      "Creed: %s\n", CUBALC_CREED);
    return 0;
  }
  fprintf(stderr, "unknown cmd %s\n", cmd);
  return 2;
}
