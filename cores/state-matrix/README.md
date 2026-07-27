# State Matrix core

Binary source of truth for hive coordination.

- Frame size: 64 bytes (SMX)  
- Fields: magic, version, purpose bits, integrity, algocube digit, payload fold  
- **No personal data fields exist** by design  

Implementation reference: `products/grokium/src/grokium/smx_binary.py`, `matrix.py`.
