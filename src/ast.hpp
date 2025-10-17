#pragma once

#include <string_view>
#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <memory>
#include <cassert>

namespace SS
{

namespace Ast
{

enum class Type
{
    kDocument,
    kHeading,
    kInvalid
};

constexpr std::array< std::string_view, 3 > TypeStringMap = { "Document", "Heading", "Invalid" };

class INode
{
public:
    INode( Type type ) : m_Type{ type }
    {}

    virtual ~INode()
    {}

    virtual std::string_view ToString() const
    {
        int index = static_cast< int >( m_Type );

        if ( index >= TypeStringMap.size() )
            assert( 0 && "New AST node type added, but didn't Update the String Map" );

        return TypeStringMap[index];
    }

    Type GetType() const
    {
        return m_Type;
    }

    void AppendChild( std::unique_ptr< INode > child )
    {
        m_Children.push_back( std::move( child ) );
    }

    void Display( int level = 0 ) const
    {
        if ( GetType() == Type::kInvalid )
            return;

        for ( int _ = 0; _ < level; _++ )
            std::cout << "  ";

        std::cout << ToString() << std::endl;

        for ( const auto& child : m_Children ) {
            child->Display( level + 1 );
        }
    }

protected:
    Type m_Type;

private:
    std::vector< std::unique_ptr< INode > > m_Children;
};

#define AST_DEFAULT_CONSTRUCTOR( name )                                                            \
    name() : INode( Type::k##name )                                                                \
    {}

class Document : public INode
{
public:
    AST_DEFAULT_CONSTRUCTOR( Document );
};

class Heading : public INode
{
private:
    AST_DEFAULT_CONSTRUCTOR( Heading );

public:
    Heading( int level ) : Heading{}
    {
        std::stringstream s;
        s << "Heading<" << level << ">";
        m_DisplayedText = s.str();
    }

    virtual std::string_view ToString() const override
    {
        return m_DisplayedText;
    }

private:
    int m_Level;
    std::string m_DisplayedText;
};

} // namespace Ast

} // namespace SS
