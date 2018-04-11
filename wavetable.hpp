//
// Copyright 2010-2012,2014 Ettus Research LLC
// Copyright 2018 Ettus Research, a National Instruments Company
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

#include <string>
#include <cmath>
#include <complex>
#include <vector>
#include <stdexcept>

//static const size_t wave_table_len = 16384*4;
//
static const size_t wave_table_len = 8192;

class wave_table_class{
public:
    wave_table_class(const std::string &wave_type, const float ampl):
        _wave_table(wave_table_len)
    {
        //compute real wave table with 1.0 amplitude
        std::vector<float> real_wave_table(wave_table_len);
        std::vector<float> message_wave_table(wave_table_len);
        std::vector<float> wave_integral(wave_table_len);
        double xariu = 0;
        
        static const double tau = 2*std::acos(-1.0);
            for (size_t i = 0; i < wave_table_len; i++)
                message_wave_table[i] = std::sin((tau*i)/wave_table_len);
            
            for(int i = 0; i < wave_table_len; i++){
                for (int i1 = i; i1 < wave_table_len; i1++) {
                    xariu += message_wave_table[i1];
                }
                wave_integral[i] = xariu/*(2*M_PI)*/;
                xariu = 0;
            }
        
        if (wave_type == "CONST"){
            for (size_t i = 0; i < wave_table_len; i++)
                real_wave_table[i] = 1.0;
        }
        else if (wave_type == "SQUARE"){
            for (size_t i = 0; i < wave_table_len; i++)
                real_wave_table[i] = (i < wave_table_len/2)? 0.0 : 1.0;
        }
        else if (wave_type == "RAMP"){
            for (size_t i = 0; i < wave_table_len; i++)
                real_wave_table[i] = 2.0*i/(wave_table_len-1) - 1.0;
        }
        else if (wave_type == "SINE"){
            static const double tau = 2*std::acos(-1.0);
            for (size_t i = 0; i < wave_table_len; i++)
                real_wave_table[i] = std::sin((tau*i+(wave_integral[i]*75000))/wave_table_len);
        }
//         else if (wave_type == "SINE"){
//             static const double tau = 2*std::acos(-1.0);
//             for (size_t i = 0; i < wave_table_len; i++)
//                 real_wave_table[i] = std::sin((tau*i)/wave_table_len);
//         }
        else throw std::runtime_error("unknown waveform type: " + wave_type);

        //compute i and q pairs with 90% offset and scale to amplitude
        for (size_t i = 0; i < wave_table_len; i++){
            const size_t q = (i+(3*wave_table_len)/4)%wave_table_len;
            _wave_table[i] = std::complex<float>(
                ampl*real_wave_table[i],
                ampl*real_wave_table[q]
            );
        }
    }

    inline std::complex<float> operator()(const size_t index) const{
        return _wave_table[index % wave_table_len];
    }

private:
    std::vector<std::complex<float> > _wave_table;
};
