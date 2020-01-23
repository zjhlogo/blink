// gradient.h
//
// Copyright (C) 2019, 2020 zjhlogo
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is zjhlogo@gmzigail.com (for great email, take
// off every 'zig'.)
//

#ifndef NOISE_MODULE_GRADIENT_H
#define NOISE_MODULE_GRADIENT_H

#include "modulebase.h"

namespace noise
{

    namespace module
    {

        /// @addtogroup libnoise
        /// @{

        /// @addtogroup modules
        /// @{

        /// @defgroup generatormodules Generator Modules
        /// @addtogroup generatormodules
        /// @{

        /// Noise module that generate gradient map
        ///
        /// @image html gradient.png
        ///
        /// The Gradient function accepts a line segment in the form of
        /// (x1,x2, y1,y2) for the 2D case, extended to (x1,x2, y1,y2, z1,z2) for
        /// the 3D case. The point formed by (x1,y1) delineates the start of the
        /// line segment, which is mapped to -1. The point formed by (x2,y2) is
        /// the end of the line segment that maps to 1.
        ///
        /// This noise module does not require any source modules.
        class Gradient : public ModuleBase
        {

        public:
            /// Constructor.
            ///
            /// (x1,x2, y1,y2) for the 2D case, extended to (x1,x2, y1,y2, z1,z2) for
            /// the 3D case.
            Gradient(double x1, double x2, double y1, double y2, double z1 = 0.0, double z2 = 0.0);

            /// Set the start point and end point for gradient
            ///
            /// (x1,x2, y1,y2) for the 2D case, extended to (x1,x2, y1,y2, z1,z2) for
            /// the 3D case.
            void setGradient(double x1, double x2, double y1, double y2, double z1 = 0.0, double z2 = 0.0);

            virtual double getValue(double x, double y, double z) const override;

        protected:
            double m_gx1, m_gy1, m_gz1;
            double m_x, m_y, m_z;
            double m_vlen;

        };

        /// @}

        /// @}

        /// @}

    } // namespace module

} // namespace noise

#endif
