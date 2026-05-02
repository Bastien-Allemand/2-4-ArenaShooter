#ifndef CORE_UTILS_H_INCLUDED
#define CORE_UTILS_H_INCLUDED

#include <Containers/Vector.hpp>
#include "define.h"

namespace gce {


 // ===== IS___ ===== //

inline bool IsDigit( char8 const c ) { return '0' <= c && c <= '9'; }

inline bool IsFloat( String const& s )
{
    uint64 const size = s.size();

    int8 commaCount = 0;

    for (uint8 i = 0; i < size; i++)
    {
        if (s[i] == '.') {
            commaCount++;
            if (commaCount > 1) return false;
            continue;
        }

        if (i == 0 && s[i] == '-')
            continue;

        int8 ASCII = (int8)s[i];

        if (47 < ASCII && ASCII < 58)
            continue;
        else
            return false;
    }
    return true;
}

inline bool IsInt( String const& s )
{
    for ( char8 const c : s )
        if ( IsDigit( c ) == false )
            return false;
    return true;
}


// ===== CONVERT_STRING_TO___ ===== //


inline float ConvertStringToFloat(String const& toConvert)
{
    if (!IsFloat(toConvert))
        return 0.0f;

    uint64 size = toConvert.size();
    float32 sign = 1.0f;
    uint64 commaIndex = String::npos;
    float32 result = 0.0f;
    float32 divider = 10.0f;

    uint64 i = 0;

    if (toConvert[i] == '-')
    {
        sign = -1.0f;
        i++;
    }

    for (; i < size; i++)
    {
        if (toConvert[i] == '.')
        {
            commaIndex = i;
            break;
        }

        result = result * 10 + (toConvert[i] - '0');
    }

    if (commaIndex != String::npos)
    {
        for (uint32 j = commaIndex + 1; j < size; j++)
        {
            result += (toConvert[j] - '0') / divider;
            divider *= 10.0f;
        }
    }

    return result * sign;
}

inline int64 ConvertStringToInt(String const& toConvert)
{
    if ( IsInt(toConvert) == false ) return 0;
    int64 result = 0;
    int64 sign = 1;
    uint64 size = toConvert.size();
    if (toConvert[0] == '-')
    {
        sign = -1;
        size--;
    }
    for (uint64 i = 0; i < size; i++)
    {
        result *= 10;
        result += (toConvert[i + (sign == -1 ? 1 : 0)] - '0');
    }
    return result * sign;
}


// ===== OTHER ===== //

inline Vector<String> EmptyRemover(Vector<String> vect)
{
    Vector<String> emptyDel;

    for (uint64 i = 0; i < vect.Size(); i++)
        if ( vect[i].empty() == false )
            emptyDel.PushBack(vect[i]);
    return emptyDel;
}

inline Vector<String> SplitString(String const& toSplit, char8 separator)
{
    Vector<String> result;
    String currentSegment;

    for (uint32 i = 0; i < toSplit.size(); i++)
    {
        if (toSplit[i] != separator)
        {
            if (toSplit[i] == '\n' || toSplit[i] == '\r')
                continue;
            currentSegment += toSplit[i];
        }
        else
        {
            result.PushBack(currentSegment);
            currentSegment.clear();
        }

    }

    result.PushBack(currentSegment);
    return result;
}


}

#endif
