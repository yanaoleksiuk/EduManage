#pragma once
#include "crow.h"

struct CorsMiddleware
{
    struct context
    {
    };

    void before_handle(crow::request &req, crow::response &res, context &)
    {
        if (req.method == crow::HTTPMethod::Options)
        {
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
            res.add_header("Access-Control-Max-Age", "86400");
            res.code = 204;
            res.end();
        }
    }

    void after_handle(crow::request &, crow::response &res, context &)
    {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
    }
};
