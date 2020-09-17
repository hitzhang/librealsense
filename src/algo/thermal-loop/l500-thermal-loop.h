// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Intel Corporation. All Rights Reserved.

#pragma once

#include <vector>
#include "../../types.h"

namespace librealsense {
namespace algo {
namespace thermal_loop {
namespace l500 {

#pragma pack( push, 1 )

// This struct based on RGB_Thermal_Info_CalibInfo table.
// The table contains 29 equally spaced bins between min & max temperature.
// Center of each bin has a set of 4 transformation parameters.
// The transformation maps a point in the RGB image in a given temperature to its expected
// Location in the temperature in which the RGB module was calibrated.
// Reference on:
// https://rsconf.intel.com/display/L500/0x317+RGB+Thermal+Table


class thermal_calibration_table
{
public:
    static const int id = 0x317;
    static const int resolution = 29;

    struct header
    {
        float min_temp;
        float max_temp;
        float reference_temp;  // not used
        float valid;           // not used
    };

    struct temp_data
    {
        float scale;
        float sheer; // parameters which affect offset that are not in use
        float tx;
        float ty;
    };

    table_meta_data md;
    std::vector< temp_data > vals;

    static thermal_calibration_table parse_thermal_table ( const std::vector< byte > & data );

    double get_current_thermal_scale( const double& hum_temp ) const;

    std::vector< byte > build_raw_data() const;
};
#pragma pack( pop )
inline bool operator==( const thermal_calibration_table & lhs, const thermal_calibration_table & rhs )
{
    if(lhs.vals.size()!=rhs.vals.size())
        return false;

    if( lhs.header.max_temp != rhs.header.max_temp || lhs.header.min_temp != rhs.header.min_temp
        || lhs.header.reference_temp != rhs.header.reference_temp || lhs.header.valid != rhs.header.valid )
        return false;

    for( auto i = 0; i < rhs.vals.size(); i++ )
    {
        if( lhs.vals[i].scale != rhs.vals[i].scale || lhs.vals[i].sheer != rhs.vals[i].sheer
            || lhs.vals[i].tx != rhs.vals[i].tx || lhs.vals[i].ty != rhs.vals[i].ty )
            return false;
    }
    return true;
}

}  // namespace l500
}  // namespace thermal_loop
}  // namespace algo
}  // namespace librealsense
