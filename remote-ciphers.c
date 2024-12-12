#include <libssh2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <dlfcn.h>

void print_supported_ciphers(LIBSSH2_SESSION *session, const char *direction) {
    const char **ciphers;
    int num_ciphers = libssh2_session_supported_algs(session, LIBSSH2_METHOD_KEX, &ciphers);

    if (num_ciphers < 0) {
        fprintf(stderr, "Failed to get supported ciphers for %s\n", direction);
        return;
    }

    printf("Supported %s ciphers:\n", direction);
    for (int i = 0; i < num_ciphers; i++) {
        printf("  %s\n", ciphers[i]);
    }
}

int connect_to_server(const char *host, int port) {
    struct sockaddr_in sin;
    struct hostent *he;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    he = gethostbyname(host);
    if (!he) {
        fprintf(stderr, "Failed to resolve hostname: %s\n", host);
        close(sock);
        return -1;
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    memcpy(&sin.sin_addr, he->h_addr, he->h_length);

    if (connect(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("connect");
        close(sock);
        return -1;
    }

    return sock;
}

// Custom trace handler function
void custom_trace_handler(LIBSSH2_SESSION *session, void *context, const char *data, size_t length) {
    // Cast the context to the expected type if used; here we ignore it
    (void)session;
    (void)context;

    // Write the trace data to stderr
    printf("%s\n", data);
}

int
_libssh2_kex_exchange(LIBSSH2_SESSION * session, int reexchange,
                      void * key_state);

int main(int argc, char *argv[]) {
    void *handle = dlopen("/usr/lib/x86_64-linux-gnu/libssh2.so.1.0.1", RTLD_LAZY | RTLD_LOCAL);
    if (!handle) {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
        return 1;
    }

    // Try to fetch a private symbol (if you know its mangled name)
    void (*private_func)() = dlsym(handle, "_libssh2_match_string");
    if (!private_func) {
        fprintf(stderr, "dlsym failed: %s\n", dlerror());
    } else {
        private_func();
    }

    dlclose(handle);
    return 0;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s --url ssh://user@host:port [--key private_key_path] [--passphrase passphrase]\n", argv[0]);
        return 1;
    }

    char *url = NULL, *key = NULL, *passphrase = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--url") == 0 && i + 1 < argc) {
            url = argv[++i];
        } else if (strcmp(argv[i], "--key") == 0 && i + 1 < argc) {
            key = argv[++i];
        } else if (strcmp(argv[i], "--passphrase") == 0 && i + 1 < argc) {
            passphrase = argv[++i];
        }
    }

    if (!url) {
        fprintf(stderr, "Error: --url is required\n");
        return 1;
    }

    char user[256] = {0}, host[256] = {0};
    int port = 22;
    if (sscanf(url, "ssh://%255[^@]@%255[^:]:%d", user, host, &port) < 2) {
        fprintf(stderr, "Error: Invalid URL format. Use ssh://user@host:port\n");
        return 1;
    }

    if (libssh2_init(0) != 0) {
        fprintf(stderr, "libssh2 initialization failed\n");
        return 1;
    }

    int sock = connect_to_server(host, port);
    if (sock < 0) {
        libssh2_exit();
        return 1;
    }

    LIBSSH2_SESSION *session = libssh2_session_init();
    libssh2_trace_sethandler(session, NULL, custom_trace_handler);
    libssh2_trace(session, LIBSSH2_TRACE_SOCKET | LIBSSH2_TRACE_TRANS | LIBSSH2_TRACE_KEX | LIBSSH2_TRACE_AUTH |LIBSSH2_TRACE_CONN | LIBSSH2_TRACE_SCP | LIBSSH2_TRACE_SFTP | LIBSSH2_TRACE_ERROR | LIBSSH2_TRACE_PUBLICKEY);
    if (!session) {
        fprintf(stderr, "Failed to create SSH session\n");
        close(sock);
        libssh2_exit();
        return 1;
    }

    _libssh2_kex_exchange(session, 0, NULL);
    if (libssh2_session_handshake(session, sock)) {
                char *err_msg;

                libssh2_session_last_error(session, &err_msg, NULL, 0);
        fprintf(stderr, "SSH session handshake failed: %s\n", err_msg);
        libssh2_session_free(session);
        close(sock);
        libssh2_exit();
        return 1;
    }

    if (key) {
        if (libssh2_userauth_publickey_fromfile(session, user, key, NULL, passphrase)) {
            fprintf(stderr, "Authentication with private key failed\n");
            libssh2_session_disconnect(session, "Authentication failed");
            libssh2_session_free(session);
            close(sock);
            libssh2_exit();
            return 1;
        }
    } else {
        fprintf(stderr, "No key provided. Only performing handshake.\n");
    }

    print_supported_ciphers(session, "client-to-server");
    print_supported_ciphers(session, "server-to-client");

    libssh2_session_disconnect(session, "Normal Shutdown");
    libssh2_session_free(session);
    close(sock);
    libssh2_exit();

    return 0;
}
