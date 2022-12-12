#include "Router.hpp"

#include <iostream>

#include "ErrorResponse.hpp"

void Router::addRoute(std::string name, Route route) {
    routes_.emplace(name, route);
}

std::string Router::process(const std::string& requestData,
                            ClientData& clientData, BasicMenu& mainMenu) {
    boost::json::object json_object;
    std::string method;
    boost::json::object data;
    try {
        json_object = boost::json::parse(requestData).get_object();
        method = json_object["type"].as_string().c_str();
        data = json_object["data"].as_object();
    } catch (std::exception ex) {
        std::cout << ex.what();
    }

    auto route = routes_.find(method);
    if (route != routes_.end()) {
        Route steps = route->second;
        steps.request->parse(data);
        steps.handler->process(steps.request, steps.response, clientData,
                               mainMenu);
        boost::json::object result({{"type", method}});
        result["data"] = steps.response->toJSON();
        return boost::json::serialize(result);
    }

    ErrorResponse response;
    return response.toJSON();
}

Router::~Router() {
    for (const auto& pair : routes_) {
        Route route = pair.second;
        delete route.handler;
        delete route.request;
        delete route.response;
    }
}