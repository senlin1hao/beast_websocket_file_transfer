#include "beast_wss_file_server.h"

const char* CERT_FILE = "./certificate/test_crt.crt";
const char* CERT_KEY_FILE = "./certificate/test_crt.key";

int main()
{
    spdlog::init_thread_pool(wss_file_server::SESSION_LOG_QUEUE_SIZE, wss_file_server::SESSION_LOG_THREAD_COUNT);

    WssFileServer server("::", 34094, 4, CERT_FILE, CERT_KEY_FILE);
    server.start();

    return 0;
}
