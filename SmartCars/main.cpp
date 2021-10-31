#include "SmartCars.h"
#include <QtWidgets/QApplication>
#include <cstdlib>  // for std::exit
#include <iostream> // for std::cerr
// Allow any format of input files (XML, PBF, ...)
#include <osmium/io/any_input.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SmartCars w;
    w.show();
    // The parameter bbox is required, and must be of the form min_lon,min_lat,max_lon,max_lat.
    // Pour le campus -> https://api.openstreetmap.org/api/0.6/map?bbox=7.324814907320534,47.73479932638564,7.343306986600456,47.744504013401915
    try {
        // The Reader is initialized here with an osmium::io::File, but could
        // also be directly initialized with a file name.
        //osmium::io::File input_file{argv[1]};
        //osmium::io::Reader reader{ input_file };

        // OSM data comes in buffers, read until there are no more.
        //while (osmium::memory::Buffer buffer = reader.read()) {
            // do nothing
        //}

        // You do not have to close the Reader explicitly, but because the
        // destructor can't throw, you will not see any errors otherwise.
        //reader.close();
    }
    catch (const std::exception& e) {
        // All exceptions used by the Osmium library derive from std::exception.
        //std::cerr << e.what() << '\n';
        //std::exit(1);
    }
    return a.exec();
}
