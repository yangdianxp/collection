#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "config.h"

using namespace std;

bool config::load_file(string file_path)
{
	try {
		boost::property_tree::ptree root;
		boost::property_tree::read_json<boost::property_tree::ptree>(file_path, root);
		// boost::property_tree::write_json<boost::property_tree::ptree>(cout, root);
		auto all_serial_port = root.get_child("serial_port");
		for (auto it = all_serial_port.begin(); it != all_serial_port.end(); ++it)
		{
			boost::property_tree::write_json<boost::property_tree::ptree>(cout, it->second);
			cout << it->second.get<string>("port_name") << endl;
			cout << it->second.get<string>("port_name1") << endl;
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		exit(-1);
	}
	return true;
}
