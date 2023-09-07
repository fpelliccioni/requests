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

  requests::session hc{ctx};
  hc.options().enforce_tls = false;
  // requests::default_session().options().enforce_tls = false;

  using FutureResponseType = decltype(requests::async_get(urls::url_view{"http://bla.com"}, {}, tk));

  std::cout << "before calling async_get bigfile" << std::endl;
  auto big_file_future = requests::async_get(hc, urls::url_view{"http://localhost:8080/bigfile?total_size=100000000&chunk_size=500000&delay_ms=10"}, {}, tk);

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  std::vector<FutureResponseType> echo_futures;
  for (int i = 0; i < 10; ++i) {
      std::cout << "before calling async_get timestamp" << std::endl;
      echo_futures.push_back(requests::async_get(hc, urls::url_view{"http://localhost:8080/timestamp"}, {}, tk));
  }

  ctx.run();

  for (auto &fut : echo_futures) {
    std::cout << "before .get timestamp future" << std::endl;
    auto response = fut.get();
    std::cout << "Received timestamp response: " << response.headers << response.string_view() << std::endl;
  }

  std::cout << "before .get big_file future" << std::endl;
  auto big_response = big_file_future.get();
  std::cout << "Received big file response. Size: " << big_response.string_view().size() << std::endl;

  return 0;
}
