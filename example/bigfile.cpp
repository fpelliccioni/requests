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
  auto tk = asio::bind_executor(ctx, asio::use_future);

  using FutureResponseType = decltype(requests::async_get(urls::url_view{"http://bla.com"}, {}, tk));

  auto big_file_future = requests::async_get(urls::url_view{"http://localhost:8080/bigfile?total_size=10000000000&chunk_size=5000&delay_ms=100"}, {}, tk);

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  std::vector<FutureResponseType> echo_futures;
  for (int i = 0; i < 10; ++i) {
      echo_futures.push_back(requests::async_get(urls::url_view{"http://localhost:8080/echo"}, {}, tk));
  }

  ctx.run();

  for (auto &fut : echo_futures) {
      auto response = fut.get();
      std::cout << "Received echo response: " << response.headers << response.string_view() << std::endl;
  }

  auto big_response = big_file_future.get();
  std::cout << "Received big file response. Size: " << big_response.string_view().size() << std::endl;

  return 0;
}
