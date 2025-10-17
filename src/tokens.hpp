#pragma once

#include <string_view>
#include <string>
#include <array>
#include <cassert>

namespace SS
{

namespace Token
{

enum class Type
{
    kHash,
    kText,
    kNewline,
    kInvalid
};

constexpr std::array< std::string_view, 4 > TypeStringMap = {
    "Hash", "Text", "Newline", "Invalid" };

class IToken
{
public:
    IToken( Type type ) : m_Type{ type }
    {}

    virtual ~IToken()
    {}

    virtual std::string_view ToString() const
    {
        int index = static_cast< int >( m_Type );

        if ( index >= TypeStringMap.size() )
            assert( 0 && "New Token type added, but didn't Update the String Map" );

        return TypeStringMap[index];
    }

    Type GetType() const
    {
        return m_Type;
    }

    bool IsInvalid() const
    {
        return GetType() == Type::kInvalid;
    }

protected:
    Type m_Type;
};

#define TOKEN_DEFAULT_CONSTRUCTOR( name )                                                          \
    name() : IToken( Type::k##name )                                                               \
    {}


class Hash : public IToken
{
public:
    TOKEN_DEFAULT_CONSTRUCTOR( Hash );
};

class Newline : public IToken
{
public:
    TOKEN_DEFAULT_CONSTRUCTOR( Newline )
};

class Text : public IToken
{
private:
    TOKEN_DEFAULT_CONSTRUCTOR( Text )

public:
    virtual std::string_view ToString() const override
    {
        if ( m_DisplayedText.empty() )
            m_DisplayedText = std::string( "Text: " ) + m_Text;

        return m_DisplayedText;
    }

    Text( std::string text ) : Text{}
    {
        m_Text = std::move( text );
    }

private:
    std::string m_Text;
    mutable std::string m_DisplayedText;
};

class Invalid : public IToken
{
public:
    TOKEN_DEFAULT_CONSTRUCTOR( Invalid )
};

} // namespace Token

} // namespace SS
