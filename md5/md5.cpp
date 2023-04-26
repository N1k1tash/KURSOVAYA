/**
 * @file md5.cpp
 */

#include "md5.h"
/**
* @brief Получение хэш-кода по алгоритму md5
* @param msg сообщения для хэширования
* @return хэш-код
*/
std::string MD5_hash(std::string msg)
{
    using namespace CryptoPP;

    // Создаем объект-"хэширователь" для MD5
    Weak::MD5 hash;

    // куда запишем результат
    std::string digest;

    // создаем цепочку преобразования
    StringSource(msg, true,             // источник-стока
                 new HashFilter(hash,       // фильтр-"хеширователь"
                                new HexEncoder(     // кодировщик в строку цифр
                                //new Base64Encoder(   // альтернатива - кодировщик в base64
                                    new StringSink(digest))));  // строка-приемник
    // выводим сообщение и его хэш
    return digest;
}
