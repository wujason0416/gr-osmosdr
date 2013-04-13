/* -*- c++ -*- */
/*
 * Copyright 2012 Dimitri Stolnikov <horiz0n@gmx.net>
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

/*
 * config.h is generated by configure.  It contains the results
 * of probing for features, options etc.  It should be the first
 * file included in your .cc file.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <osmosdr_source_c_impl.h>
#include <gr_io_signature.h>
#include <gr_noise_source_c.h>
#include <gr_throttle.h>
#include <gnuradio/gr_constants.h>

#ifdef ENABLE_OSMOSDR
#include <osmosdr_src_c.h>
#endif

#ifdef ENABLE_FCD
#include <fcd_source.h>
#endif

#ifdef ENABLE_FILE
#include <file_source_c.h>
#endif

#ifdef ENABLE_RTL
#include <rtl_source_c.h>
#endif

#ifdef ENABLE_RTL_TCP
#include <rtl_tcp_source_c.h>
#endif

#ifdef ENABLE_UHD
#include <uhd_source_c.h>
#endif

#ifdef ENABLE_MIRI
#include <miri_source_c.h>
#endif

#ifdef ENABLE_HACKRF
#include <hackrf_source_c.h>
#endif

#include <osmosdr_arg_helpers.h>

/* This avoids throws in ctor of gr_hier_block2, as gnuradio is unable to deal
 with this behavior in a clean way. The GR maintainer Rondeau has been informed. */
#define WORKAROUND_GR_HIER_BLOCK2_BUG

/*
 * Create a new instance of osmosdr_source_c_impl and return
 * a boost shared_ptr.  This is effectively the public constructor.
 */
osmosdr_source_c_sptr
osmosdr_make_source_c (const std::string &args)
{
  return gnuradio::get_initial_sptr(new osmosdr_source_c_impl (args));
}

/*
 * The private constructor
 */
