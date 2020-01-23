// scalebias.h
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

#ifndef NOISE_MODULE_SCALEBIAS_H
#define NOISE_MODULE_SCALEBIAS_H

#include "modulebase.h"

namespace noise
{

    namespace module
    {

        /// @addtogroup libnoise
        /// @{

        /// @addtogroup modules
        /// @{

        /// @addtogroup modifiermodules
        /// @{

        /// Default bias for the noise::module::ScaleBias noise module.
        const double DEFAULT_BIAS = 0.0;

        /// Default scale for the noise::module::ScaleBias noise module.
        const double DEFAULT_SCALE = 1.0;

        /// Noise module that applies a scaling factor and a bias to the output
        /// value from a source module.
        ///
        /// @image html modulescalebias.png
        ///
        /// The getValue() method retrieves the output value from the source
        /// module, multiplies it with a scaling factor, adds a bias to it, then
        /// outputs the value.
        ///
        /// This noise module requires one source module.
        class ScaleBias : public ModuleBase
        {
        public:
            /// Constructor.
            ///
            /// The default bias is set to noise::module::DEFAULT_BIAS.
            ///
            /// The default scaling factor is set to noise::module::DEFAULT_SCALE.
            ScaleBias(const noise::ScalarParameter& src);
            ScaleBias(const noise::ScalarParameter& src, const noise::ScalarParameter& scale, const noise::ScalarParameter& bias);

            /// Sets the scaling factor to apply to the output value from the
            /// source module.
            ///
            /// @param scale The scaling factor to apply.
            ///
            /// The getValue() method retrieves the output value from the source
            /// module, multiplies it with the scaling factor, adds the bias to
            /// it, then outputs the value.
            void setScale(const noise::ScalarParameter& scale);

            /// Returns the scaling factor to apply to the output value from the
            /// source module.
            ///
            /// @returns The scaling factor to apply.
            ///
            /// The getValue() method retrieves the output value from the source
            /// module, multiplies it with the scaling factor, adds the bias to
            /// it, then outputs the value.
            const noise::ScalarParameter& getScale() const;

            /// Sets the bias to apply to the scaled output value from the source
            /// module.
            ///
            /// @param bias The bias to apply.
            ///
            /// The getValue() method retrieves the output value from the source
            /// module, multiplies it with the scaling factor, adds the bias to
            /// it, then outputs the value.
            void setBias(const noise::ScalarParameter& bias);

            /// Returns the bias to apply to the scaled output value from the
            /// source module.
            ///
            /// @returns The bias to apply.
            ///
            /// The getValue() method retrieves the output value from the source
            /// module, multiplies it with the scaling factor, adds the bias to
            /// it, then outputs the value.
            const noise::ScalarParameter& getBias() const;

            virtual double getValue(double x, double y, double z) const override;

        protected:
            noise::ScalarParameter m_source;

            /// Scaling factor to apply to the output value from the source
            /// module.
            noise::ScalarParameter m_scale;

            /// Bias to apply to the scaled output value from the source module.
            noise::ScalarParameter m_bias;
        };

        /// @}

        /// @}

        /// @}

    } // namespace module

} // namespace noise

#endif
