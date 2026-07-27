# Shared services (Linux · Android)

| Service | Linux | Android | Notes |
|---------|-------|---------|-------|
| nanobot peer | `:8787` binary | bound service + notification | same peer API |
| SMX heartbeat | file/socket | same | 64B frames |
| Integrity probe | nb-integrity | same library | fail closed |
| Optional VR view | OpenXR client | Quest / WebXR | view only |
| Clanker robot API | N/A or gateway | Commander app | one host type among many |
