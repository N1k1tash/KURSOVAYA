/** 
* @file Client.h
* @author Шматков Н.С.
* @version 1.0
* @date 25.04.2023
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл для модуля Client
*/
#pragma once
#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <string>
#include <locale>
#include <codecvt>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;


/** 
* @brief Класс взаимодействия с сервером
* @param m_serverAddress адрес сервера
* @param m_serverPort порт сервера
* @param m_inputDataFile имя файла с векторами
* @param m_outputDataFile имя файла с результатами вычислений сервером
* @param m_clientCredentialsFile имя файла с логином и паролем клиента
*/
class Client{

    public:
    // Функции для взаимодействия клиента и сервера
    int interactWithServer();
    std::vector<std::vector<uint32_t>> readVectorsFromFile();
    void writeVectorsToFile( const std::vector<uint32_t>& vectors);
    std::pair<std::string, std::string> readLoginAndPasswordFromFile();

    // Гетторы и сетторы атрибутов класса Client
    int getServerAddress() const;
    void setServerAddress(int address);

    int getServerPort() const;
    void setServerPort(int port);

    string getInputDataFile() const;
    void setInputDataFile(const string& filename);

    string getOutputDataFile() const;
    void setOutputDataFile(const string& filename);

    string getClientCredentialsFile() const;
    void setClientCredentialsFile(const string& filename);

    private:
        int m_serverAddress;
        int m_serverPort;
        string m_inputDataFile;
        string m_outputDataFile;
        string m_clientCredentialsFile;

};


/** @brief Класс обработки ошибок ErrorManager
* @details Менеджер вывода сообщений об ошибках
*/
class ErrorManager: public std::invalid_argument
{
public:
	explicit ErrorManager (const std::string& what_arg):
		std::invalid_argument(what_arg) {}
	explicit ErrorManager (const char* what_arg):
		std::invalid_argument(what_arg) {}
};


    