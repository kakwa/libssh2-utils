#include <stdio.h>
#include <libssh2.h>

void list_supported_algorithms(LIBSSH2_SESSION *session, int method_type, const char *method_name) {
    const char **algorithms;
    int rc, i;

    // Retrieve the supported algorithms for the specified method type
    rc = libssh2_session_supported_algs(session, method_type, &algorithms);
    if (rc > 0) {
        printf("Supported %s algorithms:\n", method_name);
        for (i = 0; i < rc; i++) {
            printf("\t%s\n", algorithms[i]);
        }
        // Free the allocated memory for the algorithms list
        libssh2_free(session, (void *)algorithms);
    } else {
        printf("Failed to retrieve %s algorithms or none supported.\n", method_name);
    }
}

int main() {
    LIBSSH2_SESSION *session;

    // Initialize libssh2
    if (libssh2_init(0) != 0) {
        fprintf(stderr, "Failed to initialize libssh2\n");
        return 1;
    }

    // Create a new session
    session = libssh2_session_init();
    if (!session) {
        fprintf(stderr, "Failed to create libssh2 session\n");
        libssh2_exit();
        return 1;
    }

    // Iterate through all method types
    list_supported_algorithms(session, LIBSSH2_METHOD_KEX, "key exchange (KEX)");
    list_supported_algorithms(session, LIBSSH2_METHOD_HOSTKEY, "host key");
    list_supported_algorithms(session, LIBSSH2_METHOD_CRYPT_CS, "encryption (client-to-server)");
    list_supported_algorithms(session, LIBSSH2_METHOD_CRYPT_SC, "encryption (server-to-client)");
    list_supported_algorithms(session, LIBSSH2_METHOD_MAC_CS, "MAC (client-to-server)");
    list_supported_algorithms(session, LIBSSH2_METHOD_MAC_SC, "MAC (server-to-client)");

    // Cleanup session
    libssh2_session_free(session);

    // Deinitialize libssh2
    libssh2_exit();

    return 0;
}
