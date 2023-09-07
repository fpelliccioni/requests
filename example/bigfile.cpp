//
// Copyright (c) 2022 Klemens Morgenstern (klemens.morgenstern@gmx.net)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <vector>
#include <thread>

#include <boost/asio/use_future.hpp>
#include <boost/requests/json.hpp>
#include <boost/requests/method.hpp>
#include <boost/requests/request_parameters.hpp>
#include <boost/requests/service.hpp>

using namespace boost;

int main(int argc, char * argv[]) {
  asio::io_context ctx;
  requests::default_session().options().enforce_tls = false;

  auto r = requests::get(urls::url_view("http://localhost:8080/bigfile?total_size=100000000&chunk_size=500000&delay_ms=10"));
  auto buf = r.buffer.cdata();
  std::cout << "Received big file response. Size: " << buf.size() << std::endl;

  return 0;
}
