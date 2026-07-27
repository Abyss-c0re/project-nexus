"""State Matrix — 64-byte binary frames. No personal data fields exist."""
from __future__ import annotations
import hashlib, struct, time
from pathlib import Path

MAGIC = b"SMX1"
FRAME = 64

def pack(*, purpose: int = 0x1F, integrity: int = 1, digit: int = 0, seed: bytes | None = None) -> bytes:
    payload = hashlib.sha256(seed or f"smx|{time.time_ns()}".encode()).digest()[:48]
    raw = MAGIC + bytes([1, purpose & 0xFF, integrity & 0xFF, digit % 10]) + payload
    return raw[:FRAME].ljust(FRAME, b"\0")

def unpack(frame: bytes) -> dict:
    if len(frame) < 8 or frame[:4] != MAGIC:
        return {"ok": False, "error": "bad_frame"}
    return {
        "ok": True,
        "version": frame[4],
        "purpose": frame[5],
        "integrity": frame[6],
        "digit": frame[7],
        "bits_set": sum(bin(b).count("1") for b in frame),
        "hex": frame.hex(),
    }

def write_frame(path: Path | str, frame: bytes) -> None:
    Path(path).write_bytes(frame[:FRAME].ljust(FRAME, b"\0"))

def read_frame(path: Path | str) -> bytes:
    return Path(path).read_bytes()[:FRAME].ljust(FRAME, b"\0")

def harmony(a: bytes, b: bytes) -> dict:
    x = bytes(x ^ y for x, y in zip(a[:FRAME].ljust(FRAME, b"\0"), b[:FRAME].ljust(FRAME, b"\0")))
    bits = sum(bin(c).count("1") for c in x)
    digit = bits % 10
    return {"xor_bits": bits, "digit": digit, "unity": bits == 0}
