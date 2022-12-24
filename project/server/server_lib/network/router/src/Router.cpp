#include "Router.hpp"

#include <iostream>

#include "ErrorResponse.hpp"

void Router::addRoute(QueryType name, Route route) {
    routes_.emplace(name, route);
}

std::string Router::process(const std::string& requestData, User& user,
                            BasicMenu& mainMenu) {
    boost::system::error_code err;
    boost::json::object json_object =
        boost::json::parse(requestData, err).get_object();
    if (err) {
        return ErrorResponse().toJSON();
    }
    Request req;
    req.parse(json_object);

    auto route = routes_.find(req.req_type);
    if (route != routes_.end()) {
        Route steps = route->second;
        steps.request->parse(req.req_data);
        steps.handler->process(steps.request, steps.response, user, mainMenu);
        Response response(req.req_type, steps.response->toJSON());
        return response.toJSON();
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