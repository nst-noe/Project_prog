#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QSqlRecord>
/*!
     Класс database реализует подключение к базе данных и отправление ей запросов

 */
class DataBase
{
public:
    DataBase();
    bool open();
    ~DataBase(){db.close();}
    QSqlDatabase db;
    std::string querry(std::string str_querry);
};

#endif // DATABASE_H
