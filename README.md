# Data Encryption Standard

This program is a CLI tool that utilize the DES (Data Encryption Standard) algorithm to encrypt and decrypt text.

## Compilation
```
git clone https://github.com/eFanton-Dev/DES.git
cd DES

mkdir build
cd build

cmake ..
cmake --build .
```

The executable will be placed inside `src`.

## Usage
```
.\DES [flags] [options] (-f <path> | -t <text>) -k <key1>[,<key2>]
```

### Flags

| Argument | Description |
|:---:|:---|
| `(--encrypt \| --decrypt)` | Encrypt/Decrypt text (default --encrypt) |
| `--3DES` | Use 3DES instead of DES (needs key2 to be specified) |
| `--help` | Display informations |

### Oprions

| Argument | Description |
|:---:|:---|
| `-f <file>` | Get plaintext from *<file\>* |
| `-t <text>` | Specify plaintext |
| `-k <key1>[,<key2>]` | Keys to encrypt/decrypt the text (*<key2\>* must be specified if `--3DES` flag is present) |
| `-o <file>` | Place the output into *<file\>* |
