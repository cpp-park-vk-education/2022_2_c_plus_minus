#pragma once

class Handler {
   public:
    Handler(const Handler&) = delete;
    Handler& operator=(const Handler&) = delete;
    Handler() = default;
    virtual ~Handler() = default;
    virtual void Run() = 0;

   protected:
    virtual void Respond() = 0;
    virtual bool Check() = 0;
};
