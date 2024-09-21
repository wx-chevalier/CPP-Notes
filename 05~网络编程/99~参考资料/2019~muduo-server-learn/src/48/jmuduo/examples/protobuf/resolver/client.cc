#include <examples/protobuf/resolver/resolver.pb.h>

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpClient.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/protorpc/RpcChannel.h>

#include <arpa/inet.h>  // inet_ntop

#include <boost/bind.hpp>

#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

class RpcClient : boost::noncopyable
{
 public:
  RpcClient(EventLoop* loop, const InetAddress& serverAddr)
    : loop_(loop),
      client_(loop, serverAddr, "RpcClient"),
      channel_(new RpcChannel),
      got_(0),
      total_(0),
      stub_(get_pointer(channel_))
  {
    client_.setConnectionCallback(
        boost::bind(&RpcClient::onConnection, this, _1));
    client_.setMessageCallback(
        boost::bind(&RpcChannel::onMessage, get_pointer(channel_), _1, _2, _3));
    // client_.enableRetry();
  }

  void connect()
  {
    client_.connect();
  }

 private:
  void onConnection(const TcpConnectionPtr& conn)
  {
    if (conn->connected())
    {
      //channel_.reset(new RpcChannel(conn));
      channel_->setConnection(conn);
      total_ = 4;
      resolve("www.example.com");
      resolve("www.chenshuo.com");
      resolve("www.google.com");
      resolve("acme.chenshuo.org");
    }
  }

  void resolve(const std::string& host)
  {
    resolver::ResolveRequest request;
    request.set_address(host);
    resolver::ResolveResponse* response = new resolver::ResolveResponse;

    stub_.Resolve(NULL, &request, response,
        NewCallback(this, &RpcClient::resolved, response, host));
  }

  void resolved(resolver::ResolveResponse* resp, std::string host)
  {
    if (resp->resolved())
    {
      char buf[32];
      uint32_t ip = resp->ip(0);
      inet_ntop(AF_INET, &ip, buf, sizeof buf);

      LOG_INFO << "resolved " << host << " : " << buf << "\n"
               << resp->DebugString().c_str();
    }
    else
    {
      LOG_INFO << "resolved " << host << " failed";
    }

    if (++got_ >= total_)
      loop_->quit();
  }

  EventLoop* loop_;
  TcpClient client_;
  RpcChannelPtr channel_;
  int got_;
  int total_;
  resolver::ResolverService::Stub stub_;
};

int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid();
  if (argc > 1)
  {
    EventLoop loop;
    InetAddress serverAddr(argv[1], 2053);

    RpcClient rpcClient(&loop, serverAddr);
    rpcClient.connect();
    loop.loop();
  }
  else
  {
    printf("Usage: %s host_ip\n", argv[0]);
  }
}

