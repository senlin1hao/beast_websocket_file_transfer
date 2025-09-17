#include <iostream>

#include "beast_wss_file_server.h"

const char* CERT_FILE = "./certificate/test_crt.crt";
const char* CERT_KEY_FILE = "./certificate/test_crt.key";

int main()
{
    WssFileServer server("::", 34094, 4, CERT_FILE, CERT_KEY_FILE);
    server.start();

    return 0;
}
