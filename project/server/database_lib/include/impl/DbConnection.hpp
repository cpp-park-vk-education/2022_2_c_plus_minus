#include <fstream>
#include <memory>
#include <string>

template <typename C>
class DBConnection {
    const std::string path_;
    std::unique_ptr<C> connect_;
    std::string LoadConfig(const std::string& path) const;

   public:
    DBConnection(const std::string&);
    ~DBConnection();

    C& GetConnection();
};
