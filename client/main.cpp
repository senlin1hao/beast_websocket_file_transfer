#include <iostream>

#include "beast_wss_file_client.h"

const char* CERT_FILE = "./certificate/test_crt.crt";

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_name>" << std::endl;
        return -1;
    }

    const char* file_name = argv[1];

    WssFileClient client("127.0.0.1", 34094, CERT_FILE);

    client.connect();
    client.download_file(file_name);
    client.disconnect();

    return 0;
}
