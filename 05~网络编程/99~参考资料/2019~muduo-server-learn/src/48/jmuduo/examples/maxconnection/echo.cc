#include "echo.h"

#include <muduo/base/Logging.h>

#include <boost/bind.hpp>

using namespace muduo;
using namespace muduo::net;

EchoServer::EchoServer(EventLoop* loop,
                       const InetAddress& listenAddr,
                       int maxConnections)
  : loop_(loop),
    server_(loop, listenAddr, "EchoServer"),
    numConnected_(0),
    kMaxConnections_(maxConnections)
{
  server_.setConnectionCallback(
      boost::bind(&EchoServer::onConnection, this, _1));
  server_.setMessageCallback(
      boost::bind(&EchoServer::onMessage, this, _1, _2, _3));
}

void EchoServer::start()
{
  server_.start();
}

void EchoServer::onConnection(const TcpConnectionPtr& conn)
{
  LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
           << conn->localAddress().toIpPort() << " is "
           << (conn->connected() ? "UP" : "DOWN");

  if (conn->connected())
  {
    ++numConnected_; // ��ǰ��������1
    if (numConnected_ > kMaxConnections_)
    {
      conn->send("too many connections\r\n", strlen("too many connections\r\n"));
      conn->shutdown(); // �Ͽ�����
    }
  }
  else
  {
    --numConnected_; // ��ǰ��������1
  }
  LOG_INFO << "numConnected = " << numConnected_;
}

void EchoServer::onMessage(const TcpConnectionPtr& conn,
                           Buffer* buf,
                           Timestamp time)
{
  string msg(buf->retrieveAllAsString());
  LOG_INFO << conn->name() << " echo " << msg.size() << " bytes at " << time.toString();
  conn->send(msg);
}

