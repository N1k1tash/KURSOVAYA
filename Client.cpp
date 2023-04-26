/**
 * @file Client.cpp
 */
#include "Client.h"
#include "./md5/md5.h"

    /**
    * @brief Геттер для m_serverAddress
    */
    int Client::getServerAddress() const{
        return m_serverAddress;
    }

    /**
    * @brief Сеттер для m_serverAddress
    */
    void Client::setServerAddress(int address){
        m_serverAddress = address;
    }

    /**
    * @brief Геттер для m_serverPort
    */
    int Client::getServerPort() const{
        return m_serverPort;
    }

    /**
    * @brief Сеттер для m_serverPort
    */
    void Client::setServerPort(int port) {
        m_serverPort = port;
    }

    /**
    * @brief Геттер для m_inputDataFile
    */
    string Client::getInputDataFile() const{
        return m_inputDataFile;
    }

    /**
    * @brief Сеттер для m_inputDataFile
    */
    void Client::setInputDataFile(const string& filename) {
        m_inputDataFile = filename;
    }

    /**
    * @brief Геттер для m_outputDataFile
    */
    string Client::getOutputDataFile() const{
        return m_outputDataFile;
    }

    /**
    * @brief Сеттер для m_outputDataFile
    */
    void Client::setOutputDataFile(const string& filename) {
        m_outputDataFile = filename;
    }

    /**
    * @brief Геттер для m_clientCredentialsFile
    */
    string Client::getClientCredentialsFile() const{
        return m_clientCredentialsFile;
    }

    /**
    * @brief Сеттер для m_clientCredentialsFile
    */
    void Client::setClientCredentialsFile(const string& filename) {

        m_clientCredentialsFile = filename;
    }

/**
* @brief Функция для получения логина и пароля из файла
* @return упорядоченную пару значений типа string, string соответствующих логину и паролю
*/
std::pair<std::string, std::string> Client::readLoginAndPasswordFromFile() {
    ifstream file(getClientCredentialsFile());
    // Проверка файла на открытие
    if(!file.is_open()){
        file.close();
        throw ErrorManager("error in function readLoginAndPasswordFromFile.\n File does not exist");
        }
    // Проверка файла на пустоту
    if (file.peek() == std::ifstream::traits_type::eof()) {
        file.close();
        throw ErrorManager("error in function readLoginAndPasswordFromFile.\n File is empty");
    }
    std::string line;
    std::getline(file, line);
    std::istringstream iss(line);
    std::string login, password;
    iss >> login >> password;
    file.close();
    return std::make_pair(login, password);

}

/**
* @brief Функция для получения векторов из файла
* @return возвращает массив векторов типа vector<uint32_t>
*/
std::vector<std::vector<uint32_t>> Client::readVectorsFromFile(){
    // Открываем файл для чтения в двоичном режиме
    ifstream infile(getInputDataFile(), ios::binary);
    if (!infile.is_open()) {
        throw ErrorManager("error in function readVectorsFromFile.\n File does not exist");
    }

    if(infile.peek() == EOF){
        throw ErrorManager("error in function readVectorsFromFile.\n File is empty");
    }

    
    // Читаем количество векторов
    uint32_t num_vectors;
    infile.read(reinterpret_cast<char*>(&num_vectors), sizeof(num_vectors));
    
    // Создаем вектор векторов
    vector<vector<uint32_t>> vectors(num_vectors);
    
    // Читаем каждый вектор
    for (auto& vec : vectors) {
        uint32_t vec_size;
        infile.read(reinterpret_cast<char*>(&vec_size), sizeof(vec_size));
        vec.resize(vec_size);
        infile.read(reinterpret_cast<char*>(vec.data()), vec_size * sizeof(uint32_t));
    }
    
    // Закрываем файл
    infile.close();
    
    return vectors;
}

/**
* @brief Функция для записи в файл результатов вычисления сервером
*/
void Client::writeVectorsToFile( const std::vector<uint32_t>& vectors) {
    ofstream outfile(getOutputDataFile(), ios::binary);
    // Проверка файла на открытие
    if (!outfile.is_open()) {
        outfile.close();
        throw ErrorManager("error in function writeVectorsToFile.\n File does not exist");
    }

    uint32_t vec_size = vectors.size();
    outfile.write(reinterpret_cast<const char*>(&vec_size), sizeof(uint32_t));

    outfile.write(reinterpret_cast<const char*>(&vectors[0]), vectors.size() * sizeof(uint32_t));

    outfile.close();
}


/**
* @brief Главная функция для взаимодействия клиента с сервером
* @throw ErrorManager при возникновении ошибки
*/
int Client::interactWithServer()
{
    // Получение логина и пароля из файла
    std::string login, password;
    std::tie(login, password) = readLoginAndPasswordFromFile();

    // Открытие файла на чтение
    vector<vector<uint32_t>> vectors = readVectorsFromFile();
    
    // Создание сокета
    char buf[1024];
    int bytes_read;

    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {   
        throw ErrorManager("error in function interactWithServer.\n socket creation error");
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(getServerPort());
    addr.sin_addr.s_addr = htonl(getServerAddress());
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        throw ErrorManager("error in function interactWithServer.\n server is not available");
    }

    // Отправка логина клиента
    strcpy(buf,login.c_str());
    send(sock, buf, login.length(), 0);

    // Прнятие ответа либо SALT, либо ERR
    bytes_read = recv(sock, buf, sizeof(buf), 0);
    buf[bytes_read] = '\0';
    string flag1 = string(buf);
    if (flag1 == "ERR"){
        close(sock);
        throw ErrorManager("error in function interactWithServer.\n login is not correct");
    }

    // Отправка HASHMD5( SALT16 || PASSWORD)
    string mes = flag1 + password;
    string HASHMD5 = MD5_hash(mes);

    strcpy(buf,HASHMD5.c_str());
    send(sock, buf, HASHMD5.length(), 0);



    // Прнятие ответа либо ОК, либо ERR
    bytes_read = recv(sock, buf, sizeof(buf), 0);
    buf[bytes_read] = '\0';
    string flag2 = string(buf);
    if (flag2 == "ERR"){
        close(sock);
        throw ErrorManager("error in function interactWithServer.\n password is not correct");
    }

    // Отправка количества векторов uint32_t
    int vec_size = vectors.size();
    send(sock, &vec_size, sizeof(vec_size), 0);

    // Созадние вектора для результатов
     vector<uint32_t> res_vectors;

    // Отправка векторов
    for(int i = 0; i < vec_size; i++){

        // Отправка длин векторов
        int vec_len = vectors[i].size();
        send(sock, &vec_len, sizeof(vec_len), 0);


        // Создание буфера для передачи на сервер вектора uint32_t
         uint32_t arr[vec_len] = {0};
         for (size_t j = 0; j < vectors[i].size(); j++) {
            arr[j] = vectors[i][j];
        }

        // Отправка вектора
        send(sock, &arr, sizeof(arr), 0);

        // Получение результата
        uint32_t res = 0;
        recv(sock, &res, sizeof(res), 0);

        res_vectors.push_back(res);

    }
    writeVectorsToFile(res_vectors);
    cout << "Запись в файл успешная." << endl;
    cout << "Результат сохранен в " << getOutputDataFile() << endl;

    close(sock);

    return 0;
}


    