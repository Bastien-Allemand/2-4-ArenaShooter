#ifndef TEST_JSON_HPP_INCLUDED
#define TEST_JSON_HPP_INCLUDED

#include <JsonParser.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( Json )

    const char8* path = RES_PATH"res/Testing/sample1.json";

    //JsonObject* root = JsonParser::Deserialize(path);
    //JsonParser::Deserialize(path);

    Vector<JsonObject*>* parsed = JsonParser::Deserialize(path);

    if (!parsed->empty())
    {
        JsonObject* root = (*parsed)[0];
        JsonParser::Serialize(root, RES_PATH"res/Testing/output.json");
    }

END_TEST_MAIN_FUNCTION( Json )
END_TEST( Json )


#endif