osmosdr_source_c_impl::osmosdr_source_c_impl (const std::string &args)
  : gr_hier_block2 ("osmosdr_source_c_impl",
        gr_make_io_signature (0, 0, 0),
        args_to_io_signature(args))
{
  size_t channel = 0;
  bool device_specified = false;

  std::vector< std::string > arg_list = args_to_vector(args);

  std::vector< std::string > dev_types;

#ifdef ENABLE_FILE
  dev_types.push_back("file");
#endif
#ifdef ENABLE_OSMOSDR
  dev_types.push_back("osmosdr");
#endif
#ifdef ENABLE_FCD
  dev_types.push_back("fcd");
#endif
#ifdef ENABLE_RTL
  dev_types.push_back("rtl");
#endif
#ifdef ENABLE_RTL_TCP
  dev_types.push_back("rtl_tcp");
#endif
#ifdef ENABLE_UHD
  dev_types.push_back("uhd");
#endif
#ifdef ENABLE_MIRI
  dev_types.push_back("miri");
#endif
#ifdef ENABLE_HACKRF
  dev_types.push_back("hackrf");
#endif

  std::cerr << "gr-osmosdr "
            << GR_OSMOSDR_VERSION " (" GR_OSMOSDR_LIBVER ") "
            << "gnuradio " << gr_version() << std::endl;
  std::cerr << "built-in device types: ";
  BOOST_FOREACH(std::string dev_type, dev_types)
    std::cerr << dev_type << " ";
  std::cerr << std::endl << std::flush;

  BOOST_FOREACH(std::string arg, arg_list) {
    dict_t dict = params_to_dict(arg);
    BOOST_FOREACH(std::string dev_type, dev_types) {
      if ( dict.count( dev_type ) ) {
        device_specified = true;
        break;
      }
    }
  }
#ifdef WORKAROUND_GR_HIER_BLOCK2_BUG
  try {
#endif
  std::vector< std::string > dev_list;
#ifdef ENABLE_OSMOSDR
  BOOST_FOREACH( std::string dev, osmosdr_src_c::get_devices() )
    dev_list.push_back( dev );
#endif
#ifdef ENABLE_FCD
  BOOST_FOREACH( std::string dev, fcd_source::get_devices() )
    dev_list.push_back( dev );
#endif
#ifdef ENABLE_RTL
  BOOST_FOREACH( std::string dev, rtl_source_c::get_devices() )
    dev_list.push_back( dev );
#endif
#ifdef ENABLE_UHD
  BOOST_FOREACH( std::string dev, uhd_source_c::get_devices() )
    dev_list.push_back( dev );
#endif
#ifdef ENABLE_MIRI
  BOOST_FOREACH( std::string dev, miri_source_c::get_devices() )
    dev_list.push_back( dev );
#endif
#ifdef ENABLE_HACKRF
  BOOST_FOREACH( std::string dev, hackrf_source_c::get_devices() )
  dev_list.push_back( dev );
#endif
//  std::cerr << std::endl;
//  BOOST_FOREACH( std::string dev, dev_list )
//    std::cerr << "'" << dev << "'" << std::endl;

  if (!device_specified) {
    if ( dev_list.size() )
      arg_list.push_back( dev_list.front() );
    else
      throw std::runtime_error("No supported devices found to pick from.");
  }

  BOOST_FOREACH(std::string arg, arg_list) {

    dict_t dict = params_to_dict(arg);

//    std::cerr << std::endl;
//    BOOST_FOREACH( dict_t::value_type &entry, dict )
//      std::cerr << "'" << entry.first << "' = '" << entry.second << "'" << std::endl;

    osmosdr_src_iface *iface = NULL;
    gr_basic_block_sptr block;

#ifdef ENABLE_OSMOSDR
    if ( dict.count("osmosdr") ) {
      osmosdr_src_c_sptr src = osmosdr_make_src_c( arg );
      block = src; iface = src.get();
    }
#endif

#ifdef ENABLE_FCD
    if ( dict.count("fcd") ) {
      fcd_source_sptr src = make_fcd_source( arg );
      block = src; iface = src.get();
    }
#endif

#ifdef ENABLE_FILE
    if ( dict.count("file") ) {
      file_source_c_sptr src = make_file_source_c( arg );
      block = src; iface = src.get();
    }
#endif

#ifdef ENABLE_RTL
    if ( dict.count("rtl") ) {
      rtl_source_c_sptr src = make_rtl_source_c( arg );
      block = src; iface = src.get();
    }
#endif

#ifdef ENABLE_RTL_TCP
    if ( dict.count("rtl_tcp") ) {
      rtl_tcp_source_c_sptr src = make_rtl_tcp_source_c( arg );
      block = src; iface = src.get();
    }
#endif

#ifdef ENABLE_UHD
    if ( dict.count("uhd") ) {
      uhd_source_c_sptr src = make_uhd_source_c( arg );
      block = src; iface = src.get();
    }
#endif

#ifdef ENABLE_MIRI
    if ( dict.count("miri") ) {
      miri_source_c_sptr src = make_miri_source_c( arg );
      block = src; iface = src.get();
    }
#endif

#ifdef ENABLE_HACKRF
    if ( dict.count("hackrf") ) {
      hackrf_source_c_sptr src = make_hackrf_source_c( arg );
      block = src; iface = src.get();
    }
#endif

    if ( iface != NULL && long(block.get()) != 0 ) {
      _devs.push_back( iface );

      for (size_t i = 0; i < iface->get_num_channels(); i++) {
#ifdef HAVE_IQBALANCE
        iqbalance_optimize_c_sptr iq_opt = iqbalance_make_optimize_c( 0 );
        iqbalance_fix_cc_sptr iq_fix = iqbalance_make_fix_cc();

        connect(block, i, iq_fix, 0);
        connect(iq_fix, 0, self(), channel++);

        connect(block, i, iq_opt, 0);
        msg_connect(iq_opt, "iqbal_corr", iq_fix, "iqbal_corr");

        _iq_opt.push_back( iq_opt.get() );
        _iq_fix.push_back( iq_fix.get() );
#else
        connect(block, i, self(), channel++);
#endif
      }
    } else if ( (iface != NULL) || (long(block.get()) != 0) )
      throw std::runtime_error("Eitner iface or block are NULL.");

  }

  if (!_devs.size())
    throw std::runtime_error("No devices specified via device arguments.");
#ifdef WORKAROUND_GR_HIER_BLOCK2_BUG
  } catch ( std::exception &ex ) {
    std::cerr << std::endl << "FATAL: " << ex.what() << std::endl << std::endl;

    /* we try to prevent the whole application from crashing by faking
     * the missing hardware (channels) with a gaussian noise source */
    gr_noise_source_c_sptr noise_source = \
        gr_make_noise_source_c( GR_GAUSSIAN, 10 );

    gr_throttle::sptr throttle = gr_make_throttle(sizeof(gr_complex), 1e6);

    connect(noise_source, 0, throttle, 0);

    size_t missing_chans = output_signature()->max_streams() - channel;

    std::cerr << "Trying to fill up " << missing_chans
              << " missing channel(s) with gaussian noise.\n"
              << "This is being done to prevent the application from crashing\n"
              << "due to a gnuradio bug. The maintainers have been informed.\n"
              << std::endl;

    for (size_t i = 0; i < missing_chans; i++)
      connect(throttle, 0, self(), channel++);
  }
#endif
}

