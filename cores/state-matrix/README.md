# State Matrix core

64-byte binary frames. **No personal data fields.**

```python
from smx import pack, unpack, harmony
f = pack(digit=7, seed=b"genesis")
print(unpack(f))
```

Used by CubeOS control plane and Grokium SMX share path.
