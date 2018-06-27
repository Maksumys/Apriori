#ifndef TEST_APP_COMMANDLINE_H
#define TEST_APP_COMMANDLINE_H

#include <unistd.h>
#include <getopt.h>


#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <tuple>
#include <vector>

/*!
 * @class CommandLine
 * @brief Класс для разбора опций коммандной строки
 */

class CommandLine
{
protected:
    std::unordered_map< std::string, std::string >      options_without_arg;            ///< опции без аргумента
    std::unordered_map< std::string, std::string >      options_with_arg;               ///< опции с аргументом

    std::unordered_map< char, std::string >             char_to_name;                   ///< соответствие short опции с long опцией
    std::unordered_map< std::string, std::string >      helps;                          ///< соответствие long опция с ее справкой

    std::string                                         line_short_options;             ///< short опции для getopt_long
    char                                                current_options{ 'a' };         ///< short опция по умолчанию для опций, которым не указали short
    std::vector< option >                               cmd_options;                    ///< вектор опций для getopt_long

    /*!
     * @brief генерация short опции
     * @return short опцию
     */
    char getShortOption( );

    /*!
     * @brief копирование строки для option ( getopt_long )
     * @param __name
     * @return
     */
    char* copyString( const std::string &__name );

public:

    /*!
     * @brief конструктор, создает так же опцию help
     */
    CommandLine( );

    /*!
     * @brief вывод информации об опциях
     */
    void printHelp( );

    /*!
     * @brief добавление опции
     * @tparam _withArgs признак опции с аргументом, если true, то опция с аргументом иначе без
     * @param __name имя опции
     * @throw exception::cmd::Error_add_arg если опция не найдена
     */
    template< bool _withArgs >
    void add( const std::string &__name )
    {
        if( options_without_arg.find( __name ) != options_without_arg.end( ) )
            throw std::exception();
        if( options_with_arg.find( __name ) != options_with_arg.end( ) )
            throw std::exception();

        if( _withArgs )
            options_with_arg[ __name ];
        else
            options_without_arg[ __name ];

        auto short_name = getShortOption( );

        cmd_options.push_back( option{ copyString( __name ), ( _withArgs )? required_argument : no_argument, nullptr, short_name } );

        line_short_options += std::string{ short_name };
        if( _withArgs )
            line_short_options += ":";

        char_to_name[ short_name ] = __name;
    }

    /*!
     * @brief добавление опции
     * @tparam _withArgs признак опции с аргументом, если true, то опция с аргументом иначе без
     * @param __name имя опции
     * @param __help_text текст опции в help
     */
    template< bool _withArgs >
    void add( const std::string &__name, const std::string &__help_text )
    {
        add< _withArgs >( __name );
        helps[ __name ] = __help_text;
    }

    /*!
     * @brief добавление опции
     * @tparam _withArgs признак опции с аргументом, если true, то опция с аргументом иначе без
     * @param __name long имя опции
     * @param __short_name short имя опции
     * @throw exception::cmd::Error_add_arg если опция не найдена
     */
    template< bool _withArgs >
    void add( const std::string &__name, const char __short_name )
    {
        if( options_without_arg.find( __name ) != options_without_arg.end( ) )
            throw std::exception();
        if( options_with_arg.find( __name ) != options_with_arg.end( ) )
            throw std::exception();
        if( char_to_name.find( __short_name ) != char_to_name.end( ) )
            throw std::exception();

        if( _withArgs )
            options_with_arg[ __name ];
        else
            options_without_arg[ __name ];

        cmd_options.push_back( option{ copyString( __name ), ( _withArgs )? required_argument : no_argument, nullptr, __short_name } );

        line_short_options += std::string{ __short_name };
        if( _withArgs )
            line_short_options += ":";

        char_to_name[ __short_name ] = __name;
    }

    /*!
     * @brief добавление опции
     * @tparam _withArgs признак опции с аргументом, если true, то опция с аргументом иначе без
     * @param __name long имя опции
     * @param __short_name short имя опции
     * @param __help_text описание опции
     * @throw exception::cmd::Error_add_arg если опция не найдена
     */
    template< bool __withArgs >
    void add( const std::string &__name, const char __short_name, const std::string &__help_text )
    {
        add< __withArgs >( __name, __short_name );
        helps[ __name ] = __help_text;
    }

    /*!
     * @brief чтение аргументов командной строки
     * @param argc количетсов аргументов
     * @param argv аргументы
     */
    void read( int &argc, char **argv );


    /*!
     * @brief получить значение опции по имени
     * @param __name long имя опции
     * @return значение если оно есть и флаг присутствия значения
     */
    std::tuple< std::string, bool > get( const std::string &__name );

    /*!
     * @brief получить значение опции по имени
     * @param __short_name short имя опции
     * @return значение если оно есть и флаг присутствия значения
     */
    std::tuple< std::string, bool > get( const char __short_name );

    /*!
     * @brief возвращает параметр опции если он есть иначе возвращает значение по умолчанию
     * @param __long_name long имя опции
     * @param __default значение по умолчанию
     * @return возвращает параметр опции если он есть иначе возвращает значение по умолчанию
     */
    std::string check_and_get( const std::string &__name, const std::string __default );

    /*!
     * @brief возвращает параметр опции если он есть иначе возвращает значение по умолчанию
     * @param __short_name short имя опции
     * @param __default значение по умолчанию
     * @return возвращает параметр опции если он есть иначе возвращает значение по умолчанию
     */
    std::string check_and_get( char __short_name, const std::string __default );

    /*!
     * @brief признак присутствия опции в командной строке
     * @param __long_name long имя опции
     * @return признак присутствия опции в командной строке
     */
    bool used( const std::string &__name );

    /*!
     * @brief признак присутствия опции в командной строке
     * @param __short_name short имя опции
     * @return признак присутствия опции в командной строке
     */
    bool used( const char __short_name );

    /*!
     * @brief сброс cmd
     */
    void clear( );
};

#endif
