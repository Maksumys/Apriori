#include <iostream>

#include "commandLine.hpp"
#include "apriori.hpp"

bool checkCmd( CommandLine &__cmd )
{
    if( !std::get< 1 >( __cmd.get( "file" ) ) )
    {
        std::cout << "нужно указать файл бд ( --file )" << std::endl;
        return false;
    }

    if( !std::get< 1 >( __cmd.get( "support" ) ) )
    {
        std::cout << "нужно указать suport ( --support )" << std::endl;
        return false;
    }

    if( !std::get< 1 >( __cmd.get( "sort" ) ) )
    {
        std::cout << "нужно указать тип сортировки ( --sort )" << std::endl;
        return false;
    }

    if( !std::get< 1 >( __cmd.get( "count" ) ) )
    {
        std::cout << "нужно указать количество выводимых правил ( --count )" << std::endl;
        return false;
    }
    return true;
}


int main( int argc, char **argv )
{
    CommandLine cmd;
    cmd.add< true >( "file", 'f', "Нормализованный файл бд." );
    cmd.add< true >( "support", 's', "Показатель минимальной поддержки." );
    cmd.add< true >( "sort", 't', "Тип сортировки. ( asc, desc, none )" );
    cmd.add< true >( "count", 'c', "Количество правил выводимых на экран." );

    cmd.read( argc, argv );
    if( ( cmd.used( "help" ) ) || ( cmd.used( 'h' ) ) )
    {
        cmd.printHelp( );
        return 0;
    }

    if( !checkCmd( cmd ) )
        return 1;

    Apriori apriori;

    apriori.initialize( std::get< 0 >( cmd.get( "file" ) ),
                        std::stoull( std::get< 0 >( cmd.get( "support" ) ), nullptr, 10 )
        );
    apriori.run( );
    apriori.print( std::stoull( std::get< 0 >( cmd.get( "count" ) ), nullptr, 10 ),
                                std::get< 0 >( cmd.get( "sort" ) )
        );
    return 0;
}