#ifndef __C_CONFIG_READER__H_
#define __C_CONFIG_READER__H_

#include <boost/program_options.hpp>

#include <iostream>
#include <fstream>
#include <string>

#define REGISTER_CONFIG_VAR(var_name) \
  (this)->desc.add_options() \
  (var_name, boost::program_options::value<std::string>()->default_value("null")) // NOLINT

#define GET_CONFIG_VALUE(var_name) \
  *(boost::unsafe_any_cast<std::string>(&((g_config).vm[var_name].value())))

class CConfigReader {
  public:

  inline bool parse_config_file(std::string pathfile) {
    std::ifstream infile(pathfile.c_str());
    if (!infile)
      return false;

    boost::program_options::store(
      boost::program_options::parse_config_file(infile, desc), vm);
    boost::program_options::notify(vm);
	infile.close();
    return true;
  }

  inline bool parse_cmd_line(int argc, char ** argv) {
    // You can register config variable here
    // {
    REGISTER_CONFIG_VAR("pid_file");

    // }

    boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);
    return true;
  }

  boost::program_options::options_description desc;

  boost::program_options::variables_map vm;
};

extern CConfigReader g_config;

#endif
