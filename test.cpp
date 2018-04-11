#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/stream.hpp>
#include <uhd/convert.hpp>
#include <stdio.h>
#include <string.h>
#include <complex.h>
#include <unistd.h>

int main (void)
{   
    // desired data rate in samples per seconds
    double datarate = 200000.0;
    double frequency = 1490000000.0;
    double timeout = 0.1;
    
    // how many samples should be read out at once
    const size_t max_samps_per_packet = 25000;
    // number of read / write cycles that should be done
    const int num_packets = 10000;
    // counting variables
    size_t i ;
    size_t rx_num , tx_num ;
    
    // default device
    std :: string args(" ");
    std::string ant("TX/RX");
    uhd :: usrp :: multi_usrp :: sptr usrp = uhd :: usrp :: multi_usrp :: make(args);
    
    // set datarates
    usrp -> set_rx_rate (datarate);
    usrp -> set_tx_rate (datarate);
    
    // set center frequencys
    usrp -> set_rx_freq(frequency);
    usrp -> set_tx_freq(frequency);
    
    // no amplification needed
    usrp -> set_rx_gain (10.0);
    usrp -> set_tx_gain (10.0);
    // Antenna tohiruulah
    std::cout << boost::format("RX Antenna tohiruulah: %s") % ant << std::endl;
    usrp->set_tx_antenna(ant);
    std::cout << boost::format("Undsen RX Antenna: %s") % usrp->get_tx_antenna() << std::endl << std::endl;
    
    // space to store metadata
    uhd :: rx_metadata_t rx_md ;
    uhd :: tx_metadata_t tx_md ;
    uhd :: async_metadata_t async_md ;
        
    // send as soon as possible ( no time tagging )
    tx_md.has_time_spec = false ;
    
    // buffers for sending and receiving
    std :: vector < std :: complex < double > > rx_buff (
    max_samps_per_packet ) ;
    std :: vector < std :: complex < double > > tx_buff (
    max_samps_per_packet ) ;
    
    // create send signal inside the buffer
    for (i =0; i < max_samps_per_packet ; i ++) 
    {
        // create complex cosine ( alternating +/ -1+0 i )
        tx_buff[i] = std :: complex < double >((( i %2 == 0) ? -1 : 1) ,
        0);
    }
        // start streaming
        printf ( " start streaming \n");
        usrp -> issue_stream_cmd (uhd :: stream_cmd_t :: STREAM_MODE_START_CONTINUOUS);
        
    for ( i = num_packets; i >0 ; i --) 
    {
        // receive asynchronous messages ( errors during transmission ) with 1 ms timeout
        if ( usrp -> get_device () -> recv_async_msg ( async_md , 0.001) &&async_md . event_code != uhd ::async_metadata_t::EVENT_CODE_BURST_ACK ) 
        {
            printf ( " tx error \n");
        }
        
        // if received less samples than transmitted , print out the numbers
        if (rx_num < max_samps_per_packet ) 
        {
        printf (" send : %d , recv : %d , max :% d \ n " ,( int ) tx_num ,(int)
        rx_num ,( int )max_samps_per_packet );
        }
        // when there are receive errors , print them out
        if ( rx_md.error_code != uhd :: rx_metadata_t :: ERROR_CODE_NONE) 
        {
        printf ( " rx error \n ");
        }
        // sleep some time to simulate data processing in MATLAB
        usleep (5000) ;
    }
    // signal the USRP to stop transmitting
    tx_md.end_of_burst = true;
    // stop receiving
    usrp -> issue_stream_cmd (uhd :: stream_cmd_t ::STREAM_MODE_STOP_CONTINUOUS);    
    printf ( " finished \n " );
    return 0;
}
    
    
