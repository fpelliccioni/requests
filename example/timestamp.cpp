//
// Copyright (c) 2022 Klemens Morgenstern (klemens.morgenstern@gmx.net)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>

#include <boost/requests/method.hpp>
#include <boost/requests/service.hpp>

using namespace boost;

int main(int argc, char * argv[]) {
  asio::io_context ctx;
  requests::default_session().options().enforce_tls = false;

  auto r = requests::get(urls::url_view("http://localhost:8080/timestamp"));
  std::cout << r.result_code() << std::endl;
  std::cout << r.headers["Content-Type"] << std::endl;
  std::cout << r.string_view() << std::endl;
  ctx.run();
  return 0;
}
