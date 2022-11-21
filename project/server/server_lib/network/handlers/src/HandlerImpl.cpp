#include "HandlerImpl.hpp"

HandlerImpl::HandlerImpl(const Request* request, SessionImpl* session)
    : request_(request), session_(session) {}

//void HandlerImpl::Respond() {
//    // default respond for handlers  -  convert response to json and send
//    std::string responseStr;
//    response_.ToJSON(responseStr);
//    session_->Send(responseStr);
//}
