#include "commandLine.hpp"

#include <tuple>
#include <string>

char CommandLine::getShortOption( )
{
    int guard_counter = 0;
    while( char_to_name.find( current_options ) != char_to_name.end( ) )
    {
        current_options++;
        guard_counter++;
        if( guard_counter == 513 )
            throw std::exception( );
    }
    return current_options;
}

char *CommandLine::copyString( const std::string &__name )
{
    auto buf = new char[ __name.size( ) + 1 ];
    std::copy( __name.begin( ), __name.end( ), buf );
    buf[ __name.size( ) ] = '\0';
    return buf;
}

CommandLine::CommandLine( )
{
    add< false >( "help", 'h', "Display this information." );
}

void CommandLine::printHelp( )
{
    std::cout << "Usage: <prog_name> [options] file..." << std::endl
              << "Options: " << std::endl;
    for( auto &opt : helps )
    {
        std::string str = "--" + std::get< 0 >( opt );
        std::cout << std::setw( 20 )
                  << std::left << str
                  << std::left << " "
                  << std::get< 1 >( opt )
                  << std::endl;
    }
}

void CommandLine::read( int &argc, char **argv )
{
    cmd_options.push_back( option{ nullptr, 0, nullptr, 0 } );

    int iarg;
    int index;
    while( ( iarg = getopt_long( argc, argv, line_short_options.c_str( ), cmd_options.data( ), &index ) ) != -1 )
    {
        if( char_to_name.find( iarg ) != char_to_name.end( ) )
        {
            if( options_with_arg.find(char_to_name[ iarg ] ) != options_with_arg.end( ) )
                options_with_arg[ char_to_name[ iarg ] ] = optarg;
            else if( options_without_arg.find(char_to_name[ iarg ] ) != options_without_arg.end( ) )
                options_without_arg[ char_to_name[ iarg ] ] = "1";
            else
                throw std::exception( );
        }
    }
}




std::tuple< std::string, bool >
CommandLine::get( const std::string &__name )
{
    if( options_with_arg.find( __name ) == options_with_arg.end( ) )
        throw std::exception( );

    return std::make_tuple( options_with_arg[ __name ], options_with_arg[ __name ] != "" );
}


std::tuple< std::string, bool > CommandLine::get( const char __short_name )
{
    if( char_to_name.find( __short_name ) == char_to_name.end( ) )
        throw std::exception( );
    if( options_with_arg.find( char_to_name[ __short_name ] ) == options_with_arg.end( ) )
        throw std::exception( );

    return std::make_tuple( char_to_name[ __short_name ], options_with_arg[ char_to_name[ __short_name ] ] != "" );
}

bool CommandLine::used( const std::string &__name )
{
    if( options_without_arg.find( __name ) == options_without_arg.end( ) )
        throw std::exception( );
    return !( options_without_arg[ __name ] == "" );
}

bool CommandLine::used( const char __short_name )
{
    if( char_to_name.find( __short_name ) == char_to_name.end( ) )
        throw std::exception( );
    if( options_without_arg.find( char_to_name[ __short_name ] ) == options_without_arg.end( ) )
        throw std::exception( );

    return !( options_without_arg[ char_to_name[ __short_name ] ] == "" );
}

void CommandLine::clear( )
{
    options_with_arg.clear( );
    options_without_arg.clear( );
    helps.clear( );
    char_to_name.clear( );
    line_short_options.clear( );
    current_options = 'a';
    cmd_options.clear( );
}

std::string CommandLine::check_and_get( const std::string &__name, const std::string __default )
{
    return std::get< 1 >( get( __name ) )? std::get< 0 >( get( __name ) ) : __default;
}

std::string CommandLine::check_and_get( char __short_name, const std::string __default )
{
    return std::get< 1 >( get( __short_name ) )? std::get< 0 >( get( __short_name ) ) : __default;
}