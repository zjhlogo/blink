// scalepoint.h
//
// Copyright (C) 2003, 2004 Jason Bevins
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
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#ifndef NOISE_MODULE_SCALEDOMAIN_H
#define NOISE_MODULE_SCALEDOMAIN_H

#include "modulebase.h"

namespace noise
{

    namespace module
    {

        /// @addtogroup libnoise
        /// @{

        /// @addtogroup modules
        /// @{

        /// @addtogroup transformermodules
        /// @{

        /// Default scaling factor applied to the @a x coordinate for the
        /// noise::module::ScalePoint noise module.
        const double DEFAULT_SCALE_POINT_X = 1.0;

        /// Default scaling factor applied to the @a y coordinate for the
        /// noise::module::ScalePoint noise module.
        const double DEFAULT_SCALE_POINT_Y = 1.0;

        /// Default scaling factor applied to the @a z coordinate for the
        /// noise::module::ScalePoint noise module.
        const double DEFAULT_SCALE_POINT_Z = 1.0;

        /// Noise module that scales the coordinates of the input value before
        /// returning the output value from a source module.
        ///
        /// @image html modulescalepoint.png
        ///
        /// The getValue() method multiplies the ( @a x, @a y, @a z ) coordinates
        /// of the input value with a scaling factor before returning the output
        /// value from the source module.  To set the scaling factor, call the
        /// SetScale() method.  To set the scaling factor to apply to the
        /// individual @a x, @a y, or @a z coordinates, call the SetXScale(),
        /// SetYScale() or SetZScale() methods, respectively.
        ///
        /// This noise module requires one source module.
        class ScaleDomain : public ModuleBase
        {

        public:
            /// Constructor.
            ///
            /// The default scaling factor applied to the @a x coordinate is set
            /// to noise::module::DEFAULT_SCALE_POINT_X.
            ///
            /// The default scaling factor applied to the @a y coordinate is set
            /// to noise::module::DEFAULT_SCALE_POINT_Y.
            ///
            /// The default scaling factor applied to the @a z coordinate is set
            /// to noise::module::DEFAULT_SCALE_POINT_Z.
            ScaleDomain(const noise::ScalarParameter& src);
            ScaleDomain(const noise::ScalarParameter& src, const noise::ScalarParameter& scale);
            ScaleDomain(const noise::ScalarParameter& src, const noise::ScalarParameter& xScale, const noise::ScalarParameter& yScale, const noise::ScalarParameter& zScale);

            /// Sets the scaling factor to apply to the @a x coordinate of the
            /// input value.
            ///
            /// @param xScale The scaling factor to apply to the @a x coordinate.
            ///
            /// The getValue() method multiplies the ( @a x, @a y, @a z )
            /// coordinates of the input value with a scaling factor before
            /// returning the output value from the source module.
            void SetXScale(const noise::ScalarParameter& xScale);

            /// Returns the scaling factor applied to the @a x coordinate of the
            /// input value.
            ///
            /// @returns The scaling factor applied to the @a x coordinate.
            const noise::ScalarParameter& GetXScale() const;

            /// Sets the scaling factor to apply to the @a y coordinate of the
            /// input value.
            ///
            /// @param yScale The scaling factor to apply to the @a y coordinate.
            ///
            /// The getValue() method multiplies the ( @a x, @a y, @a z )
            /// coordinates of the input value with a scaling factor before
            /// returning the output value from the source module.
            void SetYScale(const noise::ScalarParameter& yScale);

            /// Returns the scaling factor applied to the @a y coordinate of the
            /// input value.
            ///
            /// @returns The scaling factor applied to the @a y coordinate.
            const noise::ScalarParameter& GetYScale() const;

            /// Sets the scaling factor to apply to the @a z coordinate of the
            /// input value.
            ///
            /// @param zScale The scaling factor to apply to the @a z coordinate.
            ///
            /// The getValue() method multiplies the ( @a x, @a y, @a z )
            /// coordinates of the input value with a scaling factor before
            /// returning the output value from the source module.
            void SetZScale(const noise::ScalarParameter& zScale);

            /// Returns the scaling factor applied to the @a z coordinate of the
            /// input value.
            ///
            /// @returns The scaling factor applied to the @a z coordinate.
            const noise::ScalarParameter& GetZScale() const;

            /// Sets the scaling factor to apply to the input value.
            ///
            /// @param scale The scaling factor to apply.
            ///
            /// The getValue() method multiplies the ( @a x, @a y, @a z )
            /// coordinates of the input value with a scaling factor before
            /// returning the output value from the source module.
            void SetScale(const noise::ScalarParameter& scale);

            /// Sets the scaling factor to apply to the ( @a x, @a y, @a z )
            /// coordinates of the input value.
            ///
            /// @param xScale The scaling factor to apply to the @a x coordinate.
            /// @param yScale The scaling factor to apply to the @a y coordinate.
            /// @param zScale The scaling factor to apply to the @a z coordinate.
            ///
            /// The getValue() method multiplies the ( @a x, @a y, @a z )
            /// coordinates of the input value with a scaling factor before
            /// returning the output value from the source module.
            void SetScale(const noise::ScalarParameter& xScale, const noise::ScalarParameter& yScale, const noise::ScalarParameter& zScale);

            virtual double getValue(double x, double y, double z) const override;

        protected:
            noise::ScalarParameter m_source;

            /// Scaling factor applied to the @a x coordinate of the input value.
            noise::ScalarParameter m_xScale;

            /// Scaling factor applied to the @a y coordinate of the input value.
            noise::ScalarParameter m_yScale;

            /// Scaling factor applied to the @a z coordinate of the input value.
            noise::ScalarParameter m_zScale;
        };

        /// @}

        /// @}

        /// @}

    } // namespace module

} // namespace noise

#endif
