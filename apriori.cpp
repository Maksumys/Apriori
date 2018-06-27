//
// Created by Максим Кузин on 27.06.2018.
//

#include <iomanip>

#include "apriori.hpp"

void Apriori::initialize( const std::string &__fileName, const std::uint64_t __support )
{
    if( __fileName.empty( ) )
        throw std::exception( );

    support = __support;

    std::ifstream in_file( __fileName );
    std::copy( std::istream_iterator< std::string >( in_file ),
               std::istream_iterator< std::string >( ),
               std::back_inserter( data ) );

    first_table.resize( data[ 0 ].size( ) );
}

void Apriori::getFirstTable( )
{
    for( std::uint64_t i = 0; i < data.size( ); i++ )
        for( std::uint64_t j = 0; j < data[ 0 ].size( ); j++ )
            if( data[ i ][ j ] == '1' )
                first_table[ j ]++;
    std::vector< Element_table > new_table;
    for( std::uint64_t i = 0; i < first_table.size(); i++ )
    {
        if( first_table[ i ] >= support )
        {
            std::set< std::uint64_t > new_set;
            new_set.insert( i );
            new_table.push_back( Element_table{ new_set, first_table[ i ] } );
        }
    }
    tables.push_back( new_table );
}

void Apriori::getElementTable( const std::uint64_t __num_iter )
{
    std::vector< Element_table > new_table;

    /// формирование новых множест из предыдущей итерации
    for( std::uint64_t i = 0; i < tables[ __num_iter ].size( ); i++ )
    {
        for( std::uint64_t j = i + 1; j < tables[ __num_iter ].size( ); j++ )
        {
            /// создаем новое множество
            std::set< std::uint64_t > set = tables[ __num_iter ][ i ].set;
            set.insert( tables[ __num_iter ][ j ].set.begin( ), tables[ __num_iter ][ j ].set.end( ) );

            /// если превышает количество элементов
            if( set.size( ) > ( __num_iter + 2 ) )
                continue;

            /// проверяем на повторение
            bool isFind{ false };
            for( const auto& elem : new_table )
            {
                if( elem.set == set )
                {
                    isFind = true;
                    break;
                }
            }
            if( !isFind )
            {
                std::uint64_t new_support = 0;
                /// цикл по элементам нового множества
                for( std::uint64_t k = 0; k < data.size(); k++ )
                {
                    std::uint64_t count_find = 0;
                    /// цикл по строкам исходных данных
                    for( const auto &el : set )
                    {
                        if( data[ k ][ el ] == '1' )
                        {
                            count_find++;
                        }
                    }
                    auto size = set.size( );
                    if( count_find == set.size( ) )
                        new_support++;
                }
                if( new_support >= support )
                    new_table.push_back( Element_table{ std::move( set ), std::move( new_support ) } );
            }
        }
    }
    tables.push_back( new_table );
}

void Apriori::run( )
{
    getFirstTable( );

    for( std::uint64_t i = 0; !tables[ tables.size() - 1 ].empty( ); i++ )
    {
        getElementTable( i );
    }
}

void Apriori::print( const std::uint64_t __count, const std::string &__sort ) noexcept
{
    std::uint64_t printed = 0;

    for( auto &endtable : tables )
    {
        if( __sort == "desc" )
        {
            std::sort( endtable.begin( ), endtable.end( ), [ ]( const Element_table &a, const Element_table &b ) -> bool
                             {
                                 if( a.support > b.support )
                                     return true;
                                 else
                                     return false;
                             }
                );
        }
        else if( __sort == "asc" )
        {
            std::sort( endtable.begin( ), endtable.end( ), [ ]( const Element_table &a, const Element_table &b ) -> bool
                       {
                           if( a.support < b.support )
                               return true;
                           else
                               return false;
                       }
                     );
        }

        for( const auto &elem : endtable )
        {
            for( const auto &seti : elem.set )
            {
                std::cout << seti << " ";
            }
            std::cout << std::setw( 20 );
            std::cout << "    [ " << static_cast< double >( elem.support ) / data.size( ) << " ]" << std::endl;
            printed++;
            if( printed >= __count )
                return;
        }
    }
}
