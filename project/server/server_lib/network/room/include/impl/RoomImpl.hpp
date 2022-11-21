#pragma once

#include <map>
#include <memory>
#include <string>

#include "Room.hpp"
#include "SessionImpl.hpp"

class RoomImpl : public Room {
   public:
    RoomImpl(std::uint64_t id, std::uint64_t max_users,
             const std::string& name);
    virtual ~RoomImpl();

    std::string GetName() const;
    std::uint64_t GetId() const;
    std::uint64_t GetUsersCount() const;
    std::uint64_t GetUsersMax() const;
    std::shared_ptr<SessionImpl> GetSession(std::uint64_t id) const;
    std::vector<std::shared_ptr<SessionImpl>> GetSessions() const;

    virtual bool NewSession(const std::shared_ptr<SessionImpl>& session);
    virtual bool DelSession(std::uint64_t id);

    // send msg to a single user by id
    virtual void Send(const std::string& message, std::uint64_t id) override;
    // send message for all users in room except one (initiator)
    virtual void SendAllExcept(const std::string& message, std::uint64_t id) override;

   protected:
    const std::uint64_t id_ = 0;
    std::uint64_t users_count = 0;
    std::uint64_t users_max_ = 0;
    std::string name_;
    // store sessions of room users in map
    std::map<std::uint64_t, std::shared_ptr<SessionImpl>> sessions_;
    // multiple users may concurrently access to sessions, need mutex
    mutable std::mutex mutex_;
};
