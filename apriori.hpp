#ifndef TEST_APP_APRIORI_HPP
#define TEST_APP_APRIORI_HPP

#include <fstream>
#include <vector>
#include <list>
#include <set>
#include <exception>
#include <iostream>

class Apriori
{
public:
    struct Element_table
    {
        std::set< std::uint64_t >       set;
        std::uint64_t                   support;
    };

    using Table = std::vector< Element_table >;
private:
    std::vector< std::string >      data;           ///< данные из файла
    std::vector< Table >            tables;         ///< таблицы n - элементных наборов удовл. поддержке
    std::uint64_t                   support{ 0 };   ///< минимальная поддержка
    std::vector< std::uint64_t >    first_table;    ///< первая таблица
public:
    Apriori( ) = default;

    /*!
     * @brief Инициализация алгоритма Apriori входными параметрами
     * @param __fileName имя файла данных
     * @param __support минимальная поддержка
     */
    void initialize( const std::string &__fileName, const std::uint64_t __support );

    /*!
     * @brief получение 1-элементного набора
     */
    void getFirstTable( );

    /*!
     * @brief поиск очередного k-го набора
     * @param __num_iter номер итерации( количестов элементов в множестве __num_iter + 2 )
     */
    void getElementTable( const std::uint64_t __num_iter );

    /*!
     * @brief запуск алгоритма
     */
    void run( );

    /*!
     * @brief печать
     * @param __count количество
     * @param __sort тип сортировки
     */
    void print( const std::uint64_t __count, const std::string &__sort ) noexcept;
};


#endif //TEST_APP_APRIORI_HPP
