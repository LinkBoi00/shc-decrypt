# shc-decrypt

Extract hidden bash scripts from executables created by [shc (Shell Script Compiler)](https://github.com/neurobin/shc).

**Tested with shc versions 3.9.8 and 4.0.3**

## How it works

shc encrypts shell scripts into C code, then compiles them into binaries (sh.x files). This tool uses `LD_PRELOAD` to intercept the decryption process at runtime, capturing the plaintext script before execution.

**Warning: To get the decrypted bash script, this tool will run code from the given sh.x executable. Make sure the executable you use is from a trusted source.**


## Build
```bash
make
```

This creates:
- `out/hooks.so` - a shared library that hooks key system functions
- `out/shc-decrypt` - A wrapper binary that automatically loads the hooks and executes the encrypted binary to get the decrypted bash script

## Usage

**Method 1: Using the wrapper binary (recommended)**
```bash
cd out/
```
```bash
./shc-decrypt /path/to/encrypted.sh.x
```

The decrypted script will be printed to your terminal.

**Example:**
```bash
$ ./shc-decrypt my_encrypted_script.sh.x

========== DECRYPTED SCRIPT START ==========
#!/bin/bash
echo "Secret script content here"
# ... rest of script ...
=========== DECRYPTED SCRIPT END ===========
```

**Method 2: Using LD_PRELOAD directly from the terminal**
```bash
cd out/
```
```bash
LD_PRELOAD="./hooks.so" /path/to/encrypted.sh.x
```

The decrypted script will be printed to your terminal.

**Example:**
```bash
$ LD_PRELOAD="./hooks.so" ./my_encrypted_script.sh.x

========== DECRYPTED SCRIPT START ==========
#!/bin/bash
echo "Secret script content here"
# ... rest of script ...
=========== DECRYPTED SCRIPT END ===========
```

## Friendly warning to all people using shc for "encryption"
shc does NOT and will never be able to properly encrypt your code. If you store your secrets (e.g passwords) in a plain bash script and expect shc to actually make it all secure, please stop.

Any executable script must exist in plaintext in memory at some point. This is a fundamental limitation, not a bug.
The password used to encrypt your script is also embedded inside the executable itself. Anyone with access to the binary has access to the key. That is how an shc executable is able to decrypt its contents **by itself on runtime**.

## Compatibility

Works with both hardened (`-H` flag) and non-hardened shc binaries. Bypasses expiration date checks as well.