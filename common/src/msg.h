#ifndef __MSG_H__
#define __MSG_H__

#include <string.h>
#include <stdio.h>

struct MsgHeader
{
  int fd;
  int lenth;
};


class CMsg
{
public:
  CMsg(char* mem, unsigned int lenth, unsigned int size, int flag) :
    m_mem(mem),
    m_buf_len(lenth),
    m_mem_size(size),
    m_flag(flag)
  {}
  ~CMsg()
  {}

  inline int read(char** data, unsigned int& lenth)
  {
    unsigned int total_len = 0;
    if (m_flag != 2)
    {
      return 1;
    }
    if (m_buf_len < sizeof(lenth))
    {
      return 2;
    }
    memcpy(&lenth, m_mem, sizeof(lenth));
    if (lenth == 0)
    {
      *data = NULL;
    }
    else
    {
      *data = m_mem + sizeof(int);
    }
    
    total_len = sizeof(int) + lenth;
    m_mem = m_mem + total_len;
    m_buf_len -= total_len;

    return 0;
  }

  inline int write(const char* data, unsigned int lenth)
  {
    unsigned int total_len = sizeof(lenth)+lenth;
    if (m_flag != 1)
    {
      return 1;
    }
    if (m_buf_len + total_len > m_mem_size)
    {
      return 1;
    }
    memcpy(m_mem, &lenth, sizeof(lenth));
    m_mem += sizeof(lenth);
    if (lenth != 0)
    {
      memcpy(m_mem, data, lenth);
    }
    
    m_mem += lenth;
    m_buf_len += total_len;

    return 0;
  }

  inline unsigned int get_len()
  {
    return m_buf_len;
  }

  inline unsigned int get_size()
  {
    return m_mem_size;
  }

  inline char* get_mem()
  {
    return m_mem;
  }

protected:
  char* m_mem;            // init for shm
  unsigned int m_buf_len;
  unsigned int m_mem_size;
  int m_flag;             // read only or write only
};


#endif