size_t osmosdr_source_c_impl::get_num_channels()
{
  size_t channels = 0;

  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    channels += dev->get_num_channels();

  return channels;
}

#define NO_DEVICES_MSG  "FATAL: No device(s) available to work with."

osmosdr::meta_range_t osmosdr_source_c_impl::get_sample_rates()
{
  osmosdr::meta_range_t rates(0, 0, 0);

  if (!_devs.empty())
    rates = _devs[0]->get_sample_rates(); // assume same devices used in the group
#if 0
  else
    throw std::runtime_error(NO_DEVICES_MSG);
#endif
  return rates;
}

double osmosdr_source_c_impl::set_sample_rate(double rate)
{
  double sample_rate = 0;

  if (_sample_rate != rate) {
#if 0
    if (_devs.empty())
      throw std::runtime_error(NO_DEVICES_MSG);
#endif
    BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
      sample_rate = dev->set_sample_rate(rate);

#ifdef HAVE_IQBALANCE
    size_t channel = 0;
    BOOST_FOREACH( osmosdr_src_iface *dev, _devs ) {
      for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++) {
        if ( channel < _iq_opt.size() ) {
          iqbalance_optimize_c *opt = _iq_opt[channel];

          if ( opt->period() > 0 ) { /* optimize is enabled */
            opt->set_period( dev->get_sample_rate() / 5 );
            opt->reset();
          }
        }

        channel++;
      }
    }
#endif

    _sample_rate = sample_rate;
  }

  return sample_rate;
}

double osmosdr_source_c_impl::get_sample_rate()
{
  double sample_rate = 0;

  if (!_devs.empty())
    sample_rate = _devs[0]->get_sample_rate(); // assume same devices used in the group
#if 0
  else
    throw std::runtime_error(NO_DEVICES_MSG);
#endif
  return sample_rate;
}

osmosdr::freq_range_t osmosdr_source_c_impl::get_freq_range( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_freq_range( dev_chan );

  return osmosdr::freq_range_t();
}

double osmosdr_source_c_impl::set_center_freq( double freq, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        if ( _center_freq[ chan ] != freq ) {
          _center_freq[ chan ] = freq;
          return dev->set_center_freq( freq, dev_chan );
        }

  return 0;
}

double osmosdr_source_c_impl::get_center_freq( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_center_freq( dev_chan );

  return 0;
}

double osmosdr_source_c_impl::set_freq_corr( double ppm, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        if ( _freq_corr[ chan ] != ppm ) {
          _freq_corr[ chan ] = ppm;
          return dev->set_freq_corr( ppm, dev_chan );
        }

  return 0;
}

double osmosdr_source_c_impl::get_freq_corr( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_freq_corr( dev_chan );

  return 0;
}

std::vector<std::string> osmosdr_source_c_impl::get_gain_names( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_gain_names( dev_chan );

  return std::vector< std::string >();
}

osmosdr::gain_range_t osmosdr_source_c_impl::get_gain_range( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_gain_range( dev_chan );

  return osmosdr::gain_range_t();
}

osmosdr::gain_range_t osmosdr_source_c_impl::get_gain_range( const std::string & name, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_gain_range( name, dev_chan );

  return osmosdr::gain_range_t();
}

