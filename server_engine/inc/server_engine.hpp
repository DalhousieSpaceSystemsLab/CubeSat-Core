#ifndef __SERVER_ENGINE_HPP__
#define __SERVER_ENGINE_HPP__
#ifdef __cplusplus


class server_engine
{
  public:
    virtual int  transmit(const char *bytes, unsigned int blen) = 0;
    virtual int  receive_callback(void)                         = 0;
    virtual bool receive_complete(void)                         = 0;
    virtual bool transmit_complete(void)                        = 0;
    virtual bool is_busy(void)                                  = 0;
};

#endif /* #ifdef __cplusplus */
#endif /* __SERVER_ENGINE_HPP__ */
