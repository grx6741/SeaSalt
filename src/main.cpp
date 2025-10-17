#include <fstream>
#include <cassert>
#include <memory>
#include <iostream>

#include "tokens.hpp"
#include "ast.hpp"

std::unique_ptr< SS::Token::IToken > NextToken( std::fstream& f )
{
    char c;
    f.get( c );

    // Skip spaces
    while ( c == ' ' )
        f.get( c );

    switch ( c ) {
        case '#':
            return std::make_unique< SS::Token::Hash >();
            break;
        case '\n':
            return std::make_unique< SS::Token::Newline >();
            break;
        default: {
            if ( isalnum( c ) ) {
                std::string text;
                do {
                    text += c;
                } while ( f.get( c ) && isalnum( c ) );
                return std::make_unique< SS::Token::Text >( std::move( text ) );
            }
        }
    }

    return std::make_unique< SS::Token::Invalid >();
}

int main( int argc, char** argv )
{
    assert( argc > 1 );

    std::fstream f( argv[1] );

    auto current_tok = NextToken( f );
    auto root = std::make_unique< SS::Ast::Document >();

    while ( !current_tok->IsInvalid() ) {
        std::cout << current_tok->ToString() << std::endl;

        switch ( current_tok->GetType() ) {
            case SS::Token::Type::kHash: {
                int level = 0;
                do {
                    level += 1;
                    current_tok = NextToken( f );
                } while ( current_tok->GetType() == SS::Token::Type::kHash );

                root->AppendChild( std::make_unique< SS::Ast::Heading >( level ) );
            } break;
            case SS::Token::Type::kNewline:
                break;
            case SS::Token::Type::kText:
                break;
            case SS::Token::Type::kInvalid:
                break;
        }

        current_tok = NextToken( f );
    }

    root->Display();
}
