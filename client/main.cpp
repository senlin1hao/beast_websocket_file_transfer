#include <iostream>

#include "beast_wss_file_client.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_name>" << std::endl;
        return -1;
    }

    const char* file_name = argv[1];

    WssFileClient client("127.0.0.1", 34094, file_name);
    client.download_file();

    return 0;
}
