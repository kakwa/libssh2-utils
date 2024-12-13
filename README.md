# libssh2-utils

Utilities for libssh2

# Build

## Dependencies

```bash
apt install cmake gcc libssh2-1-dev
```

## Compilation

```bash
cmake .
make
```

## Run

Local libssh2 ciphers listing:
```bash
./libssh2-list-ciphers
```

Cipher negociation test with a remote server:
```bash
./libssh2-test-remote-cipher-exchange --url ssh://user@localhost:22
```
