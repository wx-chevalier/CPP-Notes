#ifndef MUDUO_BASE_ASYNCLOGGING_H
#define MUDUO_BASE_ASYNCLOGGING_H

#include <muduo/base/BlockingQueue.h>
#include <muduo/base/BoundedBlockingQueue.h>
#include <muduo/base/CountDownLatch.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Thread.h>

#include <muduo/base/LogStream.h>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace muduo
{

class AsyncLogging : boost::noncopyable
{
 public:

  AsyncLogging(const string& basename,
               size_t rollSize,
               int flushInterval = 3);

  ~AsyncLogging()
  {
    if (running_)
    {
      stop();
    }
  }

  // ��ǰ���������̵߳��ã���־����д����������
  void append(const char* logline, int len);

  void start()
  {
    running_ = true;
    thread_.start(); // ��־�߳�����
    latch_.wait();
  }

  void stop()
  {
    running_ = false;
    cond_.notify();
    thread_.join();
  }

 private:

  // declare but not define, prevent compiler-synthesized functions
  AsyncLogging(const AsyncLogging&);  // ptr_container
  void operator=(const AsyncLogging&);  // ptr_container

  // ������������̵߳��ã�������д����־�ļ���
  void threadFunc();

  typedef muduo::detail::FixedBuffer<muduo::detail::kLargeBuffer> Buffer;
  typedef boost::ptr_vector<Buffer> BufferVector;
  typedef BufferVector::auto_type BufferPtr; // �����ΪBuffer������ָ�룬�ܹ���Buffer�������ڣ�
                                             // ������C++11�е�unique_ptr���߱��ƶ�����
                                             // ������unique_ptr����ָ��һ�����󣬲��ܽ��и��Ʋ���ֻ�ܽ����ƶ�������

  const int flushInterval_; // ��ʱʱ�䣬��flushInterval_���ڣ�������ûд�����Խ��������е�����д���ļ���
  bool running_;
  string basename_;
  size_t rollSize_;
  muduo::Thread thread_;
  muduo::CountDownLatch latch_;  // ���ڵȴ��߳�����
  muduo::MutexLock mutex_;
  muduo::Condition cond_;
  BufferPtr currentBuffer_; // ��ǰ������
  BufferPtr nextBuffer_;    // Ԥ��������
  BufferVector buffers_;    // ��д���ļ����������Ļ�����
};

}
#endif  // MUDUO_BASE_ASYNCLOGGING_H
