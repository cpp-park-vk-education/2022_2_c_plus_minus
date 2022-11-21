class Connection {
   public:
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
    Connection() = default;
    virtual void Start() = 0;
    virtual void Stop() = 0;

   protected:
    virtual void Read() = 0;
    virtual void ReadHandler() = 0;
    virtual void Write() = 0;
    virtual void WriteHandler() = 0;
};