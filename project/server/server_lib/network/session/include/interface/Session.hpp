#include <string>

class Session {
public:
    Session() = default;
    virtual ~Session() = default;

    virtual void BindConnection() = 0;
    // for every request in queue call private method HandleRequest
    virtual void HandleRequests() = 0;
    virtual void Send(std::string message) = 0;
    virtual void SendAllExcept(const std::string &message) = 0;
    virtual std::uint64_t NewRoom(const std::string &name, std::size_t max_user_count) = 0;
    virtual void StopConnection();
protected:
    virtual void HandleRequest(Request&& request) = 0;
};