#include "selenium_server.hpp"
#include <crails/getenv.hpp>
#include <filesystem>

using namespace Crails::Tests;
using namespace std;

unique_ptr<SeleniumServer> SeleniumServer::instance;

SeleniumServer::SeleniumServer()
{
  error_code ec;
  string java_path   = Crails::getenv("JAVA_PATH", "/usr/bin/java");
  string server_path = Crails::getenv("SELENIUM_STANDALONE_SERVER_PATH",
    filesystem::current_path().string() + "/spec/selenium-server-standalone.jar"
  );

  java_path = filesystem::canonical(java_path, ec).string();
  if (ec)
    throw runtime_error(string("java jre not found at " + java_path + " (use JAVA_PATH environment variable)"));
  server_path = filesystem::canonical(server_path, ec).string();
  if (ec)
    throw runtime_error(string("selenium standalone server jar not found at " + server_path + " (use SELENIUM_STANDALONE_SERVER_PATH environment variable)"));

  process.reset(
    new boost::process::child(
      java_path + " -jar " + server_path,
      boost::process::std_in.close(),
      boost::process::std_out > boost::process::null,
      boost::process::std_err > boost::process::null
    )
  );
}
