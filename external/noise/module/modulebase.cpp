// modulebase.cpp
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

#include "modulebase.h"

using namespace noise::module;

ModuleBase::ModuleBase(int sourceModuleCount)
{
    m_numModules = sourceModuleCount;

    // Create an array of pointers to all source modules required by this
    // noise module.  Set these pointers to nullptr.
    if (sourceModuleCount > 0)
    {
        m_pSourceModule = new const ModuleBase*[sourceModuleCount];
        for (int i = 0; i < sourceModuleCount; i++)
        {
            m_pSourceModule[i] = nullptr;
        }
    }
}

ModuleBase::~ModuleBase()
{
    if (m_pSourceModule)
    {
        delete[] m_pSourceModule;
        m_pSourceModule = nullptr;
    }
}

int ModuleBase::getSourceModuleCount() const
{
    return m_numModules;
}

void ModuleBase::setSourceModule(int index, const ModuleBase& sourceModule)
{
    assert(m_pSourceModule != nullptr);
    assert(index >= 0 && index < getSourceModuleCount());
    m_pSourceModule[index] = &sourceModule;
}

const ModuleBase& ModuleBase::getSourceModule(int index) const
{
    assert(m_pSourceModule != nullptr);
    assert(index >= 0 && index < getSourceModuleCount() && m_pSourceModule[index]);
    return *(m_pSourceModule[index]);
}

const ModuleBase& ModuleBase::operator=(const ModuleBase& m)
{
    return *this;
}
