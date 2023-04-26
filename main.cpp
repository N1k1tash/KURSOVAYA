#include <getopt.h> // для работы с опциями командной строки
#include "Client.h"
#pragma one

/** 
* @file main.cpp
* @brief Функция для предоставления пользователю справки по использованию программы
*/
void help_print(){
    cout << "Options:\n"
                 << "  -s, --server=ADDRESS    Server address (required)\n"
                 << "  -p, --port=PORT         Server port (default: 33333)\n"
                 << "  -i, --input=FILE        Input file (required)\n"
                 << "  -o, --output=FILE       Output file (required)\n"
                 << "  -c, --config=FILE       Config file (default: ~/.config/vclient.conf)\n"
                 << "  -h, --help              Display this help and exit\n";

}

/**
 * @brief Главный модуль программы для получения параметров от пользователя
*/
int main(int argc, char** argv) {
    // Опции командной строки
    const struct option long_options[] = {
        {"server", required_argument, nullptr, 's'},
        {"port", optional_argument, nullptr, 'p'},
        {"input", required_argument, nullptr, 'i'},
        {"output", required_argument, nullptr, 'o'},
        {"config", optional_argument, nullptr, 'c'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, 0}
    };

    // Переменные для хранения параметров
    int server_address;
    int port = 33333;
    string input_file;
    string output_file;
    string config_file = "./config/vclient.conf";

    // Обработка опций командной строки
    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "s:p::i:o:c::h::", long_options, &option_index)) != -1) {
        switch (opt) {
        case 's':
            try {
                server_address = stoi(optarg);
            }
            catch (const invalid_argument& e) {
            cerr << "Error: Invalid server address '" << optarg << "'\n";
            exit(EXIT_FAILURE);
            }
            break;
        case 'p':
            if (optarg != nullptr) {
            try {
                port = stoi(optarg);
            }
            catch (const invalid_argument& e) {
            cerr << "Error: Invalid port number '" << optarg << "'\n";
            exit(EXIT_FAILURE);
            }
            }
            break;
        case 'i':
            input_file = optarg;
            break;
        case 'o':
            output_file = optarg;
            break;
        case 'c':
            if (optarg != nullptr) {
                config_file = optarg;
            }
            break;
        case 'h':
            help_print();
            
            return 0;
        default:
            // Некорректная опция
            cerr << "Error: Invalid option\n";
            help_print();
            return 1;
        }
    }

    // Проверка наличия обязательных параметров
    if (server_address == 0 || input_file.empty() || output_file.empty()) {
        cerr << "Error: Required options are missing\n";
        return 1;
    }

    // Создание класса клиент Client
    Client NewClient;
    NewClient.setServerAddress(server_address);
    NewClient.setServerPort(port);
    NewClient.setInputDataFile(input_file);
    NewClient.setOutputDataFile(output_file);
    NewClient.setClientCredentialsFile(config_file);

    // Вызов основной функции для взаимодействия с сервером
    NewClient.interactWithServer();

    // ./Client -s 2130706433 -p33333 -i vectors.bin -o resultat.bin -c./config/vclient.conf
    return 0;
}