bool osmosdr_source_c_impl::set_gain_mode( bool automatic, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        if ( _gain_mode[ chan ] != automatic ) {
          _gain_mode[ chan ] = automatic;
          bool mode = dev->set_gain_mode( automatic, dev_chan );
          if (!automatic) // reapply gain value when switched to manual mode
            dev->set_gain( _gain[ chan ], dev_chan );
          return mode;
        }

  return false;
}

bool osmosdr_source_c_impl::get_gain_mode( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_gain_mode( dev_chan );

  return false;
}

double osmosdr_source_c_impl::set_gain( double gain, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        if ( _gain[ chan ] != gain ) {
          _gain[ chan ] = gain;
          return dev->set_gain( gain, dev_chan );
        }

  return 0;
}

double osmosdr_source_c_impl::set_gain( double gain, const std::string & name, size_t chan)
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->set_gain( gain, name, dev_chan );

  return 0;
}

double osmosdr_source_c_impl::get_gain( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_gain( dev_chan );

  return 0;
}

double osmosdr_source_c_impl::get_gain( const std::string & name, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_gain( name, dev_chan );

  return 0;
}

double osmosdr_source_c_impl::set_if_gain( double gain, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        if ( _if_gain[ chan ] != gain ) {
          _if_gain[ chan ] = gain;
          return dev->set_if_gain( gain, dev_chan );
        }

  return 0;
}

double osmosdr_source_c_impl::set_bb_gain( double gain, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        if ( _if_gain[ chan ] != gain ) {
          _if_gain[ chan ] = gain;
          return dev->set_bb_gain( gain, dev_chan );
        }

  return 0;
}

std::vector< std::string > osmosdr_source_c_impl::get_antennas( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_antennas( dev_chan );

  return std::vector< std::string >();
}

std::string osmosdr_source_c_impl::set_antenna( const std::string & antenna, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        if ( _antenna[ chan ] != antenna ) {
          _antenna[ chan ] = antenna;
          return dev->set_antenna( antenna, dev_chan );
        }

  return "";
}

std::string osmosdr_source_c_impl::get_antenna( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_antenna( dev_chan );

  return "";
}

void osmosdr_source_c_impl::set_iq_balance_mode( int mode, size_t chan )
{
#ifdef HAVE_IQBALANCE
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs ) {
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++) {
      if ( chan == channel++ ) {
        if ( chan < _iq_opt.size() && chan < _iq_fix.size() ) {
          iqbalance_optimize_c *opt = _iq_opt[chan];
          iqbalance_fix_cc *fix = _iq_fix[chan];

          if ( IQBalanceOff == mode  ) {
            opt->set_period( 0 );
            /* store current values in order to be able to restore them later */
            _vals[ chan ] = std::pair< float, float >( fix->mag(), fix->phase() );
            fix->set_mag( 0.0f );
            fix->set_phase( 0.0f );
          } else if ( IQBalanceManual == mode ) {
            if ( opt->period() == 0 ) { /* transition from Off to Manual */
              /* restore previous values */
              std::pair< float, float > val = _vals[ chan ];
              fix->set_mag( val.first );
              fix->set_phase( val.second );
            }
            opt->set_period( 0 );
          } else if ( IQBalanceAutomatic == mode ) {
            opt->set_period( dev->get_sample_rate() / 5 );
            opt->reset();
          }
        }
      }
    }
  }
#endif
}

void osmosdr_source_c_impl::set_iq_balance( const std::complex<double> &correction, size_t chan )
{
#ifdef HAVE_IQBALANCE
  size_t channel = 0;
  BOOST_FOREACH( osmosdr_src_iface *dev, _devs ) {
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++) {
      if ( chan == channel++ ) {
        if ( chan < _iq_opt.size() && chan < _iq_fix.size() ) {
          iqbalance_optimize_c *opt = _iq_opt[chan];
          iqbalance_fix_cc *fix = _iq_fix[chan];

          if ( opt->period() == 0 ) { /* automatic optimization desabled */
            fix->set_mag( correction.real() );
            fix->set_phase( correction.imag() );
          }
        }
      }
    }
  }
#endif
}
