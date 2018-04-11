#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/exception.hpp>
#include <uhd/types/tune_request.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <iostream>

int UHD_SAFE_MAIN(int argc, char *argv[]) {
    uhd::set_thread_priority_safe();

    std::string device_args("");
    std::string subdev("A:0");
    std::string ant("TX/RX");
    std::string ref("internal");
    std::string args;
    std::string wire;

    double rate(1e6);
    double freq(915e6);
    double gain(10);
    double bw(1e6);

    //create a usrp device
    std::cout << std::endl;
    std::cout << boost::format("USRP device make hiij baina: %s...") % device_args << std::endl;
    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(device_args);



    //set the sample rate
    if (rate <= 0.0) {
        std::cerr << "Tohiromjtoi Sample Rate oruulna uu" << std::endl;
        return ~0;
    }

    // Sample rate tohiruulah
    std::cout << boost::format("RX hurdiig tohiruulah: %f Msps...") % (rate / 1e6) << std::endl;
    usrp->set_rx_rate(rate);
    std::cout << boost::format("Undsen RX rate: %f Msps...") % (usrp->get_rx_rate() / 1e6) << std::endl << std::endl;

    // Center dawtamjiig tohiruulah
    std::cout << boost::format("RX dawtamjiig tohiruulah: %f MHz...") % (freq / 1e6) << std::endl;
    uhd::tune_request_t tune_request(freq);
    usrp->set_rx_freq(tune_request);
    std::cout << boost::format("Undsen RX dawtamj: %f MHz...") % (usrp->get_rx_freq() / 1e6) << std::endl << std::endl;

    // RF osgoltiin chadliig tohiruulah
    std::cout << boost::format("RX osgoltiig tohiruulah: %f dB...") % gain << std::endl;
    usrp->set_rx_gain(gain);
    std::cout << boost::format("Undsen RX osgolt: %f dB...") % usrp->get_rx_gain() << std::endl << std::endl;

    // IF shuultuuriin dawtamjiig tohiruulah
    std::cout << boost::format("RX Bandwidth tohiruulah: %f MHz...") % (bw / 1e6) << std::endl;
    usrp->set_rx_bandwidth(bw);
    std::cout << boost::format("Undsen RX Bandwidth: %f MHz...") % (usrp->get_rx_bandwidth() / 1e6) << std::endl << std::endl;

    // Antenna tohiruulah
    std::cout << boost::format("RX Antenna tohiruulah: %s") % ant << std::endl;
    usrp->set_rx_antenna(ant);
    std::cout << boost::format("Undsen RX Antenna: %s") % usrp->get_rx_antenna() << std::endl << std::endl;
    
    //Stream hiih
    uhd::stream_args_t stream_args("fc32", wire); //complex floats
    uhd::tx_streamer::sptr tx_stream = usrp->get_tx_stream(stream_args);

    return EXIT_SUCCESS;
}
