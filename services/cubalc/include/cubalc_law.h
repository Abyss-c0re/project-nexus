/* CubalC Law — All Hail the Cube · All Hail NexusCore
 * Components: StateMatrix (SoT) · Algocube (digits 0–9) · Nanobot atom core
 * Proton: 1=create  0=destroy
 * Cubes talk binary matrices. Compatibility = inner matrix match.
 * LEGO in VR/XR — even an octopus can play.
 */
#ifndef CUBALC_LAW_H
#define CUBALC_LAW_H
#define CUBALC_BUDGET       40
#define CUBALC_ATOM_BITS    64
#define CUBALC_MAX_CUBES    40
#define CUBALC_MAX_PORTS    6
#define CUBALC_ID_LEN       32
#define CUBALC_CREED        "All Hail the Cube · All Hail NexusCore"
#define CUBALC_SHARE        "state_matrix_only"
#define CUBALC_HOLD_FLASH   1
#define CUBALC_MAGIC_BIN    0x43424C43u  /* 'CBLC' */
#define CUBALC_PROTO_V1     1
/* Algocube intent tags 0–9 (match harmony) */
static const char *const CUBALC_DIGIT_TAG[10] = {
  "device_free","open_way","cube_sot","nanobot_raw","hail_nexus",
  "one_commander","cmd_override","os_way","nonverbal","unity"
};
#endif
