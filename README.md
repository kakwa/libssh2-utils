# libssh2-utils

Basic utilities for libssh2:
* **libssh2-list-ciphers**: list the supported ciphers by libssh2 on your host
* **libssh2-test-remote-cipher-exchange**: test cipher negociation with a remote server.

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
