#include <string>

class Server {
   public:
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    virtual ~Server() = default;
    Server() = default;

    virtual void Run() = 0;
    virtual void Stop() = 0;

   protected:
};